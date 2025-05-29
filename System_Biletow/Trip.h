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
	bool isArrivalTheSameAsDeparture() {
		if (arrival.hours == departure.hours && arrival.minutes == departure.minutes)
			return true;
		else
			return false;
	}
};

class Trip : public Route {
protected:
	//variables
	int tripID;
	std::unordered_map<int, Schedule> schedules; // stationNum
	Date date;
public:
	void printStationSchedule(int stationNum);
	void initializeAllOtherSchedules();

	//gettery i settery
	Schedule getSchedule(int stationNum);
	void setDate(Date newDate);
	void setSchedule(int stationNum,Schedule newSchedule);

	Trip();
	Trip(int);
};

