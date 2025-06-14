#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "Station.h"
#include <string>
#include "Menu.h"

// Class for routes, stores its ID and list of stations
class Route {
public:
	// methods
	MenuOption getMenuOptionRoute();
protected:
	// variables
	int routeID{};
	// int here is storing stationNum (always starts at 1 for every route)
	std::map<int,Station> stationList; 
	
public:
	// methods
	void addStation(int stationNum, Station newStation);
	void loadStations(bool all);
	void showInfo();
	std::vector<MenuOption> generateMenuListStations(bool withLast, int from = 0);

	// getters
	int getRouteID();
	std::map<int, Station> getStationList();
	Station getStation(int stationNum);

	// setters
	void setRouteID(int routeID);
	
	// constructors
	Route();
	Route(int);
};

