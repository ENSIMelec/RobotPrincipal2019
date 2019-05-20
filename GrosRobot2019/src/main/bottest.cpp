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
#include "ServoManager.h"


#define CODEUR_GAUCHE 0
#define CODEUR_DROIT 1



void StopSignalBrutal(int);

MoteurManager *p_moteurs;

ServoManager *p_servo;

int askedSpeed(int c);
bool askedMove(int c, int vitesse);
void printCommands();
int* askedCodeurs(int c);
int w = 0;

Config config;


ofstream file;

uint8_t datas[2];
uint8_t n = 30;
uint8_t b = 0;


int drone ()
{
	config.loadFromFile("config.info");

	int I2C_MOTEURS = 8;
	int I2C_SERVO = 6;


	//file.open("testBottest.point");
	int i2c = wiringPiI2CSetup(I2C_MOTEURS);
	int i2cS = wiringPiI2CSetup(I2C_SERVO);

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


	// Création du MoteurManager qui s’occupe de simuler les deux moteurs
	MoteurManager moteurs(i2c);
	ServoManager servo(i2cS);
	p_moteurs = &moteurs;
	p_servo = &servo;

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
	if (i2c < 0)
	{
		perror("wiringPiI2CSetup");
		pthread_exit(NULL);
	}


	int cmd;
	int lastMove = 'a';
	int vitesse = 50;
	int tmp;

	printCommands();

	system ("/bin/stty raw");

	while((cmd=getchar())!= '.')
	{

		putchar(cmd);

		cout << endl;

		system ("/bin/stty cooked");


		if(askedMove(cmd, vitesse))
			lastMove = cmd;
		else if((tmp = askedSpeed(cmd)) != -1)
		{
			vitesse = tmp;
			askedMove(lastMove, vitesse);
		}


		moteurs.dummy = true;

		moteurs.dummy = false;


		system ("/bin/stty raw");

	}
	file.close();

	system ("/bin/stty cooked");


	sleep(1);
	moteurs.stop();


	return 0;
}

void  StopSignalBrutal(int sig)
{	
	if(sig)
		cout << "CTRL+C détecté" << endl;
	p_moteurs->stop();
	exit(0);
}


bool askedMove(int c, int vitesse)
{
	int nS = 0;
	int aS = 0;
	switch(c)
	{
		case 'z': //haut
			p_moteurs->setConsigne(vitesse, vitesse);
		break;
		case 's': //bas
			p_moteurs->setConsigne(-vitesse, -vitesse);
		break;
		case 'q'://gauche
			p_moteurs->setConsigne(-vitesse, vitesse);
		break;
		case 'd': //droite
			p_moteurs->setConsigne(vitesse, -vitesse);
		break;

		case 'o'://roue gauche avant
			p_moteurs->setConsigne(vitesse, 0);
		break;
		case 'p': //roue droite avant
			p_moteurs->setConsigne(0, vitesse);
		break;
		case 'l'://roue gauche arriere
			p_moteurs->setConsigne(-vitesse, 0);
		break;
		case 'm': //roue droite arriere
			p_moteurs->setConsigne(0, -vitesse);
		break;

		case 'a': //stop
			p_moteurs->setConsigne(0, 0);
		break;
		case 'i': //servoAction
			cout << " numero_servo angle_servo (respecter l'espace svp entre les deux données)" << endl;
			scanf("%i %i", &nS,&aS);
			p_servo->servoAction(nS,aS);
		break;
		default:
			return false;
	}
	return true;
}


void printCommands()
{
	printf("\n");
	printf("Z Q S D : déplacement\n");
	printf("A : Arrêt du robot\n");
	printf("E : Récupérer les valeurs des codeurs (fausse la génération de points !)\n");
	printf("0-9-* : Réglage de la vitesse du robot de 0 à 100\n");
	printf("R : Ajoute un point de rotation\n");
	printf("F : Ajoute un point de déplacement\n");
	printf("G : Supprime le dernier point de la liste\n");
	printf("T : Affiche la liste des points\n");
	printf(" . puis CTRL-C : Quitter\n");
}

int askedSpeed(int c)
{
	switch(c)
	{
		case '+':
			printf("Vitesse argmente : %i\n", w+5);
			return w = w+5;
		case '-':
			printf("Vitesse diminue : %i\n",w-5);
			return w = w-5;
		case '0':
			return 0;
		case '1':
			return (int)(255*10/100);
		case '2':
			return (int)(255*20/100);
		case '3':
			return (int)(255*30/100);
		case '4':
			return (int)(255*40/100);
		case '5':
			return (int)(255*50/100);
		case '6':
			return (int)(255*60/100);
		case '7':
			return (int)(255*70/100);
		case '8':
			return (int)(255*80/100);
		case '9':
			return (int)(255*90/100);
		case '*':
			return 255;
	}
	return -1;
}


int main(void)
{
	drone();

	return 0;

}
