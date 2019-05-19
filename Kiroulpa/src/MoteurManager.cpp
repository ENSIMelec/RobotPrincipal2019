#include "MoteurManager.h"

using namespace std;

// Constructeur par défaut : on stoppe les moteurs
MoteurManager::MoteurManager(int i2c) :
	ordreGauche(0),
	ordreDroite(0),
	sensGauche(0),
	sensDroite(0),
	i2c_fd(i2c)
{
	apply();
}

/*
L'arduino va recevoir des valeurs entre 0 et 255 pour la commande de PWM (vitesse)
Une fois à la suite pour chaque roue
Puis elle reçoit également le sens (1 = marche avant // 2 = marche arrière // 0 = arrêt des moteurs)
Elle va alors changer IN1/IN2 en fonction du sens, et ENA en fonction de la commande
*/

// Changement de consigne sur les moteurs
void MoteurManager::setConsigne(int PWMGauche, int PWMDroite) {
	controleConsigne(&PWMGauche);
	controleConsigne(&PWMDroite);
	ordreGauche = (uint8_t) abs(PWMGauche);
	ordreDroite = (uint8_t) abs(PWMDroite);
	sensGauche 	= (uint8_t) (PWMGauche < 0 ? 2 : (PWMGauche > 0 ? 1 : 0));
	sensDroite 	= (uint8_t) (PWMDroite < 0 ? 2 : (PWMDroite > 0 ? 1 : 0));
	apply();
	return;
}

void MoteurManager::setConsigneGauche(int PWMGauche) {
	controleConsigne(&PWMGauche);
	ordreGauche = (uint8_t) abs(PWMGauche);
	sensGauche 	= (uint8_t) (PWMGauche < 0 ? 2 : (PWMGauche > 0 ? 1 : 0));
	apply();
	return;
}
void MoteurManager::setConsigneDroite(int PWMDroite) {
	controleConsigne(&PWMDroite);
	ordreDroite = (uint8_t) abs(PWMDroite);
	sensDroite 	= (uint8_t) (PWMDroite < 0 ? 2 : (PWMDroite > 0 ? 1 : 0));
	apply();
	return;
}

void MoteurManager::avancer() { //Fait avancer le robot
	setConsigne(100, 100);
	return;
}

void MoteurManager::reculer() { //Fait reculer le robot
	setConsigne(-100, -100);
	return;
}

void MoteurManager::controleConsigne(int *PWM) {
	if(*PWM < -255)
		*PWM = -255;
	else if(*PWM > 255)
		*PWM = 255;
	return;
}

void MoteurManager::stop()
{
	ordreGauche = 0;
	ordreDroite = 0;
	sensGauche = 0;
	sensDroite = 0;
	apply();
}

// Envoi effectif en I2C des commandes
void MoteurManager::apply()
{
	//0if(debug){
		//std::cerr << "manager::apply " << (int) ordreGauche << " " << (int) ordreDroite << std::endl;
		//}

	// Les trois octets que l’on va envoyer en I2C
	uint8_t data[4];

	data[0] = ordreGauche;
	data[1] = ordreDroite;
	data[2] = sensGauche;
	data[3] = sensDroite;

	// Envoi du tableau de deux octets
	if(dummy == false && !dummyBlocage)
		write(i2c_fd, data, 4);
}

// Getters
uint8_t MoteurManager::getOrdreGauche() const
{
	return ordreGauche;
}
uint8_t MoteurManager::getOrdreDroite() const
{
	return ordreDroite;
}

uint8_t MoteurManager::getSensGauche() const
{// 1 : marche avant // 2 : marche arrière // 0 : arrêt
	return sensGauche;
}
uint8_t MoteurManager::getSensDroite() const
{// 1 : marche avant // 2 : marche arrière // 0 : arrêt
	return sensDroite;
}
