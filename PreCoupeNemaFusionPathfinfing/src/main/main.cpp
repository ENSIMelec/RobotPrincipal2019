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
#include "lidar.h"

/*********************************** Define ************************************/

#define EXIT_SUCCESS 0
#define EXIT_FAIL_PARAM -1
#define EXIT_FAIL_I2C -2
#define EXIT_FORCING_STOP -99

#define PIN_JACK 17
#define PIN_ARU 5
#define PIN_LED 16

/***************************** Variables globales ******************************/
unsigned int temps_match;
unsigned int deltaAsservTimer;
unsigned int nbAppelsAsserv;
bool forcing_stop;
unsigned int nbActionFileExecuted;

string PATH = "/home/pi/PreCoupeNemaFusionPathfinfing/";

/********************************* prototypes **********************************/

bool argc_control(int argc);
bool argv_contains_dummy(int argc, char** argv);

void couleurThreadFunc(Strategie& strat, ClientUDP& client);
void actionThreadFunc(ActionManager& ACTION, string filename, bool& actionEnCours, bool& actionDone);

void jouerMatch(Asservissement2018& asserv, Strategie& strat, MoteurManager *p_moteurs, ActionManager& actions, timer& temps,  ClientUDP& client);

bool recontreObstacle(Strategie& strat, PositionBlocage posBloc);
bool ARUisNotPush();

void stopSignal(int signal);
void sleepMillis(int millis);


Lidar* lid;

bool relancer = false;

void lidar(Lidar *lidar){
		lidar->Scan();
		cout<<"thread Lidar"<<endl;
		relancer = true;
}

void electronThreadFunc(bool &confirm){
	int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	struct sockaddr_in electron;
	electron.sin_family = AF_INET;
    electron.sin_port = htons(5757);
    electron.sin_addr.s_addr = inet_addr("172.30.1.20");
    char * paquetRecu;
    string str_deb = "On";
    string str_fin = "Off";

    socklen_t addr = sizeof(electron);

	/*Envoi de la commande de lancement*/
	int lancement = sendto(sockfd, str_deb.c_str(), str_deb.size()+1, 0,(struct sockaddr *)&electron, addr);
	if(lancement <= 0){
		cout << "ERREUR LORS DE L'ENVOI" << endl;
	}else{
		cout << "Envoi réussi" << endl; 
	}

	/*Réception de la confirmation*/
	int retour_deb =  recvfrom(sockfd, paquetRecu, 25, 0, (struct sockaddr *)&electron, &addr);
	if(retour_deb <= 0){
		cout << "Erreur lors de la réception" << endl;
	}
	else{
		cout << "Message recu : " << paquetRecu << endl;
	}

	while(strcmp(paquetRecu, "OK") != 0){
		sendto(sockfd, str_deb.c_str(), str_deb.size()+1, 0,(struct sockaddr *)&electron, addr);
		recvfrom(sockfd, paquetRecu, 25, 0, (struct sockaddr *)&electron, &addr);
	}

	confirm = true;

}

void experienceThreadFunc(bool &confirm){
	/*Déclaration de variables pour recevoir les datagrammes*/
	int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	struct sockaddr_in experience;
	experience.sin_family = AF_INET;
    experience.sin_port = htons(5757);
    experience.sin_addr.s_addr = inet_addr("172.30.1.30");
    char * paquetRecu;
    string str = "On";


	//system("curl 172.30.1.30/on");
	/*Envoi de la commande*/
	int envoi = sendto(sockfd, str.c_str(), str.size()+1, 0,(struct sockaddr *)&experience, sizeof(experience));
	if(envoi <= 0){
		cout << "ERREUR LORS DE L'ENVOI" << endl;
	}else{
		cout << "Envoi réussi" << endl; 
	}

	/*Réception de la confirmation*/
	socklen_t addr = sizeof(experience);
	int retour =  recvfrom(sockfd, paquetRecu, 25, 0, (struct sockaddr *)&experience, &addr);
	if(retour <= 0){
		cout << "Erreur lors de la réception" << endl;
	}
	else{
		cout << "Message recu : " << paquetRecu << endl;
	}

	while(strcmp(paquetRecu, "OK") != 0){
		sendto(sockfd, str.c_str(), str.size()+1, 0,(struct sockaddr *)&experience, sizeof(experience));
		recvfrom(sockfd, paquetRecu, 25, 0, (struct sockaddr *)&experience, &addr);
	}
	confirm = true;

}

///////////////////////////// PROGRAMME PRINCIPAL ///////////////////////////////
int main(int argc, char **argv) {
/***************************** Début du programme ******************************/
	cout << "Debut du programme" << endl;

/*************************** Controle des paramètres ***************************/
	cout << "Controle des parametres" << endl;
	if(!argc_control(argc))
		return EXIT_FAIL_PARAM;


    // Interception du Ctrl-C pour l'arret des moteurs
	signal(SIGINT, stopSignal);


/***************** Déclaration et initialisation des variables *****************/

	// Initialise les GPIO de la Rasp (pour la pin ACK + blocage)
	// ATTENTION: à appeler absolument avant d’initialiser les managers
	wiringPiSetupGpio();

	Config config;
	config.loadFromFile(PATH + "config.info"); //Charge la configuration à partir du fichier config.info

	int i2cM = wiringPiI2CSetup(config.get_I2C_MOTEURS());
	int i2cS = wiringPiI2CSetup(config.get_I2C_SERVOS());
	int i2cSt = wiringPiI2CSetup(9);//Adresse i2c du nema, devrait passer dans le fichier conf si jamais

	if(i2cM < 0 || i2cS < 0 || i2cSt < 0)
		return EXIT_FAIL_I2C;

	pullUpDnControl(2, PUD_UP);
	pullUpDnControl(3, PUD_UP);


	MoteurManager moteurs(i2cM);
	MoteurManager *p_moteurs = &moteurs;
	cout << "Déclaration et initialisation des variables" << endl;

	argv_contains_dummy(argc, argv); //En fonction des paramètres du main, on active les dummy motors ou non



	// Création du groupement de deux codeurs
	SerialCodeurManager codeurs(0);
	//reset du lancement précédent
	codeurs.Closes();
	codeurs.Initialisation();
	delay(100);



	char nomFile[100];
	sprintf(nomFile, "%sfilepoint3.1/%s/", PATH.c_str(), argv[1]); //Dossier contenant le fichier main.strat et les fichier .point
	Strategie strat(nomFile);

	//Setup Connexion udp to Serveur
	string ipServeur = config.getIpServeur();
	int portServeur = config.getPort();
	ClientUDP client(ipServeur, portServeur);

	ActionManager actions(i2cS, i2cSt, config.getNbAX12(), client);

    temps_match = config.get_temps_match();
    deltaAsservTimer = config.getDeltaAsserv();
    timer temps;

    Asservissement2018 asserv(moteurs, codeurs, config, temps, client);
	asserv.initialiser(strat.getPointActuel());

    pinMode(PIN_JACK, INPUT);
    pinMode(PIN_ARU, INPUT);
    pinMode(PIN_LED, OUTPUT);

/***************************** Départ du robot *********************************/


	int speed = 5;	
	
	cout<<"speed init"<<endl;
	lid = new Lidar();	
	cout<<"lid init"<<endl;

	thread lidarThread1(lidar,lid);
	cout<<"thread1 init"<<endl;

	sleepMillis(3000);
	thread lidarThread2;

	bool electronLance = false;
	bool experienceLancee = false;

	cout << "Attente du jack" << endl;
	while(digitalRead(PIN_JACK) == 1) {
		if(ARUisNotPush()) {//Led Allumée
			digitalWrite(PIN_LED, 1);
		} else {//Blink normal -> Il faut enlever l'ARU
			if(temps.elapsed_ms() > 1000)
				temps.restart();
			else if(temps.elapsed_ms() < 500)
				digitalWrite(PIN_LED, 0);
			else
				digitalWrite(PIN_LED, 1);
		}
		sleepMillis(1);
	}
	digitalWrite(PIN_LED, 1);

	temps.restart();

	cout << "Fin d'attente" << endl;

	cout << "Depart du robot" << endl;
	codeurs.reset();
	codeurs.reset();
	cout <<"Codeur reset"<<endl;
	nbAppelsAsserv = 0;

	cout << "Lancement du thread de recepetion des couleurs" << endl;
	//thread (couleurThreadFunc, ref(strat), ref(client)).detach();//créer un nouveau Thread qui attend la reception des couleurs et qui met à jour un statut strategie

	//Lancement des threads pour l'électron et l'expérience
	thread(electronThreadFunc, ref(electronLance).detach();
	thread(experienceThreadFunc, ref(experienceLancee)).detach();
	
	
	
	if(!relancer){
		goto statement;
	}
	
	lid = new Lidar();
	lidarThread2 = thread{lidar,lid};
	cout<<"thread2 init"<<endl;
	sleepMillis(1000);
	
statement:
	
	
	
	jouerMatch(ref(asserv), ref(strat), p_moteurs, ref(actions), ref(temps), ref(client));

	delete lid;

	if(forcing_stop) {
		cout << "Forcing stop" << endl;
		delete lid;		
	} else if(!ARUisNotPush()) {
		cout << "ARU" << endl;
	} else {
		cout << "Arrivee du robot" << endl;
	}
	client.sendMessage("P "+to_string(client.getPoints()));
	cout << temps_match-temps.elapsed_s() << " secondes restantes avant la fin du match" << endl;
	cout << "Nombre de fichier action exécutés : " << nbActionFileExecuted << endl;

/************************ Libération de la mémoire *****************************/
	cout << "Liberation de la memoire" << endl;

	actions.close(); //Ferme les AX12
	codeurs.Closes();
	close(i2cM);
	close(i2cS);
	close(i2cSt);
	digitalWrite(PIN_LED, 0);

/***************************** Fin du programme ********************************/

	cout << "Fin du programme" << endl;
	cout << nbAppelsAsserv << endl;
	return forcing_stop ? EXIT_FORCING_STOP : EXIT_SUCCESS;
}


/////////////////////////// FIN PROGRAMME PRINCIPAL /////////////////////////////


void actionThreadFunc(ActionManager& ACTION, string filename, bool& actionEnCours, bool& actionDone) {
	cout << "Debut du THREAD action " << filename << endl;
	ACTION.action(PATH + "fileaction/" + filename + ".as");
	actionEnCours = false;
	actionDone = true;
	cout << "Fin du THREAD action " << filename << endl;
	return;
}



void jouerMatch(Asservissement2018& asserv, Strategie& strat, MoteurManager *p_moteurs, ActionManager& actions, timer& temps, ClientUDP& client) {
	BlocageManager blocage(lid); //Gestionnaire de blocage
	timer tempsBlocage, asservTimer, timeOut;
	bool weAreBlocked = false; //Est ce qu'on bloque ?
	bool thereIsAnAction = false, actionDone = false, actionEnCours = false;
	//Saute le point d'initialisation
	nbActionFileExecuted = 0;

	while(!forcing_stop && strat.isNotFinished() && temps.elapsed_s() < temps_match && ARUisNotPush()) {
	//Tant qu'on ne force pas l'arrêt, et que la strat n'est pas terminée, et qu'on dépasse pas le temps
		if((!strat.haveToWaitActionDone() || actionDone) && (asserv.demandePointSuivant() || weAreBlocked || ((temps.elapsed_ms() > strat.getTimeOut()) && (strat.getTimeOut() != 0)))) {
		//Si on a pas besoin d'attendre la fin de l'action en cours ET
			//que l'asservissement demande le point suivant (parce qu'on est arrivé à destination)
			//ou qu'on est bloqué et qu'on doit changer de point
			//if((!strat.haveToWaitActionDone() || actionDone) && (asserv.demandePointSuivant() || weAreBlocked || (timeOut.elapsed_ms() > 5000)))//ou qu'on dépasse un timeOut
			cout << "changement de point" << endl;

			if(weAreBlocked){
				cout << "WeAreBlocked" <<endl;

				//Blocage des noeuds autour de celui qu'on détecte
				int XNodeToBlock, YNodeToblock;
				lid->cartesianFromLidar(&XNodeToBlock, &YNodeToblock);
				cout << "Point bloque : " << XNodeToBlock << " ; " << YNodeToblock << endl;
				Point robotBloqueIci = asserv.getCoordonnees();
				if(robotBloqueIci.getX() >= 0 && strat.getPointActuel().getSens() == 0){
					XNodeToBlock = robotBloqueIci.getX() - XNodeToBlock;
					YNodeToblock = robotBloqueIci.getY() - YNodeToblock;
				}
				else if(robotBloqueIci.getX() < 0 && strat.getPointActuel().getSens() == 0){
					XNodeToBlock = robotBloqueIci.getX() + XNodeToBlock;
					YNodeToblock = robotBloqueIci.getY() + YNodeToblock;
				}
				
				cout << "Point bloque : " << XNodeToBlock << " ; " << YNodeToblock << endl;

				Noeud nodeToBlock(false, new Coordonnee(XNodeToBlock, YNodeToblock), strat.getMaTable()->getMap().getMapping().size());
				strat.blockNodes(&nodeToBlock);

				//On se prépare à reculer vers le noeud le plus proche de l'endroit de la table où on se trouve

				strat.setObjectifAatteindre(strat.getPointActuel());
				Noeud* currentNode = strat.createNodeByPoint(asserv.getCoordonnees());
				cout <<"currentNode cree" <<endl;
				Noeud* noeudPlusProche = strat.getMaTable()->graph.noeudLePlusProche(currentNode);
				cout <<"noeudPlusProche cree : " << noeudPlusProche->getId() <<endl;
				Point pointRecul = strat.convertNodeIntoPoint(noeudPlusProche);
				cout <<"pointRecul cree" <<endl;

				pointRecul.setSens(1);
				pointRecul.setVitesse(400);
				asserv.pointSuivant(pointRecul);

				// Préparation pour le pathfinding
				strat.setPathfindingInAction(true);
				strat.setDepartPathfinding(pointRecul);
			}else{
				asserv.pointSuivant(strat.getPointSuivant()); // On demande le point suivant à la strategie (elle met à jour le point actuel/courant), et on le donne à l'asservissement
			}


			strat.setStatut("null");
			weAreBlocked = false; //On est plus bloqué, on vient de changer de point
			timeOut.restart();
			if(((strat.getFileAction()).compare("null") != 0) && ((strat.getFileAction()).compare("") != 0)) { //Si il y a un fileAction
				thereIsAnAction = true;
				actionDone = false;
			} else {
				thereIsAnAction = false;
				actionDone = true; //Permet de sécurisé le fait que si le point demande la fin de l'action, et qu'il n'y a pas d'action à lancer (null), alors l'action est marquée comme "faite"
			}
			actionEnCours = false;
		}
		//cout<<"Time: "<<asservTimer.elapsed_s()<<"delta: "<<deltaAsservTimer<<endl;
		if(asservTimer.elapsed_ms() >= deltaAsservTimer) {
			asserv.asservir(); //Asservit les moteurs pour se déplacer au point demandé
			asservTimer.restart();
			nbAppelsAsserv++;
			//Envoi du statut des capteurs de détection au serveur
		}

		//Gestion des blocages moteurs
		if(asserv.blocageMoteur() == true) {
			p_moteurs->stop();
			asserv.pointSuivant(strat.getPointActuel());
			//Donne à l'asserv un pour pour s'éloigner du blocage en fonction des coordonnées du robot
			//asserv.pointSuivant(strat.eloigner(asserv.getCoordonnees(), 200)); //On demande à l'asserv de l'éloigner de 200mm du point actuel
		}

		//Gestion des capteurs


		if(asserv.getPointActuel().getDetection() == 1){			
			lid->detectionAvant = true;
			lid->detectionArriere = false;
		}
		else if(asserv.getPointActuel().getDetection() == 2){
			lid->detectionAvant = false;
			lid->detectionArriere = true;
		}
		else{
			lid->detectionAvant = false;
			lid->detectionArriere = false;
		}

		//cout << "Pallier : " << lid->speed << endl;
		switch(lid->speed){
			case 1: asserv.setVitessePointActuel(166); break;
			case 2: asserv.setVitessePointActuel(333); break;
			case 3: asserv.setVitessePointActuel(500); break;
			default: asserv.setVitessePointActuel(500); break;
		}
		//cout << "La vitesse d'arrivée au point = " << asserv.getPointActuel().getVitesse() << endl;


		tempsBlocage.restart(); //On compte le temps passé bloqué

		
		while(lid->speed == 0 && tempsBlocage.elapsed_ms() < 1000 && ARUisNotPush() && temps.elapsed_s() < temps_match) {
			//Si on rencontre un obstacle genant
			//On attend pendant 3 secondes, tant qu'on est bloqué par un obstacle genant
			p_moteurs->stop(); // On recule ou pas ??
		}

		p_moteurs->dummyBlocage = false; //On débloque les moteurs
		//Maintenant, soit le temps est dépassé (et donc il y a techniquement encore l'obstacle), soit le temps n'est pas dépassé (et donc il n'y a plus d'obstacle)
		//posBloc = blocage.isBlocked();
		if(lid->speed == 0 && ARUisNotPush() && temps.elapsed_s() < temps_match) { //Si il y a encore un obstacle genant
			p_moteurs->stop(); //On arrête les moteurs (au final, on a fait, si besoin, un petit déplacement en 1 seconde)
			strat.setStatut("Bloque");
			weAreBlocked = true; //On doit changer de point, sachant qu'on dit à la strat qu'on est bloqué -> On va changer d'objectif
		}

		//Gestion des actions
		if(thereIsAnAction && !actionDone && !actionEnCours && strat.isNotFinished() && ARUisNotPush() && temps.elapsed_s() < temps_match) { //Si on a une action a faire et qu'elle n'est pas faite et qu'elle n'est pas en cours
			thread (actionThreadFunc, ref(actions), strat.getFileAction(), ref(actionEnCours), ref(actionDone)).detach();//créer un nouveau Thread qui effectue l'action
			actionEnCours = true;
			nbActionFileExecuted++;
		}
	}
	asserv.stop();
 	sleepMillis(100); //Permet de laisser le temps de demander l'arrêt des moteurs :)
	asserv.stop();
	sleepMillis(100);
	strat.getPointActuel().display();
	return;
}

bool recontreObstacle(Strategie& strat, PositionBlocage posBloc) {
	int sensDeplacement = strat.getSensDeplacement();
	int optionDetection = strat.getOptionDetection();

	if( (sensDeplacement == 0 && optionDetection == 1 && (posBloc == PositionBlocage::AVANT)) ||
		(sensDeplacement == 1 && optionDetection == 2 && (posBloc == PositionBlocage::ARRIERE)) ||
		(optionDetection == 3 && posBloc != PositionBlocage::AUCUN))
	{
		return true;
	} else { //Si on est pas gené par un obstacle
		return false;
	}
}

bool ARUisNotPush() {
	return digitalRead(PIN_ARU) == 1;
}

bool argc_control(int argc) {
/**
 * Renvoie EXIT_SUCCESS si tout va bien,
**/
	if(argc == 2)
		return true;
	else
		return false;
}

bool argv_contains_dummy(int argc, char** argv) {
/**
 * Renvoie true si un des arguments demande des dummy motors
**/
	//dummy motors
	const char* DUMMY_SHORTARG = "-dm";
	const char* DUMMY_LONGARG = "--dummy-motors";

	for(int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], DUMMY_SHORTARG) == 0 || strcmp(argv[i], DUMMY_LONGARG))
			return true;
	}
	return false;
}


void stopSignal(int signal) {
	if(signal)
		cout << "CTRL+C détecté" << endl;
	forcing_stop = true;
	return;
}

void sleepMillis(int millis) {
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}
