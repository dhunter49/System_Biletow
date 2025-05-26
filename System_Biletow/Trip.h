#pragma once
#include "Route.h"
#include <unordered_map>

struct Date {
	unsigned int day;
	unsigned int month;
	unsigned int year;
};

struct Time {
	unsigned int hours;
	unsigned int minutes;
};

struct Schedule{
	int stationID;
	Time arrival;
	Time departure;
};

class Trip : protected Route {
protected:
	//variables
	int tripID;
	std::unordered_map<int, Schedule> schedules; // stationIDOnRoute
	Date date;
public:
	Trip();
	Trip(int);
};

