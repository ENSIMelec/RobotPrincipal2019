#ifndef SERVOMANAGER_H_INCLUDED
#define SERVOMANAGER_H_INCLUDED

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>

// Librairie trouvée sur internet pour communiquer avec le
// shield servomoteur. Elle est en c et n'est pas forcément
// bien faîte mais elle fonctionne.
#include "pca9685.h"

class ServoManager
{

public:

	/*! 
	 *  \brief Constructeur
	 * 
	 *  Constructeur de la classe ServoManager.
	 *  
	 *  \param i2c_servo : adresse i2c du shield servo. Par défaut, 0x40.
	 */
	ServoManager(int i2c_servo);

	/*! 
	 *  \brief Initialisation du shield servomoteurs
	 * 
	 *  Méthode qui permet d'initialiser le shield servomoteurs.
	 *  
	 *  \param angle : angle en degrée [0-180].
	 *  \return -1 si l'initialisation à échoué, sinon 0.
	 */
	int servoInitialisation();

	/*! 
	 *  \brief Commande du servomoteur
	 * 
	 *  Méthode qui permet d'appliquer un angle à un servomoteur.
	 *  
	 *  \param servo : id du servomoteur du shield.
	 *  \param angle : angle en degrée [0-180].
	 */
	void servoAction(int servo, int angle);

	/*! 
	 *  \brief Lecture de l'angle
	 * 
	 *  Méthode qui permet de lire l'angle actuel à un servomoteur
	 *  
	 *  \param servo : id du servomoteur du shield.
	 *  \return l'angle du servomoteur.
	 */
	int servoRead(int servo);
	
protected :

	int i2c_servo;	/*!< Adresse i2c du shield servomoteurs*/
	int frequence;	/*!< Fréquence de la pwm*/

	/*! 
	 *  \brief Conversion angle en ticks
	 * 
	 *  Méthode qui permet de convertir un angle en tick conformément à ce que veut la libraire pca9685.h.
	 *  
	 *  \param angle : angle en degrée [0-180].
	 *  \return le nombre de ticks à écrire dans le registre du shield servos
	 */
	int angleToTicks(int angle);

	/*! 
	 *  \brief Conversion d'un nombre de ticks en angle
	 * 
	 *  Méthode qui permet de convertir un nombre de ticks en angle.
	 *  
	 *  \param ticks : nombre de ticks [0-4095] (12bits).
	 *  \return le nombre de ticks à écrire dans le registre du shield servos
	 */
	int ticksToAngle(int ticks);
};

#endif //SERVOMANAGER_H_INCLUDED