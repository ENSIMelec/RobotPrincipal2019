#ifndef SERVOMANAGER_H_INCLUDED
#define SERVOMANAGER_H_INCLUDED

#include <chrono>
#include <iostream>
#include <stdint.h>
#include <thread>
#include <unistd.h>
#include <wiringPi.h>


class ServoManager
{

public:
	 
	ServoManager(int i2c_servo);
	void servoAction(int servo, int action );
	
protected :

	int i2c_servo;
};

#endif //SERVOMANAGER_H_INCLUDED