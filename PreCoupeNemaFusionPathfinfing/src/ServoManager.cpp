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
		//pwmWrite(PIN_BASE + 2, angleToTicks(15));	//servo n°2 à 15°
		//pwmWrite(PIN_BASE + 3, angleToTicks(135));	//servo n°3 à 135°

		return 0;
	}
}

void ServoManager::servoAction(int servo, int angle) {
	//Vérification des paramètres
	if((servo >= 0)&&(servo <= 15)) {
		if((angle >= 0)&&(angle <= 180)) {
			//Envoi au shield servo
			pwmWrite(PIN_BASE + servo, angleToTicks(angle));
			//Aquittement
			int readAngle = servoRead(servo);
			while((readAngle < angle-1)||(readAngle > angle+1)) {
				cout << "Echec d'envoi de la commande au servo " << servo << endl;
				cout << "Réenvoi..." << endl;
				pwmWrite(PIN_BASE + servo, angleToTicks(angle));
				readAngle = servoRead(servo);
				usleep(3000*1000);
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