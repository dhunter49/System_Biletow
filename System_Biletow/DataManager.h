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
	std::map<int, Train> trains;

public:
	std::vector<Trip> currentTrips;
	Train currentTrain; // Only one train should match with one trip, else throw exception
	std::vector<Car> currentCars;
	std::vector<Compartment> currentCompartments;
	std::vector<Seat> currentSeats;

	static DataManager& getInstance();

	// LOADING FROM DATABASE
	void loadAllRoutesFromDatabase();
	std::vector<MenuOption> generateMenuListRoutes();
	
	void loadAllTrainsFromDatabase();
	std::vector<MenuOption> generateMenuListTrains();

	void getTripsByDateAndRouteID(Date date, int routeID);
	Trip getTripByID(int tripID);

	void getTrainByTripID(int tripID); // Only one train should match with one trip, else throw exception
	Train getTrain();

	void getCarsByTrainID(std::string trainID);
	Car getCarByNumber(int carNumber);

	void getCompartmentsByCarNumber(int carNumber);
	Compartment getCompartmentByNumber(int compartmentNumber);

	void getSeatsByCompartmentNumber(int compartmentNumber, int carNumber);
	Seat getSeatByNumber(int seatNumber);

	// Lookup menu
	void showLookupMenu();
	void showLookupMenuRoutes();
	void showLookupMenuTrains();
	void showLookupMenuPassengers();
};

