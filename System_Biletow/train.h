#pragma once
#include "Trip.h"

class Train : public Trip {
protected:
	// variables
	std::string trainID;
	std::string trainName;
public:
	// getters
	std::string getTrainID();
	std::string getTrainName();

	// setters
	void setTrainID(std::string newTrainID);
	void setTrainName(std::string newTrainName);

	// methods
	int virtual getTakenSeats(int stationStartNumber, int stationEndNumber);

	// constructors
	Train();
	Train(std::string);
	Train(Trip);
};

