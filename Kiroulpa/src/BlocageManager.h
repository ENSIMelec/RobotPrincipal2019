#ifndef BLOCAGEMANAGER_H_INCLUDED
#define BLOCAGEMANAGER_H_INCLUDED

#include <wiringPi.h>
#include <iostream>
#include <lidar.h>


// Liste des pins
enum class BlocagePin {
	AVANT_DROIT = 26,
	AVANT_GAUCHE = 6,
	ARRIERE_DROIT = 19,
	ARRIERE_GAUCHE = 13
};

// Système de flags pour savoir à un instant T quels sont les positions du
// blocage à l’aide de && logique
enum class PositionBlocage {
	AUCUN = 0,
	AVANT = 1,
	ARRIEREGAUCHE = 2,
	ARRIEREDROIT = 3,
	BOTH = 4,
	ARRIERE = 5,
	AVANT_GAUCHE = 6,
	AVANT_DROIT = 7
};

// Opérateur permettant de faire un && entre deux positions de blocage pour
// savoir si un endroit est bloqué ou pas sans devoir faire les casts à chaque
// fois
inline bool operator&&(PositionBlocage a, PositionBlocage b)
{
	return static_cast<int>(a) && static_cast<int>(b);
}

class BlocageManager
{
public:
	BlocageManager(Lidar *lid);

	PositionBlocage isBlocked() const;
	Lidar *lid;

};

#endif //BLOCAGEMANAGER_H_INCLUDED