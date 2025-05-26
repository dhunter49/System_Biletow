#pragma once
#include "Compartment.h"
#include <unordered_map>

class Seat : protected Compartment {
protected:
	// variables
	bool isFirstClass{}, isFacingFront{}, isByTable{}, isInCompartment{};
	char locationOfSeating{}; // Where 'w' is window; 'm' is middle; 'c' is corridor
	int seatNumber{};
	std::unordered_map<int, int> takenBy; // stationIDOnRoute, PassengerID

	// constructors
public:
	Seat();
	Seat(int);
};
