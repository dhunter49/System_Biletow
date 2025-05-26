#pragma once
#include "headers.h"
#include <vector>
#include <unordered_map>

struct Station {
	int stationID;
	std::string stationName;
};

class Route {
protected:
	//variables
	int routeID;
	std::unordered_map<int,Station> stationList; // stationIDOnRoute
public:
	//constructors
	Route();
	Route(int);
};

