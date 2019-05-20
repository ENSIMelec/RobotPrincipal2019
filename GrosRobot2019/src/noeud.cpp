#include "noeud.h"
#include "coordonnee.h"

Noeud::Noeud(){}

Noeud::~Noeud(){}

Noeud::Noeud(bool tt, Coordonnee* pt, int i)
{
    this->etat = tt;
    this->coordonnees = pt;
    this->id = i;
}

//Calcul la distance entre deux noeuds en fonction de leurs coordonnées
double Noeud::CalculerWeight(Noeud* a)
{
    return (double)sqrt(pow(this->coordonnees->x - a->coordonnees->x,2) + pow(this->coordonnees->y - a->coordonnees->y,2));
}

//Ajouter un noeud adjacent à un noeud
void Noeud::AddNoeudVoisin(Noeud* a)
{
    this->listeVoisins.push_back(a);                //Quand on ajoute un noeud,
    this->weight.push_back(CalculerWeight(a));      //on calcule immédiatement la distance qui les séparent pour qu'ils aient le même id dans les vecteurs
}

int Noeud::getX() {return this->coordonnees->x;}
int Noeud::getY() {return this->coordonnees->y;}

Noeud::Noeud(int x, int y)
{
	
}

int Noeud::getId(){
	return id;
}

bool Noeud::getEtat(){
	return etat;
}
