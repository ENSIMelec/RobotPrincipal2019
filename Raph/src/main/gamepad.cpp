#include <iostream>
#include <thread>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/time.h>
#include <cmath>

using namespace std;

#include "Asservissement2018.h"
#include "SerialCodeurManager.h"
#include "ServoManager.h"
#include "BlocageManager.h"
#include "ActionManager.h"
#include "Strategie.h"

#define DISABLE_WATCHDOG 2
#define ENABLE_WATCHDOG 3



#define IP "172.24.1.128"
#define PORT 1111    /* the port client will be connecting to */
#define MAXDATASIZE 8 /* max number of bytes we can get at once */

MoteurManager *p_moteurs;
BlocageManager *p_blocage;
ServoManager *p_servo;
SerialCodeurManager *p_codeurs;


std::thread *threadAsserv;

bool stop_asserv = false;
bool maindebug = true;
bool blocageTime = false;

Config config;

bool demandePoint = false;
timer asservTimer;
bool actionDone = false;
bool actionencour = false;
bool actionencoursAttente = true;
bool actionAttente = false;


int temps_match = 89;

int moteursI2c = -1;
int I2C_SERVO = 6;

string fileaction = "null";


bool funny = false;
bool a = false;

const char* DUMMY_SHORTARG = "-dm";
const char* DUMMY_LONGARG = "--dummy-motors";


int sockfd, numbytes;
char buf[MAXDATASIZE];
struct hostent *he;
struct sockaddr_in their_addr; /* connector's address information */

void StopSignal(int);
void StopSignalBrutal(int);



bool args_contains(int argc, char** args, const char* compare);

void asservir_callback(Asservissement2018 asserv);

const vector<string> explode(const string& s, const char& c);




int main(int argc, char** args)
{
	cout << "start" << endl;
	// Arguments
	int couleur, num_strat;
	char nomFile[100];
	sprintf(nomFile,"filepoint3.1/%s/",args[1]); //Dossier contenant le fichier main.strat et les fichier .point


	// Configuration
	//cout << "Lecture de la configuration" << endl;
	config.loadFromFile("config.info");

	int I2C_MOTEURS = 8;
	int I2C_ACTIONNEURS1 = 0;
	int I2C_ACTIONNEURS2 = 0;



	cout << "-------------------------- LANCEMENT --------------------------" << endl;

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

	//Setup Connexion udp to Serveur
	string ipServeur = config.getIpServeur();
	int portServeur = config.getPort();
	ClientUDP client(ipServeur, portServeur);
    //------------------------------------Socket------------------------------------------
	cout << "Connexion au serveur de la manette\n" << endl;
    if ((he=gethostbyname(ipServeur)) == NULL) {  /* get the host info */
        herror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;      /* host byte order */
    their_addr.sin_port = htons(portServeur);    /* short, network byte order */
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

    if (connect(sockfd, (struct sockaddr *)&their_addr, \
                                          sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }
    cout << "Connexion ok"<<endl;

	/*-------------------------------- INITIALISATION DES CODEURS ------------------------------------*/

	// Création du groupement de deux codeurs
	SerialCodeurManager codeurs(0);

	//reset du lancement précédent
	codeurs.CloseS();
	codeurs.Initialisation();
	codeurs.reset();
	codeurs.reset();
	delay(1000);


	/*------------------------------------ INITIALISATION DU BLOCAGE ---------------------------------*/

	// Création du gestionnaire de blocage
	BlocageManager blocage;
	p_blocage = &blocage;

	/*----------------------------------- INITIALISATION DES POINTS ----------------------------------*/

	//points.erase (points.begin(),points.begin()+1);

	// Réglage de l’asserv depuis le fichier de config
	//asserv.loadConfig(config);
	Asservissement2018 asserv(moteurs, codeurs, config, client);
	asserv.initialiser(Point());
	cout << endl << "Go !!!!" << endl;
	//asserv.sleepI2c = 0; //0 à +inifini
	//asserv.seuilMoteurs = 255; //0 à 255

	/*---------------------------------------- TRAITEMENT DU JACK ------------------------------------*/

    //Initialisation du jack
    pinMode(17, INPUT);

    //Attente du jack
    while(digitalRead(17) == 0)
    {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	codeurs.reset();
	codeurs.reset();

	// Création d’un thread qui va appeler l’asservissement toutes les X millisecondes
	std::thread stratThread(asservir_callback, asserv);

	threadAsserv = &stratThread;

	// Interception du Ctrl-C pour l'arret des moteurs
	signal(SIGINT, StopSignal);

	// Attente de la fin du thread d’asservissement
	stratThread.join();

	// Fermeture du fichier jouant le rôle d'I2C
	close(i2c);
	//arret moteur
	cout << "end" << endl;
	return 0;
}




void asservir_callback(Asservissement2018 asserv){
	char *data[2];
	int valeur;
	float vit=0;
	float rot=0;

    while(stop_asserv == false){

		memset(buf,'\0',MAXDATASIZE);
        numbytes=recv(sockfd, buf, MAXDATASIZE, 0);
		if(numbytes!=0  && strstr(buf," ")!=NULL){
			cout <<"["<<buf<<"]"<<endl;
			data[0] = strtok(buf, " "); // Splits spaces between words in str
		    data[1] = strtok (NULL, " ");
			valeur = atof(data[1]);
			cout << "["<<data[0]<<"] ["<<valeur<<"]"<<endl;
			if(data[0][0]=='D'){
				vit=(valeur*512/200);
			}
			if(data[0][0]=='G'){
				rot=(valeur*512/200);
			}

			if(data[0][0]=='B'){
				vit=0;
				rot=0;
			}
			if(data[0][0]=='X'){
				asserv.resetTick();
			}

			if(data[0][0]=='L'){
				vit=0;
				rot=-(30*512/200);
			}

			if(data[0][0]=='R'){
				vit=0;
				rot=(30*512/200);
			}

			if(data[0][0]=='S'){
				asserv.stop();
				exit(0);
			}

		}
		if(abs(vit)<=25){
			asserv.setMoteur(rot, -rot);
		}else{
			if( vit>0){
				if(rot<0){
					cout <<"G :"<<vit+rot <<" D :"<<vit<<endl;
					asserv.setMoteur(vit+rot, vit);
				}else{
					cout <<"G :"<<vit <<" D :"<<vit-rot<<endl;
					asserv.setMoteur(vit, vit-rot);
				}
			}else{
				if(rot<0){
					cout <<"G :"<<vit <<" D :"<<vit+rot<<endl;
					asserv.setMoteur(vit-rot, vit);
				}else{
					cout <<"G :"<<vit-rot <<" D :"<<vit<<endl;
					asserv.setMoteur(vit, vit+rot);
				}
			}
		}
		asserv.PrintTick();



	}
	asserv.stop();
	exit(0);
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


const vector<string> explode(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);

	return v;
}
