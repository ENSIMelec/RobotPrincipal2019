#ifndef ACTIONSERVO_H_INCLUDED
#define ACTIONSERVO_H_INCLUDED

#include <iostream>
#include <string>


class ActionServo
{
	public :
		ActionServo(char ty, int n, int a, int f, int t);

		char gettype();
		void settype(char c);

		int getn();
		void setn(int n);

		int geta();
		void seta(int a);

		int getf();
		void setf(int f);
		
		int gett();
		void sett(int t);
		
		void display();

	private :
		char type; 
		int numero;
		int angle;
		int force;
		int time;
		
};

#endif // ACTIONSERVO_H_INCLUDED