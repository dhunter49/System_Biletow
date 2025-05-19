#pragma once

class Seat
{
protected:
	// variables
	bool isFirstClass{}, isFacingFront{}, isByTable{}, isInCompartment{};
	char locationOfSeating{}; // Where 'w' is window; 'm' is middle; 'c' is corridor
	int seatNumber{};
	// int takenBy[] // 0 if free, passengerID if taken on station number

	// contructors
};
