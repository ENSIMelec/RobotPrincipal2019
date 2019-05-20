#ifndef MAP_H
#define MAP_H

#include <vector>
#include "noeud.h"

using namespace std;

class Map
{
public:
    vector<Noeud*> mapping;
    int cote;
    int taille;

    Map();
    Map(int);   //soit 1,
                //soit -1
                //pour déterminer le côté de la table
    void BloquerNoeud(int);
    void DebloquerNoeud(int);
    int getX(int);
    int getY(int);
    Noeud* getNoeud(int);
    vector<Noeud*> getMapping();
    void debloquerTousLesNoeuds();
    void lierNoeuds();
    Noeud* noeudLePlusProche(Noeud*);

};

#endif // MAP_H
