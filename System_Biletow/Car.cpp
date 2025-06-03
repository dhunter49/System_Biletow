#include "Car.h"

Car::Car():carNumber(0) {};
Car::Car(int carNumber) : carNumber(carNumber) {};
Car::Car(Train train) : Train(train) {};
 
int Car::getCarNumber() {
	return carNumber;
}

std::string Car::getCarModel() {
	return carModel;
}

void Car::setCarNumber(int newCarNumber) {
	carNumber = newCarNumber;
}

void Car::setCarModel(std::string newCarModel) {
	carModel = newCarModel;
}