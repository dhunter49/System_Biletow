#pragma once
#include "Train.h"

class Car : public Train {
protected:
	// variables
	int carNumber{}; // NOT an unique identifier - just a number of car in a certain train
	std::string carModel{}; // unique identifier
public:
	// getters 
	int getCarNumber();
	std::string getCarModel();

	// setters
	void setCarNumber(int newCarNumber);
	void setCarModel(std::string newCarModel);

	// methods
	int getTakenSeats(int stationStartNumber, int stationEndNumber);
	int getSeatCount(int stationStartNumber, int stationEndNumber);
	int getFreeSeats(int stationStartNumber, int stationEndNumber);

	// constructors
	Car();
	Car(int);
	Car(Train);
};

