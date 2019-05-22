#include "map.h"

Map::Map(){}

Map::Map(int facteur)
{
    mapping = vector<Noeud*>(200);
    cote = facteur;

    /*mapping[0] = new Noeud(true, new Coordonnee(-1025*cote, 1190), 0);
    mapping[1] = new Noeud(true, new Coordonnee(-1025*cote, 1015), 1);
    mapping[2] = new Noeud(true, new Coordonnee(-1025*cote, 815), 2);
    mapping[3] = new Noeud(true, new Coordonnee(-1025*cote, 615), 3);
    mapping[4] = new Noeud(true, new Coordonnee(-1025*cote, 460), 4);
    mapping[5] = new Noeud(true, new Coordonnee(-1025*cote, 340), 5);
    mapping[6] = new Noeud(true, new Coordonnee(-1025*cote, 220), 6);
    mapping[7] = new Noeud(true, new Coordonnee(-825*cote, 1190), 7);
    mapping[8] = new Noeud(true, new Coordonnee(-825*cote, 1015), 8);
    mapping[9] = new Noeud(true, new Coordonnee(-825*cote, 815), 9);
    mapping[10] = new Noeud(true, new Coordonnee(-825*cote, 615), 10);
    mapping[11] = new Noeud(true, new Coordonnee(-825*cote, 460), 11);
    mapping[12] = new Noeud(true, new Coordonnee(-825*cote, 340), 12);
    mapping[13] = new Noeud(true, new Coordonnee(-825*cote, 220), 13);
    mapping[14] = new Noeud(true, new Coordonnee(-625*cote, 1190), 14);
    mapping[15] = new Noeud(true, new Coordonnee(-625*cote, 1015), 15);
    mapping[16] = new Noeud(true, new Coordonnee(-625*cote, 815), 16);
    mapping[17] = new Noeud(true, new Coordonnee(-625*cote, 615), 17);
    mapping[18] = new Noeud(true, new Coordonnee(-625*cote, 460), 18);
    mapping[19] = new Noeud(true, new Coordonnee(-625*cote, 340), 19);
    mapping[20] = new Noeud(true, new Coordonnee(-625*cote, 220), 20);
    mapping[21] = new Noeud(true, new Coordonnee(-425*cote, 1190), 21);
    mapping[22] = new Noeud(true, new Coordonnee(-425*cote, 1015), 22);
    mapping[23] = new Noeud(true, new Coordonnee(-425*cote, 815), 23);
    mapping[24] = new Noeud(true, new Coordonnee(-425*cote, 615), 24);
    mapping[25] = new Noeud(true, new Coordonnee(-425*cote, 460), 25);
    mapping[26] = new Noeud(true, new Coordonnee(-425*cote, 340), 26);
    mapping[27] = new Noeud(true, new Coordonnee(-425*cote, 220), 27);
    mapping[28] = new Noeud(true, new Coordonnee(-225*cote, 1190), 28);
    mapping[29] = new Noeud(true, new Coordonnee(-225*cote, 1015), 29);
    mapping[30] = new Noeud(true, new Coordonnee(-225*cote, 815), 30);
    mapping[31] = new Noeud(true, new Coordonnee(-225*cote, 615), 31);
    mapping[32] = new Noeud(true, new Coordonnee(-225*cote, 460), 32);
    mapping[33] = new Noeud(true, new Coordonnee(-225*cote, 340), 33);
    mapping[34] = new Noeud(true, new Coordonnee(-225*cote, 220), 34);
    mapping[35] = new Noeud(true, new Coordonnee(0*cote, 1190), 35);
    mapping[36] = new Noeud(true, new Coordonnee(0*cote, 1015), 36);
    mapping[37] = new Noeud(true, new Coordonnee(0*cote, 815), 37);
    mapping[38] = new Noeud(true, new Coordonnee(0*cote, 615), 38);
    mapping[39] = new Noeud(true, new Coordonnee(0*cote, 460), 39);
    mapping[40] = new Noeud(true, new Coordonnee(0*cote, 340), 40);
    mapping[41] = new Noeud(true, new Coordonnee(0*cote, 220), 41);
    mapping[42] = new Noeud(true, new Coordonnee(925*cote, 1190), 42);
    mapping[43] = new Noeud(true, new Coordonnee(925*cote, 1015), 43);
    mapping[44] = new Noeud(true, new Coordonnee(925*cote, 815), 44);
    mapping[45] = new Noeud(true, new Coordonnee(925*cote, 615), 45);
    mapping[46] = new Noeud(true, new Coordonnee(925*cote, 460), 46);
    mapping[47] = new Noeud(true, new Coordonnee(925*cote, 340), 47);
    mapping[48] = new Noeud(true, new Coordonnee(925*cote, 220), 48);
    mapping[49] = new Noeud(true, new Coordonnee(825*cote, 1190), 49);
    mapping[50] = new Noeud(true, new Coordonnee(825*cote, 1015), 50);
    mapping[51] = new Noeud(true, new Coordonnee(825*cote, 815), 51);
    mapping[52] = new Noeud(true, new Coordonnee(825*cote, 615), 52);
    mapping[53] = new Noeud(true, new Coordonnee(825*cote, 460), 53);
    mapping[54] = new Noeud(true, new Coordonnee(825*cote, 340), 54);
    mapping[55] = new Noeud(true, new Coordonnee(825*cote, 220), 55);
    mapping[56] = new Noeud(true, new Coordonnee(625*cote, 1190), 56);
    mapping[57] = new Noeud(true, new Coordonnee(625*cote, 1015), 57);
    mapping[58] = new Noeud(true, new Coordonnee(625*cote, 815), 58);
    mapping[59] = new Noeud(true, new Coordonnee(625*cote, 615), 59);
    mapping[60] = new Noeud(true, new Coordonnee(625*cote, 460), 60);
    mapping[61] = new Noeud(true, new Coordonnee(625*cote, 340), 61);
    mapping[62] = new Noeud(true, new Coordonnee(625*cote, 220), 62);
    mapping[63] = new Noeud(true, new Coordonnee(425*cote, 1190), 63);
    mapping[64] = new Noeud(true, new Coordonnee(425*cote, 1015), 64);
    mapping[65] = new Noeud(true, new Coordonnee(425*cote, 815), 65);
    mapping[66] = new Noeud(true, new Coordonnee(425*cote, 615), 66);
    mapping[67] = new Noeud(true, new Coordonnee(425*cote, 460), 67);
    mapping[68] = new Noeud(true, new Coordonnee(425*cote, 340), 68);
    mapping[69] = new Noeud(true, new Coordonnee(425*cote, 220), 69);
    mapping[70] = new Noeud(true, new Coordonnee(225*cote, 1190), 70);
    mapping[71] = new Noeud(true, new Coordonnee(225*cote, 1015), 71);
    mapping[72] = new Noeud(true, new Coordonnee(225*cote, 815), 72);
    mapping[73] = new Noeud(true, new Coordonnee(225*cote, 615), 73);
    mapping[74] = new Noeud(true, new Coordonnee(225*cote, 460), 74);
    mapping[75] = new Noeud(true, new Coordonnee(225*cote, 340), 75);
    mapping[76] = new Noeud(true, new Coordonnee(225*cote, 220), 76);*/

    int nbcases = 0;
    for(int j = 300 ; j <= 1200; j = j+100){
        for(int i=-1025 ; i <= 875; i = i+100){

            mapping[nbcases] = new Noeud(true, new Coordonnee(i*cote, j), nbcases);
            nbcases++;
            cout << i << " ; " << j << endl;
        }
    }
    cout <<"Taille map : " << nbcases << endl; 
        //Ajoute les voisins 
    lierNoeuds();

    cout << "Nombre de voisins du noeuds 1" << mapping[1]->listeVoisins.size() <<endl;



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
    cout << "taille de la map : " << taille << endl;
    for(int i = 0; i < mapping.size(); i++){
        //cout << "numero noeud : " << i << endl;
        DebloquerNoeud(i);
    }
}

void Map::lierNoeuds(){
    for(int i = 0; i<mapping.size(); i++){
        for(int j = 0; j<mapping.size(); j++){
            //Si le noeud est à moins de 30cm de celui là, il devient un de ses voisins
            if( i != j && mapping[i]->CalculerWeight(mapping[j])<300){
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
