#include "InitRobot.h"

//Constructeur par défaut : pour le robot secondaire
InitRobot::InitRobot() {

	//wiringPiSetupGpio(); fait dans le main.cpp
	pinMode(PIN_JACK, INPUT);
    pinMode(PIN_ARU, INPUT);
    pinMode(PIN_LED, OUTPUT);
    ledInitOff();
}

//Constructeur pour le robot principale
InitRobot::InitRobot(bool* p_elec, bool* p_exp, bool* p_rob) {

    p_electronConfirm = p_elec;
	p_experienceConfirm = p_exp;
	p_kiroulpaConfirm = p_rob;

	//wiringPiSetupGpio(); fait dans le main.cpp
	pinMode(PIN_JACK, INPUT);
    pinMode(PIN_ARU, INPUT);
    pinMode(PIN_LED, OUTPUT);
    ledInitOff();
}

void InitRobot::waitForRobotInitialisation(bool checkConnectedDevices) {

	if(checkConnectedDevices == true) {

		bool electronOk, experienceOk, kiroulpaOk;
		int cpt;

		//Attente électron
		cout << "Attente de connexion de l'electron" << endl;
		cpt = 0;
		electronOk = electronIsConnected();
		while(electronOk != true && cpt++<300) {	//attente de 3s
			usleep(10000);
			electronOk = electronIsConnected();
		}
		if(electronOk)
			cout << "Fin d'attente (Electron connecté)" << endl;
		else
			cout << "Fin d'attente (Electron non connecté...)" << endl;

		//Attente expérience
		cout << "Attente de connexion de l'expérience" << endl;
		cpt = 0;
		experienceOk = experienceIsConnected();
		while(experienceOk != true && cpt++<300) {	//attente de 3s
			usleep(10000);
			experienceOk = experienceIsConnected();
		}
		if(experienceOk)
			cout << "Fin d'attente (Expérience connectée)" << endl;
		else
			cout << "Fin d'attente (Expérience non connectée...)" << endl;

		//Attente kiroulpa
		cout << "Attente de connexion de Kiroulpa" << endl;
		cpt = 0;
		kiroulpaOk = kiroulpaIsConnected();
		while(kiroulpaOk != true && cpt++<300) {	//attente de 3s
			usleep(10000);
			kiroulpaOk = kiroulpaIsConnected();
		}
		if(kiroulpaOk)
			cout << "Fin d'attente (Kiroulpa connectée)" << endl;
		else
			cout << "Fin d'attente (Kiroulpa non connectée...)" << endl;
	}

	//Attente du jack et de l'arrêt d'urgence
	cout << "Attente du jack" << endl;
	while(jackIsPresent()) {
		if(aruIsNotPush()) {
			ledInitOn();
		} else {
			//Blink normal -> Il faut enlever l'ARU
			if(temps.elapsed_ms() > 1000)
				temps.restart();
			else if(temps.elapsed_ms() < 500)
				ledInitOff();
			else
				ledInitOn();
		}
		sleep_ms(1);
	}
	ledInitOn();
	temps.restart();
	cout << "Fin d'attente" << endl;
}

int InitRobot::aruIsNotPush() {
	return digitalRead(PIN_ARU) == 1;
}

int InitRobot::jackIsPresent() {
	return digitalRead(PIN_JACK) == 1;
}

int InitRobot::electronIsConnected() {
	return *p_electronConfirm;
}	

int InitRobot::experienceIsConnected() {
	return *p_experienceConfirm;
}

int InitRobot::kiroulpaIsConnected() {
	return *p_kiroulpaConfirm;
}

void InitRobot::ledInitOn() {
	digitalWrite(PIN_LED, 1);
}

void InitRobot::ledInitOff() {
	digitalWrite(PIN_LED, 0);
}

void InitRobot::sleep_ms(int delay) {
	usleep(delay*1000);
}