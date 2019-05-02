#ifndef ACTIONMANAGER_H_INCLUDED
#define ACTIONMANAGER_H_INCLUDED

#include <iostream>
#include <string>
#include <thread>
#include <sys/time.h>
#include <vector>
#include "ActionServo.h"
#include "AX12Manager.h"
#include "ClientUDP.h"
#include "FichierAction.h"
#include "LanceurManager.h"
#include "ServoManager.h"
#include "StepperManager.h"


class ActionManager
{

public:
	ActionManager(int i2c_Servos, int i2c_Lanceur, int nbAX12, ClientUDP& udp);
	void close();
	void action(std::string filname);

private:
	void sleepMillis(int millis);
	ClientUDP &client;
	ServoManager servos;
	StepperManager stepper;
	AX12Manager AX12;
};

#endif // ACTIONMANAGER_H_INCLUDED