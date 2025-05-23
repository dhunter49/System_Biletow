#pragma once
#include "Train.h"

class Car : protected Train {
protected:
	//variables
	int carNumber;
public:
	//constructors
	Car();
	Car(int);
};

