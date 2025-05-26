#pragma once
#include "Trip.h"

class Train : protected Trip {
protected:
	//variables
	int trainID;
	std::string trainName;
public:
	//constructors
	Train();
	Train(int);
};

