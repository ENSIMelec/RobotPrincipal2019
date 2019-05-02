#ifndef SERIALCODEURMANAGER_H_INCLUDED
#define SERIALCODEURMANAGER_H_INCLUDED


#include <cmath>
#include <chrono>
#include <cstdint>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "ICodeurManager.h"

#define CMD_RESET -3

class SerialCodeurManager : public ICodeurManager
{
public:

	SerialCodeurManager(int init);

	virtual void Initialisation();
	virtual void Closes();

	virtual void readAndReset() override;
	virtual void reset() override;

private:
	int initCodeur;

};

#endif //SERIALCODEURMANAGER_H_INCLUDED