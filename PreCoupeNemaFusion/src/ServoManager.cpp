#include "ServoManager.h"

using namespace std;


// Constructeur par défaut : on stoppe les moteurs
//ServoManager::ServoManager() {
//}
ServoManager::ServoManager(int i2c):
	i2c_servo(i2c)
{

}

void ServoManager::servoAction(int servo, int action){
	uint8_t datas[2];
	uint8_t buf=0;
	uint8_t servoNumber = servo;
	uint8_t servoaction = action;
	datas[0] = servoNumber ;
	datas[1] = servoaction;

	//Debuger
	//printf("donner envoyer ::::::::::: %i,%i data %i,%i\n",servo,action,(int)datas[0],(int)datas[1]);
	int i=1;
	do{
		write(i2c_servo, datas, 2);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		read(i2c_servo,&buf,1);
		cout << "Envoie I2C Servo : " <<i<<endl;
		cout << "Reception Servo : ["<< buf <<"]"<<endl;
		i++;
	}while(buf!='H' && i <= 25);

	//write(i2c_servo, datas, 2);
}
