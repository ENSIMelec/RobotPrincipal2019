#include "SerialCodeurManager.h"

using namespace std;


int arduino;
unsigned int nextTime ;
int count = 1;
int fd ;


SerialCodeurManager::SerialCodeurManager(int init) : initCodeur(init)
{ }

void SerialCodeurManager::Closes()
{
	serialClose(fd);
	cout << "Close serial" << endl;
}

void SerialCodeurManager::Initialisation()
{
	cout << "Initialisation codeur" << endl;

	if ((fd = serialOpen ("/dev/ttyUSB1", 115200)) < 0) //A REMETTRE A 1 PLUS TARD
	{
    	fprintf(stderr, "Unable to open serial device: %s\n", strerror (errno));
		exit(3);
		//return 1 ;
	} else {
		cout << "Serial device is Open" << endl;
	}

	/*if (wiringPiSetup () == -1)
		{
		   		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		    	//return 1 ;
		}*/

	nextTime = millis () + 10 ;
}

void SerialCodeurManager::readAndReset()
{

		char SerieData = ' ';
		char tickd [10000];
		char tickg [10000];
		char temps [100000];
		int g = 0;
		int d = 0;
		int t = 0;

		memset(tickd, ' ', 10000);
		memset(tickg, ' ', 10000);
		memset(temps, ' ', 100000);


	  	if (millis () > nextTime)
		{
			//Debuguer
			//printf ("\nOut: %3d: ", count) ;
		    fflush (stdout);
		    serialPutchar (fd, 'C');
		    nextTime += 10 ;
		    ++count ;
		}

		//delay (10) ;

		while(serialDataAvail(fd)<=0 || (SerieData = serialGetchar(fd))!='?'){
			serialPutchar (fd, 'C');
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			//cout<<"Attente réception codeurs"<<endl;
		}
		//cout << "reception[";
		//SerieData = serialGetchar (fd);
		//cout << SerieData;
		if(SerieData=='?'){
			SerieData = serialGetchar (fd);
			//cout << SerieData;
			while(SerieData !=','){
				tickd[d]=SerieData;
				SerieData = serialGetchar (fd);
				//cout << SerieData;
				d++;
			}
			SerieData = serialGetchar (fd);
			//cout << SerieData;
			while(SerieData !=':'){
				tickg[g]=SerieData;
				SerieData = serialGetchar (fd);
				//cout << SerieData;
				g++;
			}
			SerieData = serialGetchar (fd);
			//cout << SerieData;
			while(SerieData !=';'){
				temps[t]=SerieData;
				SerieData = serialGetchar(fd);
				//cout << SerieData;
				t++;
			}
		}
		//cout << "]"<<endl;
		serialFlush(fd);

		oldLeftTicks = leftTicks;
		oldRightTicks = rightTicks;
		oldTempsLast = tempsLast;
		leftTicks = atoi(tickg);
		rightTicks = atoi(tickd);
		tempsLast = atoi(temps);
		//cout <<"leftTicks :"<<leftTicks <<endl;
		//cout <<"rightTicks :"<<rightTicks <<endl;
		//cout <<"tempsLast :"<<tempsLast <<endl;
		if(isnan(leftTicks)){
			leftTicks=0;
		}
		if(isnan(rightTicks)){
			rightTicks=0;
		}
		if(isnan(tempsLast)){
			tempsLast=0;
		}
}

void SerialCodeurManager::reset()
{
	serialPutchar (fd, 'R');
}
