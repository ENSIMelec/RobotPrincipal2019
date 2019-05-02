#include "Point.h"

using namespace std;

Point::Point(){
	m_type = TypePoint::POSITION;
	m_type_S = "Position";
	m_x = 0;
	m_y = 0;
	m_angle = 0;
	m_deltaDeplacement = 0;
	m_deltaAngle = 0.0;
	m_vitesse = 0;
	m_sens = "avant";
	m_blocage = "aucun";
	m_coefCourbe = 0.0;
	m_lissage = false;
	m_derapage = false;
	m_timeOut = 0;
	m_action = "null";
	m_attAction = "false";
}

Point::Point(string type, int x, int y, int angle, int deltaDeplacement, double deltaAngle, int vitesse, string sens, string blocage, double coefCourbe, bool lissage, bool derapage, int timeOut, string action, bool attAction) : m_type_S(type), m_x(x), m_y(y), m_angle(angle), m_deltaDeplacement(deltaDeplacement), m_deltaAngle(deltaAngle), m_vitesse(vitesse), m_sens(sens), m_blocage(blocage), m_coefCourbe(coefCourbe), m_lissage(lissage), m_derapage(derapage), m_timeOut(timeOut), m_action(action), m_attAction(attAction)
{
	if(m_type_S.compare("Angle") == 0) {
		m_type = TypePoint::ANGLE;
	} else if(m_type_S.compare("Deplacement") == 0) {
		m_type = TypePoint::DEPLACEMENT;
	} else if(m_type_S.compare("DeplacementX") == 0) {
		m_type = TypePoint::DEPLACEMENT_X;
	} else if(m_type_S.compare("DeplacementY") == 0) {
		m_type = TypePoint::DEPLACEMENT_Y;
	} else if(m_type_S.compare("Position") == 0) {
		m_type = TypePoint::POSITION;
	} else if(m_type_S.compare("RecalageX") == 0) {
		m_type = TypePoint::RECALAGE_X;
	} else if(m_type_S.compare("RecalageY") == 0) {
		m_type = TypePoint::RECALAGE_Y;
	} else if(m_type_S.compare("RecalageXY") == 0) {
		m_type = TypePoint::RECALAGE_XY;
	} else if(m_type_S.compare("DeplacementRelatif") == 0) {
		m_type = TypePoint::DEPLA_RELATIF;
	} else if(m_type_S.compare("AngleRelatif") == 0) {
		m_type = TypePoint::ANGLE_RELATIF;
	}
}

void Point::display()
{
	cout << m_type_S << " : (" << m_x << ";" << m_y << ") +/-" << m_deltaDeplacement << " | " << m_angle << "° +/-" << m_deltaAngle << "° | " << m_vitesse << " mm/sec | marche " << m_sens << " | blocage : " << m_blocage << endl << "Courbe : " << m_coefCourbe << " | lissage : " << m_lissage << " | derapage : " << m_derapage << " | timeOut : " << m_timeOut << " ms | Action : " << m_action << " | attFinAction : " << m_attAction << endl;
}

TypePoint Point::getType() {return m_type;}
string Point::getType_S() {return m_type_S;}
int Point::getX() {return m_x;}
int Point::getY() {return m_y;}
int Point::getAngle() {return m_angle;}
int Point::getDeltaDeplacement() {return m_deltaDeplacement;}
double Point::getDeltaAngle() {return m_deltaAngle;}
int Point::getVitesse() {return m_vitesse;}
int Point::getSens() {
	if(m_sens.compare("avant") == 0)
		return MARCHE_AVANT;
	else if(m_sens.compare("arriere") == 0)
		return MARCHE_ARRIERE;
	else {
		cout << "ERREUR Point::getSens()" << endl;
		return -2;
	}
}
int Point::getBlocage() {
	if(m_blocage.compare("aucun") == 0) return CAPT_NONE;
	else if(m_blocage.compare("avant") == 0) return CAPT_AVANT;
	else if(m_blocage.compare("arriere") == 0) return CAPT_ARRIERE;
	else if(m_blocage.compare("avantG") == 0) return CAPT_AVANT_G;
	else if(m_blocage.compare("avantD") == 0) return CAPT_AVANT_D;
	else if(m_blocage.compare("arriereG") == 0) return CAPT_ARRIERE_G;
	else if(m_blocage.compare("arriereD") == 0) return CAPT_ARRIERE_D;
	else return CAPT_ALL;
}
double Point::getCoefCourbe() {return m_coefCourbe;}
bool Point::getLissage() {return m_lissage;}
bool Point::getDerapage() {return m_derapage;}
int Point::getTimeOut() {return m_timeOut;}
string Point::getAction() {return m_action;}
bool Point::getAttAction() {return m_attAction;}

void Point::setSens(int sens) {
	if(sens == MARCHE_AVANT)
		m_sens = "avant";
	else if(sens == MARCHE_ARRIERE)
		m_sens = "arriere";
	else
		m_sens = "";
	return;
}

void Point::setVitesse(int vitesse){
	m_vitesse = vitesse;
}
