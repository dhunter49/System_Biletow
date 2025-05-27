#pragma once
#include "headers.h"
#include <vector>
#include <unordered_map>
#include "StationManager.h"

class Route;

// Struct which stores all Routes in an unordered map
struct RoutesManager {
	std::unordered_map<int, Route> routes;
	void loadRoutesFromDatabase();
};

// Class for routes, stores it's ID and list of stations
class Route {
protected:
	//variables
	int routeID;
	// int here is storing stationNum (always starts at 1 for every route)
	std::unordered_map<int,Station> stationList; 
public:
	//constructors
	Route();
	Route(int, std::unordered_map<int,Station>);
};

