#include "graphe.h"
#include "noeud.h"
#include "map.h"

//Graphe::Graphe(){}
Graphe::Graphe(int monCote)
{
    this->graph = Map(monCote);
}

//Fonction qui fait l'algo de Dijkstra

 void Graphe::DijkstraAlgo(int source, vector<double> &min_distance, vector<int> &previous, const double max_weight)
{
    int n = this->graph.mapping.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    set<pair<double, int> > vertex_queue;
    vertex_queue.insert(make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        double dist = vertex_queue.begin()->first;
        int u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
    vector<Noeud*> neighbors = this->graph.mapping[u]->listeVoisins;
        for(Noeud* a : neighbors)
        {
            if(a->etat)
            {
                int v = a->id;                                  //id du noeud que l'on explore
                double weight = this->graph.mapping[u]->CalculerWeight(a);    //calcul le poids entre les deux noeuds
                double distance_through_u = dist + weight;
                if (distance_through_u < min_distance[v]) {
                    vertex_queue.erase(make_pair(min_distance[v], v));

                    min_distance[v] = distance_through_u;
                    previous[v] = u;
                    vertex_queue.insert(make_pair(min_distance[v], v));
                }
            }

        }
    }
}

//Fonction qui recompose le chemin à faire

list<int> Graphe::DijkstraChemin(int vertex, vector<int> &previous)
{
    list<int> path;
    for ( ; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}

list<int> Graphe::FaireAlgoComplet(int source, int destination, vector<double> &min_distance, vector<int> &previous, const double max_weight )
{
    this->DijkstraAlgo(source, min_distance, previous, max_weight);
    list<int> path = DijkstraChemin(destination, previous);
    return path;
}

bool Graphe::cheminExistant(vector<Noeud*> path)
{
    return path.size() > 1;
}

Map Graphe::getMap(){
    return graph;
}


