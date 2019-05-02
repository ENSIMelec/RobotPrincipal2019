#include <iostream>
#include <thread>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/time.h>

using namespace std;

#include "Asservissement2018.h"
#include "SerialCodeurManager.h"
#include "ServoManager.h"
#include "BlocageManager.h"
#include "ActionManager.h"
#include "Strategie.h"

#define DISABLE_WATCHDOG 2
#define ENABLE_WATCHDOG 3

void StopSignal(int);
void StopSignalBrutal(int);

const char* DUMMY_SHORTARG = "-dm";
const char* DUMMY_LONGARG = "--dummy-motors";

bool args_contains(int argc, char** args, const char* compare);

MoteurManager *p_moteurs;
BlocageManager *p_blocage;
ServoManager *p_servo;
SerialCodeurManager *p_codeurs;


std::thread *threadAsserv;

bool stop_asserv = false;
bool maindebug = true;

bool demandePoint = false;
timer asservTimer;
bool actionDone = false;
bool actionencour = false;
bool actionencoursAttente = true;
bool actionAttente = false;

Config config;

int temps_match = 90;

int moteursI2c = -1;
int I2C_SERVO = 6;

string fileaction = "null";


bool funny = false;
bool a = false;


void actionThread()
{
	while(1)
	{
		if( a == true )
		{
				//INITIALISATION ACTION
				a = false;
				actionencoursAttente = true;
				string f = fileaction;
				fileaction = "";
				actionencour = true;
  				ACTION = &action;
				ACTION->Action(f);
				f = "";
				actionencour = false;
				actionencoursAttente = false;
		}
	}
}

void asservir_callback(Asservissement asserv)
{

	timer temps;
    timer timeout;
    //timer timeatt;
    funny = false;

	while (true)
	{
		cout << "Temps: " << temps.elapsed_ms()<< "ms " << temps.elapsed_s() << "s" << endl;

		if(stop_asserv)
		{
			p_moteurs->stop();
			p_moteurs->apply();
			exit(0);
		}


		if(asserv.isFinished())
			continue;

		if(asserv.getPointActuel().getRecalage() != 0)
		{
			p_moteurs->stop();
			p_moteurs->apply();

			Point recalage = asserv.getPointActuel();
			recalage.setRecalage(0);
			recalage.display();
			asserv.initialiser(recalage);
			printf("recalage succes\n");
			asserv.autoriserPointSuivant();
            asserv.suite = 2;
			//continue;

		}

		/**** GESTION DES CAPTEURS ****/
		// Récupération des endroits de blocage
		PositionBlocage blocage = p_blocage->isBlocked();

		// Récupération de la config de blocage pour le point actuel
		int blocagePoint = asserv.getPointActuel().getBlocage();

		// Si le point est bloquant
		//cout <<  "point bloquant :" << asserv.pointActuel().getBlocage() << endl;
		if (asserv.getPointActuel().getBlocage()) {
			//cout << "Le point est bloquant" << endl;

			//cout << "blocage " << static_cast<int>(blocage) << endl;

			// On sépare les infos de blocage réel en un blocage avant et arrière
			bool blocageAvant = static_cast<int>(blocage) & 1;
			bool blocageArriere = static_cast<int>(blocage) & 2;

			//cout << "blocage avant " << blocageAvant << endl;
			//cout << "blocage arriere " << blocageArriere << endl;

			// On vérifier où on doit regarder par rapport au point
			// 1 => blocage avant
			// 2 => blocage arrière
			// 3 => blocage avant + arrière
			bool verifierAvant = blocagePoint & 1;
			bool verifierArriere = blocagePoint & 2;

			// Expression logique pour savoir si on doit s’arrêter
			bool bloque = (verifierAvant && blocageAvant) || (verifierArriere && blocageArriere);
			//cout << "verifierAvant : " << verifierAvant  << endl;
			// Si on est bloqué on arrête le robot
			if (bloque)
			{
				// On stoppe le moteur et ne l’asservi plus
				p_moteurs->stop();
				p_moteurs->apply();
				p_moteurs->dummyBlocage = true;

				// On explique quel(s) capteur(s) bloque le robot
				//cout << "BLOCAGE" << endl;
			}
			else
			{
				// Si on est pas/plus bloqué on asservi vraiment les moteurs
				p_moteurs->dummyBlocage = false;
			}
		}

		//enable engine watchdog
		//char data = ENABLE_WATCHDOG;
		//write(moteursI2c, &data, 1);

		/*Attente entre chaque point */
		if(demandePoint)
		{
			cout << asservTimer.elapsed() << endl;
			if(asservTimer.elapsed() >= config.getDeltaPointSuivant())
			{
				cout << "Point autorisé" << endl;
				asserv.autoriserPointSuivant();
				demandePoint = false;
                timeout.restart();
			}
			else
			{
				asserv.asservir();
				int delta = config.getDeltaAsserv();
				std::this_thread::sleep_for(std::chrono::milliseconds(delta));
			}
		}
		else if(asserv.demandePointSuivant)
		{
			demandePoint = true;
			asservTimer.restart();
		}
		else // itération normale
		{
            //Si on dépasse le temps autorisé pour atteindre le point...
            if(asserv.getPointActuel().getTimeout() != 0 &&
                timeout.elapsed_ms() >= asserv.getPointActuel().getTimeout()){
                //... on force le point suivant.
                asserv.autoriserPointSuivant();
                asserv.suite = 2;
                timeout.restart();
            }

			/* Itération de l'asservissement */
			asserv.asservir();

			/* Affichage des propriétés de l'asservissement en temps réel*/

			if(true){
				cout << "X:"  << asserv.xA << " Y: " << asserv.yA << " Angle : " << asserv.orientation << endl;
			}

			int delta = config.getDeltaAsserv();
			std::this_thread::sleep_for(std::chrono::milliseconds(delta));

		}

	}
	asserv.stop();

}


int main(int argc, char** args)
{
	// Arguments
	int couleur, num_strat;
	//Lecture des points dans un fichier
	cout << "Entrer le fichier de points de depart a lire : " << endl;
	string nomFile;
	getline(cin, nomFile);
	nomFile = "filepoint3.1/"+nomFile+".point";
	cout << nomFile << endl;



	if (argc < 3) {
		// Pas assez d’arguments
		printf("usage: asserv <couleur> <num_strat>");

		couleur = num_strat = -1;

		//return 1;
	}
	else
	{
		// Parse les arguments en entiers, pour s’en servir lors de l’ouverture du
		// fichier .point
		couleur = atoi(args[1]);
		num_strat = atoi(args[2]);

		//cout << "couleur " << couleur << " strat " << num_strat << endl;
	}



	// Configuration
	//cout << "Lecture de la configuration" << endl;

	config.loadFromFile("config.info");

	int I2C_MOTEURS = 8;
	int I2C_ACTIONNEURS1 = 0;
	int I2C_ACTIONNEURS2 = 0;

	temps_match = config.get_temps_match();


	cout << "--------------------------LANCEMENT--------------------------------------\n";

	// Initialise les GPIO de la Rasp (pour la pin ACK + blocage)
	// ATTENTION: à appeler absolument avant d’initialiser les managers
	wiringPiSetupGpio();

	/* INITIALISATION DES MOTEURS --------------------------------*/
	// Initialise l’I2C de la Rasp
	int i2c = wiringPiI2CSetup(I2C_MOTEURS);
	int i2cS = wiringPiI2CSetup(I2C_SERVO);

	if (i2c < 0)
	{
		perror("wiringPiI2CSetup");
		return -1;
	}
	if (i2cS < 0)
	{
		perror("wiringPiI2CSetup");
		pthread_exit(NULL);
	}
	ServoManager servo(i2cS);
	p_servo = &servo;

	moteursI2c = i2c;


	// Création du MoteurManager qui s’occupe de simuler les deux moteurs
	MoteurManager moteurs(i2c);
	p_moteurs = &moteurs;
	if(args_contains(argc, args, DUMMY_LONGARG) ||
		args_contains(argc, args, DUMMY_SHORTARG))
	{
		moteurs.dummy = true;
		cout << "Dummy motors ON\n" << endl;
	}
	else
		cout << "Dummmy motors OFF" << endl;
	stop_asserv = false;


	/* INITIALISATION DES CODEURS -----------------------------*/

		// Création du groupement de deux codeurs
	SerialCodeurManager codeurs(0);

	//reset du lancement précédent
	codeurs.CloseS();
	codeurs.Initialisation();
	codeurs.reset();
	codeurs.reset();
	delay(1000);




	/* INITIALISATION DU BLOCAGE --------------------------------*/

	// Création du gestionnaire de blocage
	BlocageManager blocage;
	p_blocage = &blocage;



	/* INITIALISATION DES POINTS --------------------------------*/

	std::vector<Point> points;


	points = Fichier::readPoints(nomFile);



	Point depart = points.at(0);
	//points.erase (points.begin(),points.begin()+1);
	Strategie strat(nomFile);

	Asservissement asserv(moteurs, codeurs, strat);

	// Réglage de l’asserv depuis le fichier de config
	asserv.loadConfig(config);
	/*asserv.CoeffGLong = config.getCoeffGLong();
	asserv.CoeffDLong = config.getCoeffDLong();
	asserv.CoeffGAngl = config.getCoeffGAngl();
	asserv.CoeffDAngl = config.getCoeffDAngl();*///robot 2015-2016

	asserv.CoeffGLong =  0.189933523267;
	asserv.CoeffDLong =  0.188040616773;
	/*asserv.CoeffGAngl =  0.039381667154382;
	asserv.CoeffDAngl =  0.040540492484006;*/
	/*asserv.CoeffDAngl = 0.040960536466591;
	asserv.CoeffGAngl = 0.039839884796946;*/

	asserv.CoeffDAngl = 0.040474142559561;
	asserv.CoeffGAngl = 0.040519524986098;

	/*asserv.CoeffDAngl = 0.039997720824579;
	asserv.CoeffGAngl = 0.039826983535435;*/

	asserv.initialiser(depart);
	printf("\nDEPART:");
	asserv.sleepI2c = 0; //0 à +inifini
	asserv.seuilMoteurs = 20; //0 à 255
	depart.display();



	/* TRAITEMENT DU JACK --------------------------------*/
    //Initialisation du jack
    /*pinMode(17, INPUT);
    //Attente du jack
    while(digitalRead(17)!=0)
    {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}*/
	codeurs.reset();
	codeurs.reset();

	// Création d’un thread qui va appeler l’asservissement toutes les X
	// millisecondes
	std::thread t(asservir_callback, asserv);
	std::thread actionThreadt(actionThread);

	threadAsserv = &t;

	// Interception du Ctrl-C pour l'arret des moteurs
	signal(SIGINT, StopSignal);

	// Attente de la fin du thread d’asservissement
	t.join();
	actionThreadt.join();

	// Fermeture du fichier jouant le rôle d'I2C
	close(i2c);

	//arret moteur


	return 0;
}


void  StopSignal(int sig)
{
	(void) sig; // Supprime warning unused
	printf("Forcing stop\n");
	stop_asserv = true;
}

bool args_contains(int argc, char** args, const char* compare)
{
	int i;
	for(i = 0; i < argc; i++)
	{
		if(strcmp(args[i], compare) == 0)
			return true;
	}
	return false;
}
