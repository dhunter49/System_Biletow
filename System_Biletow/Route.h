#pragma once
#include "headers.h"
#include <vector>
#include <unordered_map>
#include <map>
#include "StationManager.h"
#include <string>
#include "Menu.h"

class Route;

// Struct which stores all Routes in an unordered map
struct RoutesManager {
	std::unordered_map<int, Route> routes;
	void loadRoutesFromDatabase();
};

// Class for routes, stores it's ID and list of stations
class Route {
public:
	//methods
	MenuOption getMenuOption();
protected:
	//variables
	int routeID;
	// int here is storing stationNum (always starts at 1 for every route)
	std::map<int,Station> stationList; 
public:
	//constructors
	Route();
	Route(int, std::map<int,Station>);
};

