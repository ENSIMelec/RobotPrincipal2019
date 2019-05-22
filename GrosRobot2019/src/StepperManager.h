#ifndef STEPPERMANAGER_H_INCLUDED
#define STEPPERMANAGER_H_INCLUDED

/*!
 * \file StepperManager.h
 * \brief utilisation d'un stepper en i2c
 * \author ENSIM'Elec
 * \version 0.1
 * \date 7 mars 2019
 */

#include <stdint.h>
#include <unistd.h>

#define I2CADDRESS 0x09


/*! \class StepperManager
   * \brief Classe d'utilisation d'un moteur pas a pas
   *
   *  Cette classe permet de d'initialiser et de controller
   *  un moteur pas a pas via un driver en i2c
   *
   */


class StepperManager
{

public:
    /*!
        *   \brief iniatilise la liaison i2c et le driver stepper
        *  Constructeur de la classe StepperManager.
        *  Le constructeur initialise les variables, ouvre la liaison i2c et home le moteur
        */
    StepperManager();

    /*!
        *  \brief Home et close
        *  Destructeur de la classe StepperManager.
        *  Le destructeur home le moteur et ferme la liaison i2c
        */
    ~StepperManager();

    /*!
        *  \brief Modifie la position du moteur
        *  Met à jour la position cible du moteur, renvoie la commande tant que la variable n'a pas été changée
        *  \param pos entier de 0 à (max à calculer) en mm
        */
    int setPosition(int pos);

    /*!
        *  \brief Modifie la vitesse du moteur
        *  Met à jour la vitesse cible du moteur,(To do renvoie la commande tant que la variable n'a pas été changée)
        *  \param pos entier de 0 à 50 en mm/s
        */

    int setSpeed(int speed);

    /*!
        *  \brief Modifie la vitesse de home du moteur
        *  Met à jour la vitesse cible du moteur,(To do renvoie la commande tant que la variable n'a pas été changée)
        *  \param pos entier de 0 à (max à calculer) en mm/s
        */


    int setHomingSpeed(int speed);


    /*!
        *  \brief Change les step du moteur jusqu'a la prochaine mise hors tension de la carte
        *
        *   0: Full step
            1: 1/2 step
            2: 1/4 step
            3: 1/8 step
        */
    int setStepMode(uint8_t mode);

    /*!
        *  \brief Sort du mode safe
        *  A l'allumage le moteur est verouille il faut le debloquer avec cette fonction
        * met a jour le multiplicateur
        */

    int exitSafeStart();

    /*!
        *  \brief Initialise l'origine du moteur
        *  Lance le moteur jusqu'a ce qu'il touche un endstop de la direction choisie
        *  met a jour la variable homed à la fin
        */

    int goHome();

    /*!
        *  \brief renvoie la position actuel
            La fonction permet de verifier la position théorique du moteur, sans prendre en compte les pas sautés
        */

    int getCurrentPosition(int32_t *output);

    /*!
        *  \brief renvoie la position cible
            La fonction permet de verifier la position cible du moteur, afin d'être sur que la commande setPosition est passée
        */

    int getTargetPosition(int32_t *output);

    /*!
        *  \brief libere le moteur et la liaison i2c
        * Desalimente le moteur et libere le fichier i2c
        */

    int closeStep();

    static int writeError(char *error);
protected:

    /*!
        *  \brief permet de savoir si la procédure de homing est finie
        * la valeur attendue est 16 pendant le home et 0 à la fin
        */
    int getHomingFlag(uint8_t *output);
    /*!
        *  \brief fonction d'acces aux registre
        * fonction appelé par tout les get pour obtenir des valeurs de la carte
        */
    int getVariable(uint8_t offset, uint8_t *buffer, uint8_t length);


    int goAway();

    /*!
        *  \brief libere le moteur
        */
    int deEnergize();
    /*!
        *  \brief realimente le moteur et autorise le moteur à tourner si il n'y a pqs d'qutres erreurs
        */
    int Energize();

    const char * device;/*!< Adresse du fichier i2c*/
    uint8_t  address;/*!< Adresse i2c de la carte stepper*/
    int i2c_stepper;/*!< Descripteur du fichier i2c ouvert*/
    int homed;/*!< indique si le moteur à été home*/
    int Multiplier/*!< Multiplicateur pour le calcul des positions et vitesses en fonction du stepMode*/;
    int init;
};

#endif //SERVOMANAGER_H_INCLUDED
