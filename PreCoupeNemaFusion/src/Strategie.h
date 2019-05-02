#ifndef STRATEGIE_H_INCLUDED
#define STRATEGIE_H_INCLUDED

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "FichierPoint.h"

#define EXIT_SUCCESS 0
#define EXIT_READ_FAIL 1
#define EXIT_PARSER_FAIL 2
#define EXIT_NO_OBJECTIVE -1 //Ne doit pas être positif


class Strategie
{
public:
	Strategie(std::string path);


	std::string getNameFileStrat();
	std::string getNameFilePoint();
	std::string getStatut();
	unsigned int getNbObjectifs();

	Point getPointActuel();
	Point getPointSuivant();

	std::string getFileAction();
	int getSensDeplacement();
	int getOptionBlocage();
	unsigned int getTimeOut();

	bool haveToWaitActionDone();

    bool isNotFinished();
	void setVitesse(int vit);

	void setStatut(std::string stat);
	void setStatutCouleurs(std::string statCoul);
	void incrNbObjectifs();
	void pushInteruption();
	void popInteruption();

	Point eloigner(Point pt, int distanceMM);
	bool estEloigne();

	std::vector<std::vector<std::string> > objectifs;



private:
	int lireStrat();//lit la strategie, et l'enregistre dans la classe
	int parserLigneChoix(std::string line);

	unsigned int indexObjectif;
	unsigned int indexPoint;
	unsigned int indexLigne;

	std::string pathFolder;
	std::string nameFileStrat;
	std::string nameFilePoint;
	std::string statut;
	std::string statutCouleurs;
	unsigned int nbObjectifs;

	std::vector<bool> objectifsDone; //liste des objectifs pour savoir s'il sont realise ou pas
	std::vector<std::string> nomObjectifs;
	std::vector<std::vector<unsigned int> > pileInterruption; //objectif - point - ligne

	std::vector<Point> points; //point de lecture
	Point pointActuel;
	Point pointSuivant;

	bool loin;
    bool finished;
};

#endif // STRATEGIE_H_INCLUDED
