#pragma once
#include "Trip.h"

class Train : protected Trip {
protected:
	//variables
	std::string trainID;
	std::string trainName;
public:
	//constructors
	Train();
	Train(int);
};

