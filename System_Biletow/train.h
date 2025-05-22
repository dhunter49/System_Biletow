#pragma once
#include "Trip.h"

class Train : protected Trip {
protected:
	//variables
	int trainID;
public:
	//constructors
	Train();
	Train(int);
};

