#ifndef GRAPHE_H
#define GRAPHE_H

#include <vector>
#include "noeud.h"
#include "coordonnee.h"
#include "map.h"

using namespace std;

class Graphe
{

    public:
        Map graph;

        Graphe();
        Graphe(int);
        void DijkstraAlgo(int , vector<double> &, vector<int> &, const double);
        static list<int> DijkstraChemin(int , vector<int> &);
        list<int> FaireAlgoComplet(int, int, vector<double> &, vector<int> &, const double);
        static bool cheminExistant(vector<Noeud*>);
        Map getMap();
};

#endif // GRAPHE_H
