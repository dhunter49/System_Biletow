#pragma once
#include <unordered_map>
#include <vector>
#include "Seat.h"
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
	Route getRouteByID(int routeID);
	
	void loadAllTrainsFromDatabase();
	std::vector<MenuOption> generateMenuListTrains();

	void loadTripsByDateAndRouteID(Date date, int routeID);
	void loadTripByID(int tripID);
	Trip getTripByID(int tripID);
	std::vector<MenuOption> generateMenuListTrips(int stationNumberStart, int stationNumberEnd);

	void loadTrainByTripID(int tripID); // Only one train should match with one trip, else throw exception
	Train getTrain();

	void loadCarsByTrainID(std::string trainID);
	Car getCarByNumber(int carNumber);

	void loadCompartmentsByCarNumber(int carNumber);
	Compartment getCompartmentByNumber(int compartmentNumber);

	void loadSeatsByCompartmentNumber(int compartmentNumber, int carNumber);
	void loadFreeSeatsByCompartmentNumber(int compartmentNumber, int carNumber, int stationNumberStart, int stationNumberEnd);
	Seat getSeatByNumber(int seatNumber);

	// Lookup menu
	void showLookupMenu();
	void showLookupMenuRoutes();
	void showLookupMenuTrains();
	void showLookupMenuPassengers();
};

