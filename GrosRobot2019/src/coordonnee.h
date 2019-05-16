#ifndef COORDONNEE_H
#define COORDONNEE_H

#include <iostream>

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>

#include <stdio.h>      /* printf */
#include <math.h>


using namespace std;

class Coordonnee
{
public:
    int x;
    int y;
    Coordonnee(int, int);
    ~Coordonnee();
};

#endif // COORDONNEE_H
