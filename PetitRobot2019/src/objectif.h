#ifndef OBJECTIF_H
#define OBJECTIF_H

#include <list>
#include "noeud.h"
using namespace std;

class Objectif
{
public:
    int getIdObjectif();
    int angleObj;
    bool getDone();
    Objectif();
    Objectif(int);
    Objectif(int, string);
    Objectif(int, string, int);
	Objectif(int, string, int, bool);
    void setObjectifDone();
    void setObjectifBloque();
    void setObjectifDebloque();
    bool getObjectifBloque();
    //void setBloque(bool);
    void setItineraire(vector<Noeud*>);
    vector<Noeud*> getItineraire();
    void setDone(bool);
    string getActionToDo();
    int getAngleObj();
    void setAngleObj(int);
	bool getRecalage();
	void setRecalage(bool);

    bool done; //false s'il n'est pas fait, true s'il est fait
private:
    int idObjectif;
    string action;
    bool bloque; //accessible ou non
    vector<Noeud*> itineraire;
	bool recalage;
    
};

#endif // OBJECTIF_H
