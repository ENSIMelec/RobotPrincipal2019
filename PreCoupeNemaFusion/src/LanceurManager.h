#ifndef LANCEURMANAGER_H_INCLUDED
#define LANCEURMANAGER_H_INCLUDED

#include <chrono>
#include <iostream>
#include <stdint.h>
#include <thread>
#include <unistd.h>
#include <wiringPi.h>


class LanceurManager
{

public:
	 
	LanceurManager(int i2c_lanceur);
	void lanceurAction(int action, int tempsFrein); //Prend une valeur entre 0 et 1023 : correspond à une valeur de réference pour l'asservissement du moteur
	
protected :

	int i2c_lanceur;
};

#endif //LANCEURMANAGER_H_INCLUDED