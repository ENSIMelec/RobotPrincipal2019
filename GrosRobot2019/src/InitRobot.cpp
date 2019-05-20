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
		cout << "Attente de connexion de l'electron" << endl;
		while(electronIsConnected() != true);
		cout << "Fin d'attente" << endl;
		
		cout << "Attente de connexion de l'experience" << endl;
		while(experienceIsConnected() != true);
		cout << "Fin d'attente" << endl;

		cout << "Attente de connexion du second robot" << endl;
		while(kiroulpaIsConnected() != true);
		cout << "Fin d'attente" << endl;
	}

	cout << "Attente du jack" << endl;
	while(jackIsPresent() == true) {
		if(aruIsNotPush() == true) {
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