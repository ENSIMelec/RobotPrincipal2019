#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <iostream>
#include <string>

#define MARCHE_AVANT 0
#define MARCHE_ARRIERE 1

#define CAPT_NONE 0
#define CAPT_AVANT 1
#define CAPT_ARRIERE 2
#define CAPT_ALL 3
#define CAPT_AVANT_G 11
#define CAPT_AVANT_D 12
#define CAPT_ARRIERE_G 21
#define CAPT_ARRIERE_D 22

enum class TypePoint {
	ANGLE = 0,
	DEPLACEMENT = 1,
	DEPLACEMENT_X = 2,
	DEPLACEMENT_Y = 3,
	POSITION = 4,
	RECALAGE_X = 5,
	RECALAGE_Y = 6,
	RECALAGE_XY = 7,
	DEPLA_RELATIF = 8,
	ANGLE_RELATIF = 9
};

class Point
{
	public :
	Point();
	Point(std::string type, int x, int y, int angle, int deltaDeplacement, double deltaAngle, int vitesse, std::string sens, std::string blocage, double coefCourbe, bool lissage, bool derapage, int timeOut, std::string action, bool attAction);

	TypePoint getType();
	std::string getType_S();
	int getX();
	int getY();
	int getAngle();
	int getDeltaDeplacement();
	double getDeltaAngle();
	int getVitesse();
	int getSens();
	int getBlocage();
	double getCoefCourbe();
	bool getLissage();
	bool getDerapage();
	int getTimeOut();
	std::string getAction();
	bool getAttAction();

	void setSens(int sens);
	void setVitesse(int vitesse);

	void display();

	private :
	TypePoint m_type;
	std::string m_type_S;
	int m_x;
	int m_y;
	int m_angle;
	int m_deltaDeplacement;
	double m_deltaAngle;
	int m_vitesse;
	std::string m_sens;
	std::string m_blocage;
	double m_coefCourbe;
	bool m_lissage;
	bool m_derapage;
	int m_timeOut;
	std::string m_action;
	bool m_attAction;
};

#endif //POINT_H_INCLUDED
