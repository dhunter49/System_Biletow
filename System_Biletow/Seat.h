#pragma once
#include "Compartment.h"
#include <unordered_map>

class Seat : protected Compartment {
protected:
	// variables
	bool isFirstClass{}, isFacingFront{}, isByTable{}, isInCompartment{};
	bool isWindow{}, isMiddle{}, isCorridor{};
	char special{}; // 'b' - for passengers with bikes, 'd' - for passengers with disabilities, TBA
	int seatNumber{};
	std::unordered_map<int, int> takenBy; // stationIDOnRoute, PassengerID

	// constructors
public:
	Seat();
	Seat(int);
};
