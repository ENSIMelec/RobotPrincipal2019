#include <iostream>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/time.h>

using namespace std;

#include "ActionManager.h"
#include "ClientUDP.h"
#include "Config.h"
#include "SerialCodeurManager.h"

/*********************************** Define ************************************/

#define EXIT_SUCCESS 0
#define EXIT_FAIL_I2C -2
#define EXIT_FORCING_STOP -99
#define EXIT_FAIL_ARGUMENT -3
#define PIN_ARU 5

/***************************** Variables globales ******************************/

bool forcing_stop;
string PATH = "/home/pi/GrosRobot2019/";

/********************************* prototypes **********************************/

bool ARUisNotPush();
void actionThreadFunc(ActionManager& ACTION, string filename, bool& actionDone);
void stopSignal(int signal);
void sleepMillis(int millis);

///////////////////////////// PROGRAMME PRINCIPAL ///////////////////////////////

int main(int argc, char **argv) {
/***************************** Début du programme ******************************/

	cout << "Debut du programme" << endl;

    // Interception du Ctrl-C pour l'arret du programme
	signal(SIGINT, stopSignal);

	if(argc < 2) {
		cout << "Too few arguments. Exit." << endl;
		exit(EXIT_FAIL_ARGUMENT);
	}
/***************** Déclaration et initialisation des variables *****************/

	// Initialise les GPIO de la Rasp (pour la pin ACK + blocage)
	// ATTENTION: à appeler absolument avant d’initialiser les managers
	wiringPiSetupGpio();

	Config config;
	config.loadFromFile(PATH + "config.info"); //Charge la configuration à partir du fichier config.info

	int i2cS = wiringPiI2CSetup(config.get_I2C_SERVOS());
	int i2cSt = wiringPiI2CSetup(9); //Adresse i2c du nema, devrait passer dans le fichier conf si jamais
		
	if(i2cS < 0 || i2cSt < 0)
		return EXIT_FAIL_I2C;

	cout << "Déclaration et initialisation des variables" << endl;

	// Création du groupement de deux codeurs
	SerialCodeurManager codeurs(0);
	//reset du lancement précédent
	codeurs.Closes();
	codeurs.Initialisation();
	delay(100);

	char nomFile[100];
	sprintf(nomFile, "%s", argv[1]); //fichier action à lire

	//Setup Connexion udp to Serveur
	string ipServeur = config.getIpServeur();
	int portServeur = config.getPort();
	ClientUDP client(ipServeur, portServeur);

	ActionManager actions(i2cS, i2cSt, config.getNbAX12(), client);

	pinMode(PIN_ARU, INPUT);

    timer temps;

/***************************** Départ du robot *********************************/
	temps.restart();

	cout << "Depart de l'action" << endl;
	codeurs.reset();
	codeurs.reset();
	cout <<"Codeur reset"<<endl;

	bool actionDone = false;
	thread (actionThreadFunc, ref(actions), nomFile, ref(actionDone)).detach();//créer un nouveau Thread qui effectue l'action

	while(!actionDone && !forcing_stop && ARUisNotPush()) sleepMillis(10);

	if(forcing_stop) {
		cout << "Forcing stop" << endl;
	} else if(!ARUisNotPush()){
		cout << "ARU" << endl;
	} else {
		cout << "Arrivee du robot" << endl;
	}

	cout << temps.elapsed_s() << " secondes d'execution" << endl;

/************************ Libération de la mémoire *****************************/
	cout << "Liberation de la memoire" << endl;

	actions.close(); //Ferme les AX12
	codeurs.Closes();
	close(i2cS);
	close(i2cSt);


/***************************** Fin du programme ********************************/

	cout << "Fin du programme" << endl;
	return forcing_stop ? EXIT_FORCING_STOP : EXIT_SUCCESS;
}


/////////////////////////// FIN PROGRAMME PRINCIPAL /////////////////////////////

void actionThreadFunc(ActionManager& ACTION, string filename, bool& actionDone) {
	cout << "Debut du THREAD action " << filename << endl;
	ACTION.action(PATH + "fileaction/" + filename + ".as");
	actionDone = true;
	cout << "Fin du THREAD action " << filename << endl;
	return;
}

bool ARUisNotPush() {
	return digitalRead(PIN_ARU) == 1;
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
