#include "LanceurManager.h"

using namespace std;


LanceurManager::LanceurManager(int i2c):
	i2c_lanceur(i2c)
{

}

void LanceurManager::lanceurAction(int action, int tempsFrein) {
	uint8_t datas[2];
	uint8_t buf=0;
	datas[0] = action;
	datas[1] = tempsFrein;

	//Debuger
	//printf("donner envoyer ::::::::::: %i,%i data %i,%i\n",servo,action,(int)datas[0],(int)datas[1]);
	int i=1;
	do{
		write(i2c_lanceur, datas, 2);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		read(i2c_lanceur,&buf,1);
		cout << "Envoie I2C Lanceur: " <<i<<endl;
		cout << "Reception Lanceur : ["<< buf <<"]"<<endl;
		i++;
	}while(buf!='H' && i <= 25);
	//write(i2c_lanceur, datas, 2);
	//wiringPiI2CWrite(i2c_lanceur, datas, 2);
}
