#ifndef INITROBOT_H_INCLUDED
#define INITROBOT_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include "Clock.h"

#define PIN_JACK	17
#define PIN_ARU		22
#define PIN_LED		26

using namespace std;

class InitRobot
{

public:
	InitRobot();
	InitRobot(bool* p_elec, bool* p_exp, bool* p_rob);
	void waitForRobotInitialisation(bool checkConnectedDevices);
	static int aruIsNotPush();
	static int jackIsPresent();
	int electronIsConnected();
	int experienceIsConnected();
	int kiroulpaIsConnected();

private:
	bool* p_electronConfirm;
	bool* p_experienceConfirm;
	bool* p_kiroulpaConfirm;
	timer temps;

	void ledInitOn();
	void ledInitOff();
	void sleep_ms(int delay);
};

#endif //INITROBOT_H_INCLUDED