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
	std::vector<Trip> currentTrips;
	Train currentTrain; // Only one train should match with one trip, else throw exception
	std::vector<Car> currentCars;
	std::vector<Compartment> currentCompartments;
	std::vector<Seat> currentSeats;

	static DataManager& getInstance();

	// LOADING FROM DATABASE
	void loadAllRoutesFromDatabase();
	std::vector<MenuOption> generateMenuList();

	void getTripsByDateAndRouteID(Date date, int routeID);
	Trip getTripByID(int tripID);

	void getTrainByTripID(int tripID); // Only one train should match with one trip, else throw exception
	Train getTrain();

	void getCarsByTrainID(std::string trainID);
	Car getCarByNumber(int carNumber);

	void getCompartmentsByCarNumber(int carNumber);
	Compartment getCompartmentByNumber(int compartmentNumber);

	void getSeatsByCompartmentNumber(int compartmentNumber, int carNumber);
	void getFreeSeatsByCompartmentNumber(int compartmentNumber, int carNumber, int stationNumberStart, int stationNumberEnd);
	Seat getSeatByNumber(int seatNumber);
};

