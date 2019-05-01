#ifndef MOTEURMANAGER_H_INCLUDED
#define MOTEURMANAGER_H_INCLUDED

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <unistd.h>

class MoteurManager
{

public:
	MoteurManager(int i2c);

	void setConsigne(int PWMGauche, int PWMDroite);
	void setConsigneGauche(int PWMGauche);
	void setConsigneDroite(int PWMDroite);

	void avancer();
	void reculer();

	void stop();

	uint8_t getOrdreGauche() const;
	uint8_t getOrdreDroite() const;
	uint8_t getSensGauche() const;
	uint8_t getSensDroite() const;

	bool dummy = false;
	bool dummyBlocage = false;

private:
	void controleConsigne(int *PWM);
	void apply();
	
	uint8_t ordreGauche;
	uint8_t ordreDroite;
	uint8_t sensGauche;
	uint8_t sensDroite;

	int i2c_fd;
};

#endif //MOTEURMANAGER_H_INCLUDED