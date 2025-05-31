#pragma once
#include "Trip.h"

class Train : public Trip {
protected:
	//variables
	int trainID;
	std::string trainName;
public:
	//constructors
	Train();
	Train(int);
	Train(Trip);
};

