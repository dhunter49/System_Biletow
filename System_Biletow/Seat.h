#pragma once
#include "Compartment.h"

class Seat : protected Compartment {
protected:
	// variables
	bool isFirstClass{}, isFacingFront{}, isByTable{}, isInCompartment{};
	char locationOfSeating{}; // Where 'w' is window; 'm' is middle; 'c' is corridor
	int seatNumber{};
	// int takenBy[] // 0 if free, passengerID if taken on station number

	// constructors
public:
	Seat();
	Seat(int);
};
