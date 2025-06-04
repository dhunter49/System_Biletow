#pragma once
#include "Car.h"

class Compartment : public Car {
protected:
	// variables
	int compartmentNumber;
	bool isAnActualCompartment;
public:
	// setters
	void setCompartmentNumber(int newCompartmentNumber);
	void setIsAnActualCompartment(bool is);

	// getters
	int getCompartmentNumber();
	bool getIsAnActualCompartment();

	// methods
	int getTakenSeats(int stationStartNumber, int stationEndNumber);
	int getSeatCount(int stationStartNumber, int stationEndNumber);
	int getFreeSeats(int stationStartNumber, int stationEndNumber);

	// constructors
	Compartment();
	Compartment(int);
	Compartment(Car);
};

