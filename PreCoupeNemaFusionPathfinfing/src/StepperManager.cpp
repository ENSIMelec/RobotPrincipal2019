#include "StepperManager.h"

using namespace std;



StepperManager::StepperManager(int i2c) :
	i2c_stepper(i2c)
{
	notMoving = false;
	if (i2c_stepper != NULL)
	{
		this->exitSafeStart();
		this->goHome();
		
		usleep(4000000);
	}
	notMoving = true;
}	

void StepperManager::setSpeed(int speed)
{
	if (speed > 0)
	{
		while (!notMoving);
		notMoving = false;
		uint8_t command[] = {
		0xE6,
		(uint8_t)(speed >> 0 & 0xFF),
		(uint8_t)(speed >> 8 & 0xFF),
		(uint8_t)(speed >> 16 & 0xFF),
		(uint8_t)(speed >> 24 & 0xFF),
		};

		write(i2c_stepper, command, sizeof(command));
		usleep(5000);
		notMoving = true;
	}
}


void StepperManager::setPosition(int pos)
{

	pos = pos * 100;
	this->haltAndSet(currentPos);
	if (pos < 0)
	{
		cout << "Commande impossible" << endl;
	}
	else
	{
		while (!notMoving);
		notMoving = false;
		uint8_t command[] = {
		0xE0,
		(uint8_t)(pos >> 0 & 0xFF),
		(uint8_t)(pos >> 8 & 0xFF),
		(uint8_t)(pos >> 16 & 0xFF),
		(uint8_t)(pos >> 24 & 0xFF),
		};

		write(i2c_stepper, command, sizeof(command));
		currentPos = pos;
		usleep(50000);
		notMoving = true;
	}

}


void StepperManager::exitSafeStart()
{
	uint8_t command[] = { 0x83 };
	write(i2c_stepper, command, sizeof(command));
	currentPos = 0;
}

void StepperManager::goHome()
{
	uint8_t command[] = {
	0x97,
	0x00
	};
	
	write(i2c_stepper, command, sizeof(command));

}

void StepperManager::haltAndSet(int setPos)
{
	


	uint8_t command[] = {
	0xEC,
	(uint8_t)(setPos >> 0 & 0xFF),
	(uint8_t)(setPos >> 8 & 0xFF),
	(uint8_t)(setPos >> 16 & 0xFF),
	(uint8_t)(setPos >> 24 & 0xFF),
	};


	write(i2c_stepper, command, sizeof(command));

}