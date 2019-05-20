#include "FichierPoint.h"

using namespace std;
namespace pt = boost::property_tree;

vector<Point> FichierPoint::readPoints(string filename) {
    vector<Point> pts;

    string type;
    int X, Y, angle;
    int deltaDeplacement;
    double deltaAngle;
    int vitesse;
    string sens, blocage;
    double coefCourbe;
    bool lissage, derapage, attAction;
    int timeOut;
    string action;

    cout << "Lecture du fichier point : '" << filename << "'" << endl;
    
    pt::ptree liste;
    pt::read_json(filename, liste);

    BOOST_FOREACH(const pt::ptree::value_type &point, liste.get_child("Points")) {
        X = 0;
        Y = 0;
        angle = 0;
        deltaDeplacement = 0;
        deltaAngle = 0.0;
        vitesse = 0;
        sens = "avant";
        blocage = "aucun";
        coefCourbe = 0.0;
        lissage = false;
        derapage = false;
        timeOut = 0;
        action = "null";
        attAction = false;

        type = point.second.get<string>("type");
        
        if(type.compare("Angle") == 0) {
            angle = point.second.get<int>("angle");
            deltaAngle = point.second.get<double>("deltaAngle");
            vitesse = point.second.get<int>("vitesse");
            blocage = point.second.get<string>("blocage");
            timeOut = point.second.get<int>("timeOut");
            action = point.second.get<string>("action");
            attAction = point.second.get<bool>("attAction");
        }
        else if(type.compare("Deplacement") == 0) {
            X = point.second.get<int>("X");
            Y = point.second.get<double>("Y");
            deltaDeplacement = point.second.get<int>("deltaDeplacement");
            deltaAngle = point.second.get<double>("deltaAngle");
            vitesse = point.second.get<int>("vitesse");
            sens = point.second.get<string>("sens");
            blocage = point.second.get<string>("blocage");
            coefCourbe = point.second.get<double>("coefCourbe");
            lissage = point.second.get<bool>("lissage");
            derapage = point.second.get<bool>("derapage");
            timeOut = point.second.get<int>("timeOut");
            action = point.second.get<string>("action");
            attAction = point.second.get<bool>("attAction");
        }
        else if(type.compare("DeplacementX") == 0) {
            X = point.second.get<int>("X");
            deltaDeplacement = point.second.get<int>("deltaDeplacement");
            deltaAngle = point.second.get<double>("deltaAngle");
            vitesse = point.second.get<int>("vitesse");
            sens = point.second.get<string>("sens");
            blocage = point.second.get<string>("blocage");
            coefCourbe = point.second.get<double>("coefCourbe");
            lissage = point.second.get<bool>("lissage");
            derapage = point.second.get<bool>("derapage");
            timeOut = point.second.get<int>("timeOut");
            action = point.second.get<string>("action");
            attAction = point.second.get<bool>("attAction");
        }
        else if(type.compare("DeplacementY") == 0) {
            Y = point.second.get<double>("Y");
            deltaDeplacement = point.second.get<int>("deltaDeplacement");
            deltaAngle = point.second.get<double>("deltaAngle");
            vitesse = point.second.get<int>("vitesse");
            sens = point.second.get<string>("sens");
            blocage = point.second.get<string>("blocage");
            coefCourbe = point.second.get<double>("coefCourbe");
            lissage = point.second.get<bool>("lissage");
            derapage = point.second.get<bool>("derapage");
            timeOut = point.second.get<int>("timeOut");
            action = point.second.get<string>("action");
            attAction = point.second.get<bool>("attAction");
        }
        else if(type.compare("Position") == 0) {
            X = point.second.get<int>("X");
            Y = point.second.get<double>("Y");
            angle = point.second.get<int>("angle");
            deltaDeplacement = point.second.get<int>("deltaDeplacement");
            deltaAngle = point.second.get<double>("deltaAngle");
            vitesse = point.second.get<int>("vitesse");
            blocage = point.second.get<string>("blocage");
            timeOut = point.second.get<int>("timeOut");
            action = point.second.get<string>("action");
            attAction = point.second.get<bool>("attAction");
        }
        else if(type.compare("RecalageX") == 0) {
            X = point.second.get<int>("X");
            angle = point.second.get<int>("angle");
        }
        else if(type.compare("RecalageY") == 0) {
            Y = point.second.get<int>("Y");
            angle = point.second.get<int>("angle");
        }
        else if(type.compare("RecalageXY") == 0) {
            X = point.second.get<int>("X");
            Y = point.second.get<int>("Y");
            angle = point.second.get<int>("angle");
        }
        else if(type.compare("DeplacementRelatif") == 0) {
            X = point.second.get<int>("X");
            Y = point.second.get<double>("Y");
            deltaDeplacement = point.second.get<int>("deltaDeplacement");
            vitesse = point.second.get<int>("vitesse");
            sens = point.second.get<string>("sens");
            blocage = point.second.get<string>("blocage");
            coefCourbe = point.second.get<double>("coefCourbe");
            lissage = point.second.get<bool>("lissage");
            derapage = point.second.get<bool>("derapage");
            timeOut = point.second.get<int>("timeOut");
            action = point.second.get<string>("action");
            attAction = point.second.get<bool>("attAction");
        }
        else if(type.compare("AngleRelatif") == 0) {
            angle = point.second.get<int>("angle");
            deltaAngle = point.second.get<double>("deltaAngle");
            vitesse = point.second.get<int>("vitesse");
            blocage = point.second.get<string>("blocage");
            timeOut = point.second.get<int>("timeOut");
            action = point.second.get<string>("action");
            attAction = point.second.get<bool>("attAction");
        }
        else {
            continue;//aucun type connu, on passe à l'itération suivante
        }

        Point p(type, X, Y, angle, deltaDeplacement, deltaAngle, vitesse, sens, blocage, coefCourbe, lissage, derapage, timeOut, action, attAction);
        pts.push_back(p);
    }
    return pts;
}