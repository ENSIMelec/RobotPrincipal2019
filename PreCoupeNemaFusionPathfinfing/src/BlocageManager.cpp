#include "BlocageManager.h"

using namespace std;

BlocageManager::BlocageManager(Lidar *lid)
{
	pinMode(static_cast<int>(BlocagePin::AVANT_DROIT), INPUT);
	pinMode(static_cast<int>(BlocagePin::AVANT_GAUCHE), INPUT);
	pinMode(static_cast<int>(BlocagePin::ARRIERE_DROIT), INPUT);
	pinMode(static_cast<int>(BlocagePin::ARRIERE_GAUCHE), INPUT);

	this->lid = lid;
}

PositionBlocage BlocageManager::isBlocked() const
{
	// Mise en place des flags
	int avant_droit = digitalRead(static_cast<int>(BlocagePin::AVANT_DROIT));
	int avant_gauche = digitalRead(static_cast<int>(BlocagePin::AVANT_GAUCHE));
	int arriere_droit = digitalRead(static_cast<int>(BlocagePin::ARRIERE_DROIT));
	int arriere_gauche = digitalRead(static_cast<int>(BlocagePin::ARRIERE_GAUCHE));

	int avant;
	int arriere;

	if(lid->detectionAvant){
		avant = lid->speed;
	}
	else{
		avant = 1;
	}

	if(lid->detectionArriere){
		arriere = lid->speed;
	}
	else{
		arriere = 1;
	}


	

	//cout<<"Blocage :"<<avant_gauche<<" "<<avant_droit<<" "<<arriere_gauche<<" "<<arriere_droit<<endl;


	/*cout << "capteur avant droit " << digitalRead(static_cast<int>(BlocagePin::AVANT_DROIT)) << endl;
	cout << "capteur avant gauche " << digitalRead(static_cast<int>(BlocagePin::AVANT_GAUCHE)) << endl;
	cout << "capteur arriere droit " << digitalRead(static_cast<int>(BlocagePin::ARRIERE_DROIT)) << endl;
	cout << "capteur arriere gauche " << digitalRead(static_cast<int>(BlocagePin::ARRIERE_GAUCHE)) << endl;*/

	//if (!avant && !arriere)
		//return PositionBlocage::BOTH;

	if (!avant)
		return PositionBlocage::AVANT;

	else if (!arriere)
		return PositionBlocage::ARRIERE;
	
	return PositionBlocage::AUCUN;
}
