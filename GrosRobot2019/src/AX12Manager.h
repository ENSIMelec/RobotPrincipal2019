#ifndef AX12MANAGER_H_INCLUDED
#define AX12MANAGER_H_INCLUDED


#include <iostream>
#include <stdlib.h> //The function abs() is in the example code, and it needs stdlib.h to be included.
#include <stdio.h> //The example shows Dynamixel status in sequence by the function printf(). So here stdio.h is needed.

#include "dynamixel_sdk/dynamixel_sdk.h"                                  // Uses Dynamixel SDK library
//All libraries of Dynamixel SDK are linked with the header file dynamixel_sdk.h.
#include "Clock.h"

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
#define DXL_MOVING_STATUS_THRESHOLD     30                  // Dynamixel moving status threshold


/*
COMM_TX_FAIL
COMM_SUCCESS
*/

class AX12Manager
{

public:
	AX12Manager(int nbAX);
	void close();
	int AX12Action(int numActionneur, int angleAction, int forceAction);

private:
	int initialisation();
	int nbAX12;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler;
};

#endif //AX12MANAGER_H_INCLUDED