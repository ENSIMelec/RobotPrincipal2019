#include "Strategie.h"

using namespace std;

//Constructeur
Strategie::Strategie(string path){
    cout << "--- Constructeur ---" << endl;
    finished = false;
    pathFolder = path;
    nameFileStrat = pathFolder + "main.strat";
    nbObjectifs = 0;
    statut = "null";
    statutCouleurs = "null";
    indexObjectif = 0;
    indexPoint = 0;
    indexLigne = 0;
    loin = false;
    cout << "\tLecture de la strategie de : [" << nameFileStrat <<"]"<< endl;
    lireStrat();
    cout <<"Nb objectifs " << nbObjectifs <<endl;
    nameFilePoint = pathFolder + objectifs[indexObjectif][indexLigne];
    cout << "\tLecture des Points de : '" << nameFilePoint << "'" << endl;
    points = FichierPoint::readPoints(nameFilePoint);
    cout << "\tnombre de points lus : " << points.size() << endl;
    cout << "--- Fin Constructeur ---" << endl;
}

int Strategie::lireStrat() {
    //lit la strategie, et l'enregistre dans la classe
    ifstream fichier(nameFileStrat.c_str());
    string line;

    while(getline(fichier, line)) {
        if(line.find("(") != string::npos){//npos est la valeur renvoyee si la valeur n'est pas trouvee
            //Si le caractere est trouve, on est dans une situation de "choix". il faudra parser la ligne pour la traiter.
            //On traitera la ligne a la volee dans le programme, on l'enregistre juste
            objectifs[getNbObjectifs()-1].push_back(line.substr(0,line.size()-1));
            cout << "Choix : " <<objectifs[getNbObjectifs()-1].back() <<endl;
        }
        else if(line.find(".json") != string::npos) {
            objectifs[getNbObjectifs()-1].push_back(line.substr(0,line.find(".json")+5));
            cout << "Ligne : " <<objectifs[getNbObjectifs()-1].back() <<endl;
        }
        else if(line.find("[") != string::npos) {
            nomObjectifs.push_back(line.substr(1, line.find("]")-1));
            objectifs.push_back(vector<string>()); // ajoute une ligne de fichiers points
            if(line.find("@facultatif") != string::npos){
                objectifsDone.push_back(true); //ajoute dans la liste des objectif a realiser
            }else{
                objectifsDone.push_back(false);
            }
            incrNbObjectifs();
            cout << "["<< nomObjectifs[nbObjectifs-1]<<"] " << endl;
        }
    }
    fichier.close();
    //cout << "Fin lecture strat" << endl;
    return EXIT_SUCCESS;
}


int Strategie::parserLigneChoix(string line) {
    cout << "[Parser ligne]" << endl;
    //renvoit le nom du fichier point en fonction du statut (on effectue donc un choix)
    int indiceParentheseOuvrante = -1;
    int indiceParentheseFermante = 0;

    string statutParser;
    while((unsigned)(indiceParentheseOuvrante = line.find("(", indiceParentheseOuvrante+1)) != string::npos) {
        //Tant qu'on a des parentheses ouvrantes
        indiceParentheseFermante = line.find(")", indiceParentheseFermante+1); //Si le fichier est bien fait, on trouve une parenthese fermante quelque part ... Sinon, erreur !
        if((unsigned)indiceParentheseFermante == string::npos) //Si pas de parenthese fermante, fail
            return EXIT_PARSER_FAIL;
        statutParser = line.substr(indiceParentheseOuvrante+1, indiceParentheseFermante-indiceParentheseOuvrante-1);
        //on recupere le statut correspondant
        if(statutParser.compare(statutCouleurs) == 0 || statutParser.compare(statut) == 0 || statutParser.compare("default") == 0) {
            //si le statut corespond, ou qu'on a pas d'autres choix, on enregistre le nom du fichier point associe
            cout << "Go Statut : " << statutParser << endl;
            unsigned int indice = line.find("(", indiceParentheseFermante);
            cout << "Indice : " << indice << endl;

            if(indice != string::npos){
                cout << "milieu de ligne" << endl;
                string substr = line.substr(indiceParentheseFermante, indice-indiceParentheseFermante+1);
                cout << "find '" << substr << "'" << endl;
                if(substr.find(".json") != string::npos) {
                    cout << "trouve1" << endl;
                    nameFilePoint = pathFolder + line.substr(indiceParentheseFermante+2, indice-3-indiceParentheseFermante);
                }else {
                    string nomStrategie =  line.substr(indiceParentheseFermante+3, indice-5-indiceParentheseFermante);
					objectifsDone[indexObjectif] = true;
                    cout << "Nom objectif1 :["<< nomStrategie << "]"<<endl;
                    for(unsigned int i=0 ; i<nbObjectifs ; i++){
                        if(nomObjectifs[i].compare(nomStrategie) == 0){
                            indexPoint = 0;
                            indexLigne = 0;
                            indexObjectif = i;
                            cout << "Choix Objectif1 : " << nomObjectifs[i] << endl;
                            nameFilePoint = pathFolder + objectifs[indexObjectif][indexPoint];
                            return EXIT_SUCCESS;
                        }
                    }
                    cout << "Erreur 1 " << endl;
                    return EXIT_PARSER_FAIL;
                }
            }else{
                cout << "fin de ligne" << endl;
                string substr = line.substr(indiceParentheseFermante, indice-indiceParentheseFermante+1);
                cout << "find '" << substr << "'" << endl;
                if(line.find(".json",indiceParentheseFermante) != string::npos){
                    cout << "trouve2" << endl;
                    nameFilePoint = pathFolder + line.substr(indiceParentheseFermante+2, line.size()-2-indiceParentheseFermante);
                }else {
                    string nomStrategie =  line.substr(indiceParentheseFermante+3,line.size()-4-indiceParentheseFermante);
                    objectifsDone[indexObjectif] = true;
                    cout << "Nom objectif2 :["<< nomStrategie << "]"<<endl;
                    for(unsigned int i=0 ; i<nbObjectifs ; i++){
                        if(nomObjectifs[i].compare(nomStrategie) == 0){
                            indexPoint = 0;
                            indexLigne = 0;
                            indexObjectif = i;
                            cout << "Choix Objectif2 : " << nomObjectifs[i] << endl;
                            nameFilePoint = pathFolder + objectifs[indexObjectif][indexPoint];
                            return EXIT_SUCCESS;
                        }
                    }
                    cout << "Erreur 2 " << endl;
                    return EXIT_PARSER_FAIL;
                }
            }
            cout << "Choix : " << nameFilePoint << endl;
            return EXIT_SUCCESS;
        }
    }
    return EXIT_PARSER_FAIL;
}


string Strategie::getStatut() {
    return statut;
}

void Strategie::setStatut(string stat){
    statut = stat;
}

void Strategie::setStatutCouleurs(string statCoul) {
    statutCouleurs = statCoul;
}

unsigned int Strategie::getNbObjectifs() {
    return nbObjectifs;
}

Point Strategie::getPointActuel() {
    //cout << "Point actuel : "; points[indexLigne].display();
    return points[indexLigne];
}

//renvoie le point actuel si plus de point
Point Strategie::getPointSuivant() {
    loin = false;
    cout << "Statut : " << statut << endl;
    cout <<"Nb interruption : "<< pileInterruption.size() << endl;
    if(statut == "Bloque"){// interruption
        pushInteruption(); //ajout du point interrompu
        do {
            indexObjectif++;
        }while(indexObjectif < nbObjectifs && objectifsDone[indexObjectif]);
        //Tant qu'on trouve pas d'objectif marque "non fait", et qu'on arrive pas a la fin de la liste des objectifs, on continue
        if(indexObjectif < nbObjectifs) { //Si il y a un objectif suivant trouve
            indexLigne = 0;
            indexPoint = 0;
        } else { //Sinon, on a effectue l'ensemble des objectifs, sauf ceux de la pile
            if(pileInterruption.size() >= 2){//Si il y a maintenant au moins 2 interruptions, on échange la dernière (celle qui vient d'être push) par l'avant dernière
                vector<unsigned int> old = pileInterruption[pileInterruption.size()-2];
                pileInterruption[pileInterruption.size()-2] = pileInterruption[pileInterruption.size()-1];
                pileInterruption[pileInterruption.size()-1] = old;
                popInteruption();
            } else {//Si l'interruption qu'on a push est la seule dans la pile, on la reprend
                popInteruption();
            }
        }
        if(objectifs[indexObjectif][indexPoint].find("(")!= string::npos){
            parserLigneChoix(objectifs[indexObjectif][indexPoint]);
        }else{
            nameFilePoint = pathFolder + objectifs[indexObjectif][indexPoint]; //On charge le nom du fichier point
        }
        points = FichierPoint::readPoints(nameFilePoint); //On enregistre les points
        statut = "null";
    }
    else if(indexLigne < points.size()-1) { //Si on a pas fini la lecture de tous les points du fichier point courant
        indexLigne++; //On change de ligne
        cout << "[Ligne suivante]" << endl;
    }
    else if(indexPoint < objectifs[indexObjectif].size()-1) {//Sinon si on a pas fini la lecture de tous les fichiers point de l'objectif courant
        indexLigne = 0;
        indexPoint++; //On change de fichier point
        cout << "[Fichier Point suivant]" << endl;
        if(objectifs[indexObjectif][indexPoint].find("(")!= string::npos){
            parserLigneChoix(objectifs[indexObjectif][indexPoint]);
        }else{
            nameFilePoint = pathFolder + objectifs[indexObjectif][indexPoint]; //On charge le nom du fichier point
        }
        points = FichierPoint::readPoints(nameFilePoint);
    }
    else {//Si on a fini un objectif
        cout << "[Objectif suivant]" << endl;
        objectifsDone[indexObjectif] = true; //Marque l'objectif "fait"
        if(pileInterruption.empty() == false) { //Si on a une pile d'interruption, on remonte
            popInteruption();
            if(objectifs[indexObjectif][indexPoint].find("(")!= string::npos){
                parserLigneChoix(objectifs[indexObjectif][indexPoint]);
            }else{
                nameFilePoint = pathFolder + objectifs[indexObjectif][indexPoint]; //On charge le nom du fichier point
            }
            points = FichierPoint::readPoints(nameFilePoint);
        }
        else {//Si on a rien a depiler, on passe a l'objectif suivant
            do {
                indexObjectif++;
            }while(indexObjectif < nbObjectifs && objectifsDone[indexObjectif]);
            //Tant qu'on trouve pas d'objectif marque "non fait", et qu'on arrive pas a la fin de la liste des objectifs, on continue
            if(indexObjectif < nbObjectifs) { //Si il y a un objectif suivant
                indexLigne = 0;
                indexPoint = 0;
                if(objectifs[indexObjectif][indexPoint].find("(")!= string::npos){
                    parserLigneChoix(objectifs[indexObjectif][indexPoint]);
                }else{
                    nameFilePoint = pathFolder + objectifs[indexObjectif][indexPoint]; //On charge le nom du fichier point
                }
                points = FichierPoint::readPoints(nameFilePoint);
            } else { //Sinon, on a effectue l'ensemble des objectifs
                finished = true;
                cout << "FIN !" << endl;
                return getPointActuel();
            }
        }
    }
    cout << "###" << endl;
    cout << "Objectif( "<< indexObjectif << ") : "<< nomObjectifs[indexObjectif] << endl;
    cout << "Point( "<< indexPoint << ") : " << nameFilePoint << endl;
    cout << "Ligne : ";
    points[indexLigne].display();
    cout << "###" << endl;
    return points[indexLigne]; //On envoie la ligne
}

string Strategie::getFileAction() {
    return getPointActuel().getAction();
}

int Strategie::getSensDeplacement() {
    return getPointActuel().getSens();
}

int Strategie::getOptionBlocage() {
    return getPointActuel().getBlocage();
}

bool Strategie::haveToWaitActionDone() {
    return getPointActuel().getAttAction();
}

unsigned int Strategie::getTimeOut() {
    return (unsigned int)getPointActuel().getTimeOut();
}

string Strategie::getNameFileStrat() {
    return nameFileStrat;
}
string Strategie::getNameFilePoint() {
    return nameFilePoint;
}

bool Strategie::isNotFinished(){
    return !finished;
}
void Strategie::incrNbObjectifs() {
    nbObjectifs++;
    return;
}

void Strategie::pushInteruption() {
    cout << "Ajout interruption" << endl;
    pileInterruption.push_back(vector<unsigned int>()); //On ajoute une interuption
    int nbInteruption = pileInterruption.size();
    pileInterruption[nbInteruption-1].push_back(indexObjectif);
    pileInterruption[nbInteruption-1].push_back(indexPoint);
    pileInterruption[nbInteruption-1].push_back(indexLigne); //On stock l'endroit ou on s'est arrete
    objectifsDone[indexObjectif] = true; //On met l'objectif dans l'etat "fait" pour ne pas retourner dessus autrement qu'en dépilant
    return;
}

void Strategie::popInteruption() {
    vector<unsigned int> interruption = pileInterruption.back();//recuperation de l'interruption
    indexObjectif = interruption[0];
    indexPoint = interruption[1];
    indexLigne = interruption[2];
    cout << "Recuperation interruption ["<<indexObjectif<<","<<indexPoint<<","<<indexLigne<<"]"<< endl;
    objectifsDone[indexObjectif] = false; //Puisqu'on reprend l'objectif, il n'est plus dans l'etat provisoire "fait"
    pileInterruption.pop_back();//Supression de l'interuption dans le tableau
    return;
}



Point Strategie::eloigner(Point pt, int distanceMM) {
    int monX, monY, monAngle, dx, dy;
    string monSens;
    string monBlocage;

    monX = pt.getX();
    monY = pt.getY();
    monAngle = pt.getAngle();

    dx = distanceMM * sin(monAngle * M_PI / 180);
    dy = distanceMM * cos(monAngle * M_PI / 180);

    if(pt.getSens() == 0) {
        if(monAngle < -90) {
            monX += dx;
            monY += dy;
        } else if(monAngle < 0) {
            monX += dx;
            monY -= dy;
        } else if(monAngle < 90) {
            monX -= dx;
            monY -= dy;
        } else {
            monX -= dx;
            monY += dy;
        }
    } else if(pt.getSens() == 1) {
        if(monAngle < -90) {
            monX -= dx;
            monY -= dy;
        } else if(monAngle < 0) {
            monX -= dx;
            monY += dy;
        } else if(monAngle < 90) {
            monX += dx;
            monY += dy;
        } else {
            monX += dx;
            monY -= dy;
        }
    }
/*
    if(monAngle < -180){
        monAngle = monAngle + 360;
    } else if(monAngle > 180){
        monAngle = monAngle - 360;
    }
*/
    loin = true;
    return Point("Deplacement", monX, monY, monAngle, 50, 1.0, 300, monSens, monBlocage, 0.0, false, false, 0, "null", false);
}

bool Strategie::estEloigne() {
    return loin;
}

void Strategie::setVitesse(int vit) {
    points[indexLigne].setVitesse(vit);
    return;
}
