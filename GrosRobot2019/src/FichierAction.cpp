#include "FichierAction.h"

using namespace std;

vector<ActionServo> FichierAction::readPoints(string filename){
	vector<ActionServo> act;
	string tmpString;
	char type;
	int numero;
	int angle;
	int force;
	int time;
	ifstream file(filename);
	printf("Ouverture Action : [%s]\n",filename.c_str());
	printf("fichier ok : %d\n",file.good());
	printf("fichier fini : %d\n",file.eof());
	printf("fichier fail : %d\n",file.fail());
	printf("fichier bad : %d\n",file.bad());
	bool bloc_com = false;

	while(file.good()){
		getline(file, tmpString);
		stringstream read(tmpString);

		if(tmpString.find("/*") != string::npos)
		{
			bloc_com = true;
			cout << "Bloc commentaire: " << tmpString << "\n";
			continue;
		}
		else if(tmpString.find("*/") != string::npos && bloc_com)
		{
			bloc_com = false;
			cout << "Bloc commentaire: " << tmpString << "\n";
			continue;
		}
		else if(bloc_com)
		{
			cout << "Bloc commentaire: " << tmpString << "\n";
			continue;
		}
		else if(countSubstring(tmpString, " ") < 3)
		{
			cout << "Ligne invalide: " << tmpString << "\n";
			continue;
		}
		else if(tmpString.find("#") != string::npos
			|| tmpString.find("//") != string::npos
			|| tmpString.find("--") != string::npos
			|| tmpString.find("%") != string::npos)
		{
			cout << "Commentaire isolé: " << tmpString << "\n";
			continue;
		}

		cout << "Action: " << tmpString << "\n";

		read >> type;
		read >> numero;
		read >> angle;
		read >> force;
		read >> time;
		ActionServo p(type,numero,angle,force,time);
		act.push_back(p);
	}
	return act;
}

int FichierAction::countSubstring(const std::string& str, const std::string& sub)
{
	if (sub.length() == 0) return 0;
	int count = 0;
	for (size_t offset = str.find(sub); offset != std::string::npos;
	offset = str.find(sub, offset + sub.length()))
	{
		++count;
	}
	return count;
}
