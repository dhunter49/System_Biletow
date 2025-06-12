#pragma once
#include "Trip.h"

class Train : public Trip {
protected:
	// variables
	std::string trainID{};
	int trainIDNumber{};
	std::string trainName{};
public:
	// getters
	std::string getTrainID();
	std::string getTrainName();

	// setters
	void setTrainID(std::string newTrainID);
	void setTrainName(std::string newTrainName);

	// methods
	MenuOption getMenuOptionTrain();
	int getTakenSeats(int stationStartNumber, int stationEndNumber);
	int getSeatCount(int stationStartNumber, int stationEndNumber);
	int getFreeSeats(int stationStartNumber, int stationEndNumber);
	void showInfo();

	// constructors
	Train();
	Train(std::string);
	Train(std::string, int, std::string);
	Train(Trip);
};

