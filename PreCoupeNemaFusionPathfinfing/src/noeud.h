
#ifndef NOEUD_H
#define NOEUD_H

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

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>

#include <stdio.h>      /* printf */
#include <math.h>
#include "coordonnee.h"

using namespace std;

class Noeud
{
public:
    bool etat;
    vector<Noeud*> listeVoisins;
    vector<double> weight;
    Coordonnee* coordonnees;
    int id;

    int getX();
    int getY();
    int getId();
    bool getEtat();

    Noeud();
    Noeud(bool, Coordonnee*, int);
    Noeud(int, int);
    ~Noeud();

    double CalculerWeight(Noeud*);
    void AddNoeudVoisin(Noeud*);
    Noeud getNoeudById(int);
};

#endif // NOEUD_H
