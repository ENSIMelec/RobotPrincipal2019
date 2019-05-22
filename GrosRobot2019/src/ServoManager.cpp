#include "ServoManager.h"

using namespace std;

// Constructeur
ServoManager::ServoManager(int i2c): i2c_servo(i2c), frequence(50) {

}

int ServoManager::servoInitialisation() {
	if(pca9685Setup(PIN_BASE, i2c_servo, frequence) != 0)
	{
		cout << "Erreur initialisation du shield servo" << endl;
		return -1;
	}
	else
	{
		pca9685PWMReset(i2c_servo); // Init des servos à 0°

		/*ICI pour initialiser les servomoteurs à une certaine position*/
		pwmWrite(PIN_BASE + 14, angleToTicks(0));	//Droite
		pwmWrite(PIN_BASE + 15, angleToTicks(150));	//Gauche

		return 0;
	}
}

void ServoManager::servoAction(int servo, int angle) {
	//Vérification des paramètres
	if((servo >= 0)&&(servo <= 15)) {
		if((angle >= 0)&&(angle <= 180)) {

			//Envoi au shield servo
			pwmWrite(PIN_BASE + servo, angleToTicks(angle));

			//Acquitement
			int readAngle = servoRead(servo);
			int cpt = 0;
			//renvoi 3 commandes au maximum au servo espacées d'une seconde, sinon il continue
			while (((readAngle < angle-1)||(readAngle > angle+1)) && (cpt++<30)) {
				cout << "Echec d'envoi de la commande au servo " << servo << endl; 
				cout << "Renvoi..." << endl;
				pwmWrite(PIN_BASE + servo, angleToTicks(angle));
				readAngle = servoRead(servo);
				usleep(100000);
			}
		}
		else {
			cout << "Angle servo incorrect (0° à 180°)" << endl;
		}
	}
	else {
		cout << "Adresse servo incorrect (0 à 15)" << endl;
	}
}

int ServoManager::servoRead(int servo) {
	//Vérification des paramètres
	if((servo >= 0)&&(servo <= 15)) {
		int ticks = digitalRead(PIN_BASE + servo);
		return ticksToAngle(ticks);
	}
	else {
		cout << "Adresse servo incorrect (0 à 15)" << endl;
		return -1;
	}
}

/**
 * Converti un angle en ticks : 0° < angle < 180°.
 * 0.5ms --> 0°
 *   3ms --> 180°
 * La conversion ms en tick est tiré de la librairie pca9685
 */
int ServoManager::angleToTicks(int angle) {
	float impulseMs = (2.5/180)*angle+0.5;
	float cycleMs = 1000.0f / frequence;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

int ServoManager::ticksToAngle(int ticks) {
	float cycleMs = 1000.0f / frequence;
	return (int)((((ticks-0.5)*cycleMs/4096)-0.5)*180/2.5);
}