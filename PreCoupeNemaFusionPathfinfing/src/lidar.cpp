#include "lidar.h"
#include <unistd.h>

using namespace std;
using namespace rp::standalone::rplidar;

#define PI 3.14159265


#define MAX_SPEED 500
#define AVG_SPEED 425
#define LOW_SPEED 350

// Angle de détection de 80°
#define ANGLE_ROBALLS 95

// Angle de détection de 95°
#define ANGLE_MILHABOT 95




Lidar::Lidar()
{
	speed = 5;
	detectionActive = true;
	detectionAvant = false;
	detectionArriere = false;
	cout<< "etape 1" << endl;
	connectSuccess = false;
	cout <<"etape 2" << endl;
	drv = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);
	cout<< "etape 3" << endl;

	

	
		if (IS_OK(drv->connect("/dev/ttyUSB0", 115200))) //test avec 256000 sinon
		{
			cout<< "1111111111111111111" << endl;
			cout<< "etape 4" << endl;
			op_result = drv->getDeviceInfo(devinfo);
			cout <<"etape 5" << endl;
			if (IS_OK(op_result))
			{
				cout<< "etape 6" << endl;
				connectSuccess = true;
				cout<< "lidar connected"<<endl;
				//drv->startMotor();

			}
			else
			{
				delete drv;
				drv = NULL;
			}
		}
		else if (IS_OK(drv->connect("/dev/ttyUSB2", 115200))) //test avec 256000 sinon
		{
			cout<< "222222222222222222" << endl;
			cout<< "etape 4" << endl;
			op_result = drv->getDeviceInfo(devinfo);
			cout <<"etape 5" << endl;
			if (IS_OK(op_result))
			{
				cout<< "etape 6" << endl;
				connectSuccess = true;
				cout<< "lidar connected"<<endl;
				//drv->startMotor();

			}
			else
			{
				delete drv;
				drv = NULL;
			}
		}
		

		

		if (!checkRPLIDARHealth(drv)) {
			RPlidarDriver::DisposeDriver(drv);
			drv = NULL;
			cout<<"Rip"<<endl;
		}
		drv->startMotor();
	

	
}

Lidar::~Lidar()
{
	drv->stopMotor();
	cout << "stop motor" << endl;
	RPlidarDriver::DisposeDriver(drv);

	

}




bool Lidar::checkRPLIDARHealth(RPlidarDriver * drv)
{
	u_result     op_result;
	rplidar_response_device_health_t healthinfo;


	op_result = drv->getHealth(healthinfo);
	if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
		printf("RPLidar health status : %d\n", healthinfo.status);
		if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
			fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
			// enable the following code if you want rplidar to be reboot by software
			// drv->reset();
			return false;
		}
		else {
			return true;
		}

	}
	else {
		fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
		return false;
	}
}

int Lidar::Scan()
{
		

	count = _countof(nodes);

	
    // start scan...
    drv->startScan(0,1);

	int i =0;
	
	int etat = 4;
	
	while(1){
		if(speed == 4){
			cout << "C BUGUAY C BUGUAY C BUGUAY C BUGUAY C BUGUAY " << endl;
			break;
		}

	
		int distance_min = 65000;
		op_result = drv->grabScanData(nodes, count,2500);
		if (IS_OK(op_result)) 
		{
			
			drv->ascendScanData(nodes, count);	
			for (int pos = 1; pos < (int)count ; ++pos) 
    		{				
            	if(distance_min > nodes[pos].distance_q2/4.0 && nodes[pos].distance_q2/4.0 != 0.0){
            		distance_min = nodes[pos].distance_q2/4.0;
            		distance = distance_min;
            		angle = nodes[pos].angle_q6_checkbit/128.0f;
            	}         		

            	if(distance_min >= MAX_SPEED){
            		etat = 3;
            	}
            	else if(distance_min >= AVG_SPEED){
            		etat = 2;
            	}	
            	
            	else if(distance_min >= LOW_SPEED){
            		etat = 1;
            	}
            	else {
            		etat = 0;
            	}

            	
    		}
    	}

		if(detectionActive){
			if((angle < (ANGLE_ROBALLS / 2) || angle > (360 - ANGLE_ROBALLS / 2)) && detectionAvant){
                speed = etat;
            }
            else if(angle > (180 - ANGLE_ROBALLS / 2) && angle < (180 + ANGLE_ROBALLS / 2) && detectionArriere){
                speed = etat;
            }
            else{
                speed = 6; // On détecte un truc mais on ne regarde pas dans cette direction
            }
		}
		else{
			speed = 7; // Détection désactivée
		}
    	

    }
	
}


void Lidar::cartesianFromLidar(int *x, int *y)
{
	*x = (int)(distance * cos(angle / 180.0 * PI));
	*y = (int)(distance * sin(angle / 180.0 * PI));
}