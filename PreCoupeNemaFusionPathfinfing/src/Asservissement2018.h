#ifndef ASSERVISSEMENT2018_H_INCLUDED
#define ASSERVISSEMENT2018_H_INCLUDED

#include <iostream>
#include <cmath>
#include <cstdint>
#include <netdb.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>


#include "ClientUDP.h"
#include "Clock.h"
#include "Config.h"
#include "ICodeurManager.h"
#include "MoteurManager.h"
#include "Point.h"
#include "ServoManager.h"



/// @brief Classe permettant de gérer toute la partie d’asservissement
class Asservissement2018
{
public:
    /// @brief Constructeur par défaut de l’asservissement
    ///
    /// @param moteurs Les deux moteurs à gérer
    /// @param codeurs Les deux codeurs à lire
    /// @param st instance de la classe Strategie
    Asservissement2018(
            MoteurManager& moteurs,
            ICodeurManager& codeurs,
            Config conf,
            timer &tmps,
        ClientUDP& udp);

    // Méthodes
    void initialiser(const Point& pt);

    void resetCodeur();

    void asservir();

    Point getPointActuel() const;

    void pointSuivant(Point point);

    void stop();

    void loadConfig(Config conf);

    bool demandePointSuivant();

    bool blocageMoteur();

    void PrintTick();

    void resetTick();

    void setMoteur(double vitG, double vitD);

    Point getCoordonnees();

    void setVitessePointActuel(int);

private:

    void odometrie();
    void calculConsigne();

    void Deplacement();
    void Angle();
    void Position();
    void RecalageXY();
    void RecalageX();
    void RecalageY();

    void asservVitesse(double vitConsigneG, double vitConsigneD);
    void asservPosition();
    void asservAngle(double vit);
    void correctionAngle();



    Config config;

    timer &temps;
    ClientUDP client;
    // Moteurs
    MoteurManager &moteurs;
    int cmdG = 0, cmdD = 0;
    // Codeurs
	ICodeurManager &codeurs;

    //Point à atteindre
    Point pointActuel;

    //Valeur des codeurs gauche et droite
    int comptG;
    int comptD;

    //Coordonnées actuelles
    double x, y;

    double dX, dY;
    //Angle du robot;
    double angle;

    //Point cible
    double xCible;
    double yCible;

    //Calcul consigne
    double erreurAngle=0;
    double consigne_distance, consigne_angle;

    bool rotationDone;

    bool moteurBloque;


    //Fin d'asservissement
    bool asservFini = false;
    double coefAccel = 0.5;
    double ditanceAParcourir;
    double pourcentageDistance;

    //Coefficient tick codeur  -> distance
    double CoeffGLong;
	double CoeffDLong;

    double CoeffAngl;


    double mesure_dist, mesure_angle, mesure_vitesse;
    int tempsLast;


    //Coefs PID en angle
    double kpA, kiA, kdA;
    //Coefs PID en angle en déplacement
    double kpDep, kiDep, kdDep;
    //Coefs PID en position
    double kpPos, kiPos, kdPos;


    //Asserv en vitesse
    double old_erreurG = 0, old_erreurD = 0;
    double somme_erreurG = 0, somme_erreurD = 0;
    double dErreurG, dErreurD;
    double vitG, vitD;
    double 	nbVitG=0, nbVitD=0;

    //Asserv en angle
    double old_erreurAngle = 0;
    double somme_erreurAngle = 0;
    double consigneG =0 , consigneD = 0;

    //Correction d'angle
    double erreurCorrection=0;
    double old_erreurCorrection= 0;
    double somme_erreurCorrection = 0;


    //Asserv en position
    double old_erreurPosition = 0;
    double somme_erreurPosition = 0;
    double consigneVitesse;

    double ticksG=0;
    double  ticksD=0;



};


#endif //ASSERVISSEMENT2018_H_INCLUDED
