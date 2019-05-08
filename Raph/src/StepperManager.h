#ifndef STEPPERMANAGER_H_INCLUDED
#define STEPPERMANAGER_H_INCLUDED

/*!
 * \file StepperManager.h
 * \brief utilisation d'un stepper en i2c
 * \author ENSIM'Elec
 * \version 0.1
 * \date 7 mars 2019
 */

#include <chrono>
#include <iostream>
#include <stdint.h>
#include <thread>
#include <unistd.h>
#include <wiringPi.h>



 /*! \class StepperManager
   * \brief Classe d'utilisation d'un moteur pas a pas 
   *
   *  Cette classe permet de d'initialiser et de controller
   *  un moteur pas a pas via un driver en i2c
   *
   */

class StepperManager
{

public:
	/*!
	*  \brief Constructeur
	*
	*  Constructeur de la classe StepperManager,
	*  Initialise la position du lidar

	*/
	StepperManager(int i2c_stepper);

	/*!
	*  \brief Modifie la position du moteur 
	*
	*  Envoie les données en i2c pour que le moteur aille à la position voulue
	*\param pos entier 32 bit de  −2,147,483,648 to +2,147,483,647 micro steps

	*/
	void setPosition(int pos);


	void setSpeed(int speed);

	/*!
	*  \brief Sort du mode safe
	*
	*  A l'allumage le moteur est verouille il faut le debloquer avec cette fonction
	

	*/


	void exitSafeStart();

	/*!
	*  \brief Initialise l'origine du moteur
	*
	*  Lance le mmoteur jusqu'a ce qu'il touche un endstop de la direction choisie


	*/

	void goHome();

	/*!
	*  \brief Ne pas utilisez
	*
	*  

	*/
	void haltAndSet(int setPos);




protected:

	int i2c_stepper;/*!< Variable d'adresse i2c de la carte moteur pas a pas*/
	int currentPos;
	int notMoving;
};

#endif //SERVOMANAGER_H_INCLUDED