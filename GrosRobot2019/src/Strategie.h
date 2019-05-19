#ifndef STRATEGIE_H_INCLUDED
#define STRATEGIE_H_INCLUDED

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "FichierPoint.h"
#include "graphe.h"
#include "noeud.h"
#include "coordonnee.h"
#include "Point.h"
#include "Asservissement2018.h"


#define EXIT_SUCCESS 0
#define EXIT_READ_FAIL 1
#define EXIT_PARSER_FAIL 2
#define EXIT_NO_OBJECTIVE -1 //Ne doit pas être positif

#define VIOLET 1
#define JAUNE -1

using namespace std;


class Strategie
{
public:
	Strategie(string path, string cote_table);

	string getNameFileStrat();
	string getNameFilePoint();
	string getStatut();
	unsigned int getNbObjectifs();

	Point getPointActuel();
	Point getPointSuivant();

	string getFileAction();
	int getSensDeplacement();
	int getOptionDetection();
	unsigned int getTimeOut();

	bool haveToWaitActionDone();

    bool isNotFinished();
	void setVitesse(int vit);

	void setStatut(string stat);
	void setStatutCouleurs(string statCoul);
	void incrNbObjectifs();
	void pushInteruption();
	void popInteruption();

	Point eloigner(Point pt, int distanceMM);
	bool estEloigne();

	vector<vector<string> > objectifs;

	Noeud* getNoeudByPoint(Point point);
	vector<Noeud*> trouverCheminVersObjectif(int pointDepart, int destination);
	void setPathfindingInAction(bool);
	void convertNoeudsIntoPoints();
	Point convertNodeIntoPoint(Noeud*);
	void drawCurrentPath();
	void blockNodes(Noeud* centralPoint);
	Graphe* getMaTable();
	Noeud* getNoeudCurrentItineraire();
    vector<Noeud*> getCurrentItineraire();
    bool getPathfindingInAction();

    Noeud* getDepartPathfinding();
    void setDepartPathfinding(Point);
    void setObjectifAatteindre(Point);

    Noeud* createNodeByPoint(Point);

private:
	int lireStrat();//lit la strategie, et l'enregistre dans la classe
	int parserLigneChoix(string line);

	unsigned int indexObjectif;
	unsigned int indexPoint;
	unsigned int indexLigne;
	unsigned int indexItineraire;

	string pathFolder;
	string nameFileStrat;
	string nameFilePoint;
	string statut;
	string statutCouleurs;
	unsigned int nbObjectifs;

	vector<bool> objectifsDone; //liste des objectifs pour savoir s'ils sont realises ou pas
	vector<string> nomObjectifs;
	vector<vector<unsigned int> > pileInterruption; //objectif - point - ligne

	vector<Point> points; //point de lecture
	Point pointActuel;
	Point pointSuivant;

	bool loin;
    bool finished;


    Graphe* maTable;
    vector<Noeud*> currentItineraire;
    vector<Point> cheminVersPoint;
    bool pathfindingInAction;
    list<int> currentPath;
    Point objectifAatteindre;
    Noeud* departPathfinding;

};

#endif // STRATEGIE_H_INCLUDED
