#pragma once
#include <unordered_map>
#include <vector>
#include "Passenger.h"
#include "Route.h"
#include "Car.h"
#include "Compartment.h"
#include "Trip.h"
#include "Train.h"

// This class will hold data of currently proccessed table
class DataManager {
private:
	static DataManager* instance;
	std::unordered_map<int, Route> routes;

public:
	static DataManager& getInstance();

	void loadAllRoutesFromDatabase();
	std::vector<Trip> getTripsByDateAndRouteID(Date date, int routeID);
	Train currentTrain; // Only one train should match with one trip, else throw exception
	std::vector<Car> currentCars;
	std::vector<Compartment> currentCompartments;
	std::vector<Seat> currentSeats;
};

