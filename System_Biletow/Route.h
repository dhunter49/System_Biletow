#pragma once
#include "headers.h"
#include <vector>
#include <unordered_map>
#include "StationManager.h"

class Route;

struct RoutesManager {
	std::unordered_map<int, Route> routes;
	void loadRoutesFromDatabase(StationManager stations);
};

class Route {
protected:
	//variables
	int routeID;
	std::unordered_map<int,Station> stationList; // stationIDOnRoute
public:
	//constructors
	Route(int, std::unordered_map<int,Station>);
};

