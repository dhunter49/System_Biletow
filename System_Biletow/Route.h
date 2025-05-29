#pragma once
#include "headers.h"
#include <vector>
#include <unordered_map>
#include "StationManager.h"

// Class for routes, stores it's ID and list of stations
class Route {
protected:
	//variables
	int routeID;
	// int here is storing stationNum (always starts at 1 for every route)
	std::unordered_map<int,Station> stationList; 
public:
	//constructors
	void addStation(int stationNum, Station newStation);
	void loadStations(bool all);

	//gettery i settery
	Station getStation(int stationNum);

	Route();
	Route(int);
};

