#include "objectif.h"
#include <iostream>
#include <vector>

using namespace std;

Objectif::Objectif()
{

}

Objectif::Objectif(int id)
{
    idObjectif = id;
    done = false;
    bloque = false;
    action = "null";
    angleObj = 500;
	recalage = false;
}

Objectif::Objectif(int id, string act)
{
    idObjectif = id;
    done = false;
    bloque = false;
    action = act;
    angleObj = 500;
	recalage = false;

}

Objectif::Objectif(int id, string act, int ang)
{
    idObjectif = id;
    done = false;
    bloque = false;
    action = act;
    angleObj = ang;
	recalage = false;

}

Objectif::Objectif(int id, string act, int ang, bool recal)
{
    idObjectif = id;
    done = false;
    bloque = false;
    action = act;
    angleObj = ang;
	recalage = recal;
}

void Objectif::setObjectifDone()
{
    done = true;
}

int Objectif::getIdObjectif()
{
    return idObjectif;
}

bool Objectif::getDone()
{
    return done;
}

bool Objectif::getObjectifBloque()
{
    return bloque;
}

void Objectif::setObjectifBloque()
{
    bloque = true;
}

void Objectif::setObjectifDebloque()
{
    bloque = false;
}

vector<Noeud*> Objectif::getItineraire()
{
    return itineraire;
}

void Objectif::setItineraire(vector<Noeud*> chemin)
{
    itineraire = chemin;
}

void Objectif::setDone(bool newValue)
{
    done = newValue;
}

string Objectif::getActionToDo(){
    return action;
}

int Objectif::getAngleObj(){
    return angleObj;
}

void Objectif::setAngleObj(int angleArrivee){
    angleObj = angleArrivee;
}

bool Objectif::getRecalage(){
    return recalage;
}

void Objectif::setRecalage(bool recal){
    recalage = recal;
}

