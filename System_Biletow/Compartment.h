#pragma once
#include "Car.h"

class Compartment : protected Car {
protected:
	//variables
	int compartmentNumber;
public:
	//constructors
	Compartment();
	Compartment(int);
};

