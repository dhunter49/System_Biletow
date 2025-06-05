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

	std::vector<Trip> currentTrips;
	Train currentTrain; // Only one train should match with one trip, else throw exception
	std::vector<Car> currentCars;
	std::vector<Compartment> currentCompartments;
	std::vector<Seat> currentSeats;

public:
	static DataManager& getInstance();

	void loadAllRoutesFromDatabase();
	std::vector<MenuOption> generateMenuList();

	std::vector<Trip> getTripsByDateAndRouteID(Date date, int routeID);
	Trip getTripByID(int tripID);

	Train getTrainByTripID(int tripID); // Only one train should match with one trip, else throw exception
	Train getTrain();

	std::vector<Car> getCarsByTrainID(std::string trainID);
	Car getCarByNumber(int carNumber);

	std::vector<Compartment> getCompartmentsByCarNumber(int carNumber);
	Compartment getCompartmentByNumber(int compartmentNumber);

	std::vector<Seat> getSeatsByCompartmentNumber(int compartmentNumber, int carNumber);
	Seat getSeatByNumber(int seatNumber);
};

