#pragma once
#include "Train.h"

class Car : protected Train {
protected:
	//variables
	int carNumber;
	std::string carModel;
public:
	//constructors
	Car();
	Car(int);
};

