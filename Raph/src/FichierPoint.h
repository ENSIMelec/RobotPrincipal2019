#ifndef FICHIERPOINT_H_INCLUDED
#define FICHIERPOINT_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Point.h"

class FichierPoint {
public :
	static std::vector<Point> readPoints(std::string);
};

#endif // FICHIERPOINT_H_INCLUDED