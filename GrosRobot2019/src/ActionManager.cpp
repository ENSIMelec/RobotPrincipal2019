#include "ActionManager.h"

using namespace std;

//Constructeur par défaut : utilise la liste d'initialisation pour init les classes utilisées dans la classe
ActionManager::ActionManager(int i2c_Servos, int i2c_Stepper, int nbAX12, ClientUDP& udp) :
	client(udp), servos(i2c_Servos), stepper(),  AX12(nbAX12)
{
	stepper.goHome();
	servos.servoInitialisation();
	printf("Constructeur action");
}


void ActionManager::close() {
	cout << "Arrêt du lanceur" << endl;
	sleepMillis(250);
	AX12.close();
	return;
}


void ActionManager::action(string filname) {
	
	unsigned int indexAction = 0;
	vector<ActionServo> actions;
	actions = FichierAction::readPoints(filname);
	char typeActionneur;
	int numActionneur, angleAction, forceAction, tempsAction;
	

	while(indexAction < actions.size()) {

		typeActionneur = actions.at(indexAction).gettype();
		numActionneur = actions.at(indexAction).getn();
		angleAction = actions.at(indexAction).geta();
		forceAction = actions.at(indexAction).getf();
		tempsAction = actions.at(indexAction).gett();
		

		switch(typeActionneur) {
			case 'S':
				servos.servoAction(numActionneur, angleAction);
				client.sendMessage("I Servo n°"+to_string(numActionneur)+" angle : "+to_string(angleAction));
				break;
			case 'N':
				// Ici angleAction represente la hauteur en mm
				stepper.setSpeed(forceAction);
				stepper.setPosition(angleAction);

				client.sendMessage("I Stepper hauteur" + to_string(angleAction));
				break;
			case 'P':
				client.sendMessage("I Ajout de "+to_string(numActionneur)+" points");
				client.addPoints(numActionneur, angleAction);
				break;
			case 'A':
				tempsAction -= AX12.AX12Action(numActionneur, angleAction, forceAction); //On effectue l'action AX12, on met à jour le temps qu'il reste avant la fin de l'action
				client.sendMessage("I AX12 n°"+to_string(numActionneur)+" angle : "+to_string(angleAction)+" force : "+to_string(forceAction));
				break;//Attention aux delays
			default: //On ne lance pas d'action, mais on attend le temps demandé
				break;
		}
		if(tempsAction > 0)
		{
			cout << "Attente " << tempsAction << " ms" << endl;
			sleepMillis(tempsAction);
		}
		indexAction++;
	}
	
	return;
}

void ActionManager::sleepMillis(int millis) {
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}
