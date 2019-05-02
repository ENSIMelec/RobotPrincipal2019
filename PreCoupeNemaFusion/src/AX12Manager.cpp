#include "AX12Manager.h"

AX12Manager::AX12Manager(int nbAX) : nbAX12(nbAX) {
	initialisation();
}

int AX12Manager::initialisation() {
	// Initialize PortHandler instance
  	// Set the port path
  	// Get methods and members of PortHandlerLinux or PortHandlerWindows
  	portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
	//getPortHandler() function sets port path as DEVICENAME, and prepare an appropriate dynamixel::PortHandler in controller OS automatically.
	// Initialize PacketHandler instance
	// Set the protocol version
	// Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
	packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	//getPacketHandler() function sets the methods for packet construction by choosing the PROTOCOL_VERSION.

	int index_ID = 1;								//ID_AX12
	int dxl_comm_result = COMM_TX_FAIL;             // Communication result
	uint8_t dxl_error = 0;                          // Dynamixel error
	/*
	index variable points the direction to where the Dynamixel should be rotated.
	dxl_comm_result indicates which error has been occurred during packet communication.
	dxl_goal_position stores goal points of Dynamixel rotation.
	dxl_error shows the internal error in Dynamixel.
	dxl_present_position views where now it points out.
	*/

	// Open port
	if (portHandler->openPort())
	{
		printf("Succeeded to open the port!\n");
	}
	else
	{
		printf("Failed to open the port!\n");
		return 1;
	}
	//First, controller opens the port to do serial communication with the Dynamixel. If it fails to open the port, the example will be terminated.

	// Set port baudrate
	if (portHandler->setBaudRate(BAUDRATE))
	{
		printf("Succeeded to change the baudrate!\n");
	}
	else
	{
		printf("Failed to change the baudrate!\n");
		return 1;
	}
	//Secondly, the controller sets the communication BAUDRATE at the port opened previously.
/////////////////////////////////////////IL FAUT INIT TOUS LES AX12 /////////////////////////////////////////////////
	for(index_ID = 1; index_ID <= nbAX12; index_ID++) {
		// Enable DXL Torque
		dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, index_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS)
		{
			//packetHandler->printTxRxResult(dxl_comm_result);
			std::cout << "Erreur COMM init Dyna " << index_ID << std::endl;
		}
		else if (dxl_error != 0)
		{
			//packetHandler->printRxPacketError(dxl_error);
			std::cout << "Erreur init Dyna " << index_ID << std::endl;
		}
		else
		{
			printf("Dynamixel %i has been successfully connected \n", index_ID);
		}
		/*
		As mentioned in the document, above code enables Dynamixel torque to set its status as being ready to move.
		dynamixel::PacketHandler::write1ByteTxRx() function orders to the #index_ID Dynamixel through the port which the portHandler handles,
		writing 1 byte of TORQUE_ENABLE value to ADDR_MX_TORQUE_ENABLE address. Then, it receives the dxl_error.
		The function returns 0 if no communication error has been occurred.
		*/
	}

	return 0;
}

void AX12Manager::close() {
	int dxl_comm_result = COMM_TX_FAIL;
	uint8_t dxl_error = 0;
	for(int index_ID = 1; index_ID <= nbAX12; index_ID++) {
		// Disable Dynamixel Torque
		dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, index_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS)
		{
			//packetHandler->printTxRxResult(dxl_comm_result);
			std::cout << "Erreur COMM close " << index_ID << std::endl;
		}
		else if (dxl_error != 0)
		{
			//packetHandler->printRxPacketError(dxl_error);
			std::cout << "Erreur close " << index_ID << std::endl;
		}
		/*
		The controller frees the Dynamixel to be idle.

		dynamixel::PacketHandler::write1ByteTxRx() function orders to the #index_ID Dynamixel through the port which the portHandler handles,
		writing 1 byte of TORQUE_DISABLE value to ADDR_MX_TORQUE_ENABLE address.
		Then, it receives the dxl_error. The function returns 0 if no communication error has been occurred.
		*/
	}
	std::cout << "Close port AX12" << std::endl;
	portHandler->closePort();
}

int AX12Manager::AX12Action(int numActionneur, int angleAction, int forceAction) {
	timer temps;

	std::cout << "Numéro AX12 ID : " << numActionneur << std::endl;
	int DXL_ID = numActionneur;
	int dxl_goal_position = angleAction;

	int dxl_comm_result = COMM_TX_FAIL;
	uint8_t dxl_error = 0;                          // Dynamixel error
	//uint16_t dxl_present_position = 0;              // Present position
/*
	//Speed
	if(speed != 0)
	{
		dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_SPEED_VALUE, speed, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS)
		{
	   	    packetHandler->printTxRxResult(dxl_comm_result);
		}
		else if (dxl_error != 0)
		{
	    	packetHandler->printRxPacketError(dxl_error);
		}
	}
*/
	//limite le couple
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_LIMIT, forceAction, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		std::cout << "Erreur COMM limitation couple " << DXL_ID << std::endl;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		std::cout << "Erreur limitation couple " << DXL_ID << std::endl;
	}

	temps.restart();
    // Write goal position
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_POSITION, dxl_goal_position, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
      	//packetHandler->printTxRxResult(dxl_comm_result);
		std::cout << "Erreur COMM ecriture angle " << DXL_ID << std::endl;
    }
    else if (dxl_error != 0)
    {
     	//packetHandler->printRxPacketError(dxl_error);
		std::cout << "Erreur ecriture angle " << DXL_ID << std::endl;
    }
    /* //Pas si utile que ça ... Il faudrait plutot faire une boucle "tant qu'on a pas une charge correspondant à la prise de cube dans les pinces ..."
    do {
		// Read present position
		dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_MX_PRESENT_POSITION, &dxl_present_position, &dxl_error);
		if (dxl_comm_result != COMM_SUCCESS)
		{
			packetHandler->printTxRxResult(dxl_comm_result);
		}
		else if (dxl_error != 0)
		{
			packetHandler->printRxPacketError(dxl_error);
		}

		//printf("[ID:%03d] GoalPos:%03d  PresPos:%03d\n", DXL_ID, dxl_goal_position, dxl_present_position);

	} while(abs(dxl_goal_position - dxl_present_position) > DXL_MOVING_STATUS_THRESHOLD && (timeOut == 0 || temps.elapsed_ms() < timeOut));
	*/
	/*
	During while() loop, the controller writes and reads the Dynamixel position through packet transmission/reception(Tx/Rx).

	To continue its rotation, press any key except ESC.

	dynamixel::PacketHandler::write2ByteTxRx() function orders to the #DXL_ID Dynamixel through the port which the portHandler handles,
	writing 2 bytes of dxl_goal_position[index] value to ADDR_MX_GOAL_POSITION address. Then, it receives the dxl_error.
	The function returns 0 if no communication error has been occurred.

	dynamixel::PacketHandler::read2ByteTxRx() functions orders to the #DXL_ID Dynamixel through the port which the portHandler handles,
	requesting 2 bytes of value of ADDR_MX_PRESENT_POSITION address. Then, it receives dxl_present_position and dxl_error.
	The function returns 0 if no communication error has been occurred.

	Reading its present position will be ended when absolute value of (dxl_goal_position[index] - dxl_present_position) becomes smaller then DXL_MOVING_STATUS_THRESHOLD.

	At last, it changes its direction to the counter-wise and waits for extra key input.
	*/
	return temps.elapsed_ms();
}
