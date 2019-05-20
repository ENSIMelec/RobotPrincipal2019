#ifndef LIDAR_H
#define LIDAR_H


#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header
#include <stdio.h>
#include <stdlib.h>
#include <ostream>
#include <iostream>
#include <signal.h>
#include <math.h>


using namespace std;
using namespace rp::standalone::rplidar;
#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif





class Lidar
{
public:
	
	//Variable d'arret du scan pour tuer le programme avec Ctrl+c
	 
	
	//Constructeur et destruteur par defaut
	Lidar();
	~Lidar();

	//Verifie que le lidar est present et branche correctement
	bool checkRPLIDARHealth(RPlidarDriver * drv);

	//Lance un scan de 0 a 360 deg et stoque les valeurs dans nodes[]
	int Scan();
	rplidar_response_measurement_node_t nodes[8192];

	void cartesianFromLidar(int*, int*);

	
	
	size_t count;
	int lidarOrder;

	int speed;
	float angle;
	int distance;
	bool detectionActive;
	bool detectionAvant;
	bool detectionArriere;

	

private:
	RPlidarDriver * drv;
	u_result op_result;
	rplidar_response_device_info_t devinfo;
	bool connectSuccess;

	



};

#endif // LIDAR_H