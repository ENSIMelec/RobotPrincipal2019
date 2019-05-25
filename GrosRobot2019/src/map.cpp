#include "map.h"

Map::Map(){}

Map::Map(int facteur)
{
    mapping = vector<Noeud*>(200);
    cote = facteur;

    int nbcases = 0;
    for(int j = 300 ; j <= 1200; j = j+100){
        for(int i=-1025 ; i <= 875; i = i+100){
            mapping[nbcases] = new Noeud(true, new Coordonnee(i*cote, j), nbcases);
            nbcases++;
        }
    }
        //Ajoute les voisins 
    lierNoeuds();

}

//Bloque un noeud
void Map::BloquerNoeud(int id)
{
    mapping[id]->etat = false;
}

//Débloque un noeud
void Map::DebloquerNoeud(int id)
{
    mapping[id]->etat = true;
}

int Map::getX(int n)
{
    return this->mapping[n]->coordonnees->x;
}

int Map::getY(int n)
{
    return this->mapping[n]->coordonnees->y;
}

Noeud* Map::getNoeud(int noeud)
{
    return mapping[noeud];
}

vector<Noeud*> Map::getMapping(){
    return mapping;
}


void Map::debloquerTousLesNoeuds(){
    //cout << "taille de la map : " << mapping.size() << endl;
    for(int i = 0; i < mapping.size(); i++){
        //cout << "numero noeud : " << i << endl;
        DebloquerNoeud(i);
    }
}

void Map::lierNoeuds(){
    for(int i = 0; i<mapping.size(); i++){
        for(int j = 0; j<mapping.size(); j++){
            //Si le noeud est à moins de 30cm de celui là, il devient un de ses voisins
            if( i != j && mapping[i]->CalculerWeight(mapping[j])<200){
                mapping[i]->AddNoeudVoisin(mapping[j]);
            }
        }
    }
}

Noeud* Map::noeudLePlusProche(Noeud* noeudActuel){
    double min_dist = 3000;
    double dist = 0;
    int index = 0;

    for(int i = 0; i < mapping.size(); i++){
        dist = mapping[i]->CalculerWeight(noeudActuel);
        if(min_dist >= dist && mapping[i]->getEtat()){
            min_dist = dist;
            index = i;
        }
    }
    //cout << "Id final : " << mapping[index]->getX() << " | Id du Noeud : " << mapping[index]->getY() << endl;
    return mapping[index];
}
