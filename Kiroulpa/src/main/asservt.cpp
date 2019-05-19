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
#include "Strategie.h"

/*********************************** Define ************************************/

#define EXIT_SUCCESS 0
#define EXIT_FAIL_PARAM -1
#define EXIT_FAIL_I2C -2
#define EXIT_FORCING_STOP -99

/***************************** Variables globales ******************************/

bool forcing_stop;

/********************************* prototypes **********************************/

bool argc_control(int argc);
bool argv_contains_dummy(int argc, char** argv);

void jouerMatch(Asservissement2018& asserv, Strategie& strat);

void stopSignal(int signal);
void sleepMillis(int millis);


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
	// Initialise les GPIO de la Rasp
	// ATTENTION: à appeler absolument avant d’initialiser les managers
	wiringPiSetupGpio();

	Config config;
	config.loadFromFile("config.info"); //Charge la configuration à partir du fichier config.info
	

	int i2cM = wiringPiI2CSetup(config.get_I2C_MOTEURS());
	
	if(i2cM < 0)
		return EXIT_FAIL_I2C;
	
	MoteurManager moteurs(i2cM);


	cout << "Déclaration et initialisation des variables" << endl;


	argv_contains_dummy(argc, argv); //En fonction des paramètres du main, on active les dummy motors ou non

	// Création du groupement de deux codeurs
	SerialCodeurManager codeurs(0);
	//reset du lancement précédent
	codeurs.Closes();
	codeurs.Initialisation();
	delay(100);

	char nomFile[100];
	sprintf(nomFile, "filepoint3.1/%s/", argv[1]); //Dossier contenant le fichier main.strat et les fichier .point
	Strategie strat(nomFile);

	//Setup Connexion udp to Serveur
	string ipServeur = config.getIpServeur();
	int portServeur = config.getPort();
	ClientUDP client(ipServeur, portServeur);

   	timer temps;

	Asservissement2018 asserv(moteurs, codeurs, config, temps, client);
	asserv.initialiser(strat.getPointActuel());

/***************************** Départ du robot *********************************/
	cout << "Depart du robot" << endl;
	codeurs.reset();
	codeurs.reset();
	cout <<"Codeur reset"<<endl;

	jouerMatch(ref(asserv), ref(strat));

	if(forcing_stop) {
		cout << "Forcing stop" << endl;
	} else {
		cout << "Arrivee du robot" << endl;
	}

/************************ Libération de la mémoire *****************************/
	cout << "Liberation de la memoire" << endl;

	codeurs.Closes();
	close(i2cM);

/***************************** Fin du programme ********************************/
	cout << "Fin du programme" << endl;
	return forcing_stop ? EXIT_FORCING_STOP : EXIT_SUCCESS;
}


/////////////////////////// FIN PROGRAMME PRINCIPAL /////////////////////////////


void jouerMatch(Asservissement2018& asserv, Strategie& strat) {
	while(!forcing_stop) {
	//Tant qu'on ne force pas l'arrêt, et que la strat n'est pas terminée, et qu'on dépasse pas le temps
		asserv.PrintTick();
		sleepMillis(10);
	}
	asserv.stop();
 	sleepMillis(100); //Permet de laisser le temps de demander l'arrêt des moteurs :)
	strat.getPointActuel().display();
	return;
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
