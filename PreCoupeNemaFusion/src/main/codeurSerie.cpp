#include <iostream>
#include <thread>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <wiringPiI2C.h>

using namespace std;

#include "Config.h"
#include "MoteurManager.h"
#include "SerialCodeurManager.h"
#include "ServoManager.h"

#define CODEUR_GAUCHE 0
#define CODEUR_DROIT 1

int cmd ;


void StopSignalBrutal(int);

MoteurManager *p_moteurs;
//I2CCodeurManager *p_codeurs;
SerialCodeurManager *p_codeurs;
ServoManager *p_servo;

int askedSpeed(int c);
bool askedMove(int c, int vitesse);
void printCommands();
int* askedCodeurs(int c);

Config config;

ofstream file;
bool run = true;
void *theard_1 (void *arg);


uint8_t datas[2];
uint8_t n = 30;
uint8_t b = 0;

void *theard_1 (void *arg){
	int c;
	if(arg == NULL)
		c = 0;
	while(run)
	{
		sleep(1);
		c = getchar();
		if(c == 'a'){
			run = false;
			//p_codeurs->CloseS();
		}
	}
	pthread_exit(NULL);

}

void *theard_2 (void *arg)
{
	int I2C_MOTEURS = 8;
	if(arg == NULL)
		I2C_MOTEURS = 8;
	config.loadFromFile("config.info");

	//int I2C_DEVID = 8;
	I2C_MOTEURS = 8;
	//int I2C_CODEURS = 9;
	int I2C_SERVO = 6;


	int i2c = wiringPiI2CSetup(I2C_MOTEURS);
	int i2cS = wiringPiI2CSetup(I2C_SERVO);
	//int i2cc = wiringPiI2CSetup(I2C_CODEURS);

	if (i2c < 0)
	{
		perror("wiringPiI2CSetup");
		pthread_exit(NULL);
	}
	if (i2cS < 0)
	{
		perror("wiringPiI2CSetup");
		pthread_exit(NULL);
	}
	/*if (i2cc < 0)
	{
		perror("wiringPiI2CSetup");
		pthread_exit(NULL);
	}*/

	// Cr�ation du MoteurManager qui s�occupe de simuler les deux moteurs
	MoteurManager moteurs(i2c);
	ServoManager servo(i2cS);
	p_moteurs = &moteurs;
	p_servo = &servo;


	//i2cc = wiringPiI2CSetup(I2C_CODEURS);
	i2c =  wiringPiI2CSetup(I2C_MOTEURS);
	i2cS = wiringPiI2CSetup(I2C_SERVO);

	if (i2c < 0)
	{
		perror("wiringPiI2CSetup");
		pthread_exit(NULL);
	}

	if (i2cS < 0)
	{
		perror("wiringPiI2CSetup");
		pthread_exit(NULL);
	}
	/*if (i2cc < 0)
	{
		perror("wiringPiI2CSetup");
		pthread_exit(NULL);
	}*/

	// Cr�ation du groupement de deux codeurs
	SerialCodeurManager codeurs(0);
	p_codeurs = &codeurs;
	//reset du lancement pr�c�dent
	codeurs.reset();

	int dtick = 0;
	int gtick = 0;
	p_codeurs->Initialisation();

	system ("/bin/stty raw");
	while(run)
	{
			usleep(1000*10);
			int* res = (int*)malloc(sizeof(int)*2);
			p_codeurs->readAndReset();
			res[CODEUR_GAUCHE] = p_codeurs->getLeftTicks();
			res[CODEUR_DROIT] = p_codeurs->getRightTicks();
			gtick += p_codeurs->getLeftTicks();
			dtick += p_codeurs->getRightTicks();
			printf("G:%d/D:%d\n\r", res[CODEUR_GAUCHE], res[CODEUR_DROIT]);
			printf("nbTick gauche : %d, nbTick droit : %d \n",gtick,dtick);
			//usleep(1000*20);

	}
	file.close();
	p_codeurs->Closes();

	system ("/bin/stty cooked");


	sleep(2);
	moteurs.stop();

	return 0;
}

int main()
{
	pthread_t theard1;
	pthread_t theard2;

	pthread_create(&theard1, NULL, theard_1, (void*)NULL);
	pthread_create(&theard2, NULL, theard_2, (void*)NULL);

	pthread_join(theard2, NULL);
	pthread_join(theard1, NULL);

	return 0;
}
