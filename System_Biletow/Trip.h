#pragma once
#include "Route.h"
#include <unordered_map>
struct Date;
void initializeTripsByDate(Date, int);

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

class Trip : public Route {
protected:
	//variables
	int tripID;
	std::unordered_map<int, Schedule> schedules;
	Date date;
public:
	friend void initializeTripsByDate(Date date, int routeID);
	Trip();
	Trip(int);
};

