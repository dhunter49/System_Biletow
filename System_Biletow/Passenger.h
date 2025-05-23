#pragma once
#include "Seat.h"

class Passenger : protected Seat {
protected:
	//variables
	int passengerID;
public:
	//constructors
	Passenger();
	Passenger(int);
};

