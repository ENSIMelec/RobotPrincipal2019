#include <iostream>
#include <stdlib.h> //The function abs() is in the example code, and it needs stdlib.h to be included.
#include <stdio.h> //The example shows Dynamixel status in sequence by the function printf(). So here stdio.h is needed.

#include "dynamixel_sdk/dynamixel_sdk.h"                                  // Uses Dynamixel SDK library
//All libraries of Dynamixel SDK are linked with the header file dynamixel_sdk.h.

// Control table address
#define ADDR_MX_TORQUE_ENABLE           24                  // Control table address is different in Dynamixel model
#define ADDR_MX_GOAL_POSITION           30
#define ADDR_MX_PRESENT_POSITION        36
#define ADDR_MX_PRESENT_VOLTAGE        	42
#define ADDR_MX_SPEED_VALUE             32
#define ADDR_MX_TORQUE_LIMIT            34
#define ADDR_MX_PRESENT_LOAD            40
//Dynamixel series have their own control tables: Addresses and Byte Length in each items. To control one of the items, its address (and length if necessary) is required. Find your requirements in http://support.robotis.com/.

// Protocol version
#define PROTOCOL_VERSION                1.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define BAUDRATE                        1000000
#define DEVICENAME                      "/dev/ttyACM0"      // Check which port is being used on your controller
                                                            // ex) Windows: "COM1"   Linux: "/dev/ttyUSB0"

#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque



#define DXL_MINIMUM_POSITION_VALUE      300                 // Dynamixel will rotate between this value
#define DXL_MAXIMUM_POSITION_VALUE      900                // and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
#define DXL_MOVING_STATUS_THRESHOLD     5                  // Dynamixel moving status threshold
#define DXL_LOAD_THRESHOLD				800


using namespace std;

int main(int argc, char **argv) {
	if(argc != 5) {
		cout << "Nombre d'arguments invalides. Exit." << endl;
	}
	dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

	int dxl_comm_result = COMM_TX_FAIL;         // Communication result
	uint8_t dxl_error = 0;						// Dynamixel error

	int DXL_ID = atoi(argv[1]);
	int dxl_goal_position = atoi(argv[2]);
	int speed = atoi(argv[3]);
	int force = atoi(argv[4]);

	int sens = 1;

	uint16_t dxl_present_position = 0;          // Present position
	uint16_t dxl_present_load = 0;				// Present position


	if (portHandler->openPort()) {
		printf("Succeeded to open the port!\n");
	}
	else {
		printf("Failed to open the port!\n");
		return 1;
	}
	if (portHandler->setBaudRate(BAUDRATE)) {
		printf("Succeeded to change the baudrate!\n");
	}
	else {
		printf("Failed to change the baudrate!\n");
		return 1;
	}


	cout << "Numéro AX12 ID : " << DXL_ID << endl;


	// Enable DXL Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		cout << "Erreur COMM init Dyna " << DXL_ID << endl;
	}
	else if (dxl_error != 0) {
		cout << "Erreur init Dyna " << DXL_ID << endl;
	}
	else {
		printf("Dynamixel %i has been successfully connected \n", DXL_ID);
	}

	dxl_comm_result = COMM_TX_FAIL;
	dxl_error = 0;
	//Speed
	if(speed != 0) {
		dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_SPEED_VALUE, speed, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS) {
	   		cout << "Erreur COMM vitesse " << DXL_ID << endl;
		}
		else if (dxl_error != 0) {
	   		cout << "Erreur vitesse " << DXL_ID << endl;
		}
	} else {
		cout << "Vitesse ecrite" << endl;
	}

	dxl_comm_result = COMM_TX_FAIL;
	dxl_error = 0;
	//limite le couple
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_LIMIT, force, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		cout << "Erreur COMM limitation couple " << DXL_ID << endl;
	}
	else if (dxl_error != 0) {
		cout << "Erreur limitation couple " << DXL_ID << endl;
	} else {
		cout << "Limite de couple ecrite" << endl;
	}


	dxl_comm_result = COMM_TX_FAIL;
	dxl_error = 0;
    // Write goal position
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_POSITION, dxl_goal_position, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
		cout << "Erreur COMM ecriture angle " << DXL_ID << endl;
    }
    else if (dxl_error != 0)
    {
		cout << "Erreur ecriture angle " << DXL_ID << endl;
    } else {
    	cout << "Angle ecrit" << endl;
    }


    do {
		// Read present position
		dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_MX_PRESENT_POSITION, &dxl_present_position, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS) {
			cout << "Erreur COMM lecture angle " << DXL_ID << endl;
		}
		else if (dxl_error != 0) {
			cout << "Erreur lecture angle " << DXL_ID << endl;
		}

		if(dxl_present_position < dxl_goal_position) {
			sens = 1;
		} else if(dxl_present_position > dxl_goal_position) {
			sens = -1;
		}

		dxl_comm_result = COMM_TX_FAIL;
		dxl_error = 0;
	    // Write goal position
	    if(sens == 1)
	    	dxl_present_position += 2;
		else
			dxl_present_position -= 2;
		
	    dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_POSITION, dxl_present_position, &dxl_error);
	    if (dxl_comm_result != COMM_SUCCESS)
	    {
			cout << "Erreur COMM ecriture angle " << DXL_ID << endl;
	    }
	    else if (dxl_error != 0)
	    {
			cout << "Erreur ecriture angle " << DXL_ID << endl;
	    } 

		dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_MX_PRESENT_LOAD, &dxl_present_load, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS) {
			cout << "Erreur COMM lecture charge " << DXL_ID << endl;
		}
		else if (dxl_error != 0) {
			cout << "Erreur lecture charge " << DXL_ID << endl;
		} else {
				cout << "Charge de l'AX12 : " << dxl_present_load << endl;
		}



	} while(dxl_present_load < DXL_LOAD_THRESHOLD);

	cout << "On est arrivé à la position demandée" << endl;


	cout << "Lecture de la charge" << endl;

	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_MX_PRESENT_LOAD, &dxl_present_load, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		cout << "Erreur COMM lecture charge " << DXL_ID << endl;
	}
	else if (dxl_error != 0) {
		cout << "Erreur lecture charge " << DXL_ID << endl;
	} else {
			cout << "Charge de l'AX12 : " << dxl_present_load << endl;
	}


	cout << "FIN DU PROGRAMME : fermeture du port et désactivation du couple" << endl;


	dxl_comm_result = COMM_TX_FAIL;
	dxl_error = 0;
	// Disable Dynamixel Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		cout << "Erreur COMM close " << DXL_ID << endl;
	}
	else if (dxl_error != 0) {
		cout << "Erreur close " << DXL_ID << endl;
	}
	cout << "Close port AX12" << endl;
	portHandler->closePort();

	return 0;
}