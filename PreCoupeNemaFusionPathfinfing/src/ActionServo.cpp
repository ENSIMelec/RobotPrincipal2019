#include "ActionServo.h"

using namespace std;

ActionServo::ActionServo(char ty, int n, int a, int f, int t)
{
	type = ty;
	numero = n;
	angle = a;
	force = f;
	time = t;
	
}
void ActionServo::display()
{
	cout << numero << ' ' << angle << ' ' << force << ' ' <<
		time <<  endl;
}

char ActionServo::gettype()
{
	return type;
}
void  ActionServo::settype(char c)
{
	type = c; 
}

int ActionServo::getn()
{
	return numero;
}

void ActionServo::setn(int nservo)
{
	numero = nservo;
}
int ActionServo::geta()
{
	return angle;
}

void ActionServo::seta(int aservo)
{
	angle = aservo;
}


int ActionServo::getf()
{
	return force;
}

void ActionServo::setf(int ff)
{
	force = ff;
}


int ActionServo::gett()
{
	return time;
}

void ActionServo::sett(int tt)
{
	time = tt;
}

