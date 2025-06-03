#include "Compartment.h"

Compartment::Compartment() : compartmentNumber(0) {};
Compartment::Compartment(int compartmentNumber) :compartmentNumber(compartmentNumber) {};
Compartment::Compartment(Car car) : Car(car) {};

void Compartment::setCompartmentNumber(int newCompartmentNumber) {
	compartmentNumber = newCompartmentNumber;
}

void Compartment::setIsAnActualCompartment(bool is) {
	isAnActualCompartment = is;
}

int Compartment::getCompartmentNumber() {
	return compartmentNumber;
}

bool Compartment::getIsAnActualCompartment() {
	return isAnActualCompartment;
}
