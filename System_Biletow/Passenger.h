#pragma once
#include "Seat.h"

class Passenger : protected Seat {
protected:
	//variables
	int passengerID;
	int fromStationNum;
	int toStationNum;
	std::string name;
	std::string surname;
	int price;
	int discount; // in percents
public:
	//constructors
	Passenger();
	Passenger(int);
};

