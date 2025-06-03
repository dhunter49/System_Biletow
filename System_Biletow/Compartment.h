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

	// constructors
	Compartment();
	Compartment(int);
	Compartment(Car);
};

