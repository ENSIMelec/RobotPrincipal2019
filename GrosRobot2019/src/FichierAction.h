#ifndef FICHIERACTION_H_INCLUDED
#define FICHIERACTION_H_INCLUDED

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ActionServo.h"

class FichierAction{
	public :
	static std::vector<ActionServo> readPoints(std::string);
	//static vector<Point> writePoints(string);
	static int countSubstring(const std::string&, const std::string&);
};

#endif //FICHIERACTION_H_INCLUDED