#pragma once
#include "Route.h"
#include <unordered_map>

struct Date {
	unsigned int day;
	unsigned int month;
	unsigned int year;
	// Get Date in "dd.mm.yyyy"
	std::string getDateFormat() {
		std::string stringDay, stringMonth, out;
		if (day < 10) // ensure 2 digit day
			stringDay = std::to_string(0) + std::to_string(day);
		else
			stringDay = std::to_string(day);
		if (month < 10) // ensure 2 digit month
			stringMonth = std::to_string(0) + std::to_string(month);
		else
			stringMonth = std::to_string(month);

		out = stringDay + "." + stringMonth + "." + std::to_string(year);
		return out;
	}
};

struct Time {
	unsigned int hours;
	unsigned int minutes;
	// Get Time in "hh:mm"
	std::string getTimeFormat() {
		std::string stringHours, stringMinutes, out;
		if (hours < 10) // ensure 2 digit hour
			stringHours = std::to_string(0) + std::to_string(hours);
		else
			stringHours = std::to_string(hours);
		if (minutes<10) // ensure 2 digit minutes
			stringMinutes = std::to_string(0) + std::to_string(minutes);
		else
			stringMinutes = std::to_string(minutes);

		out = stringHours + ":" + stringMinutes;
		return out;
	}
};

struct Schedule {
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
	// variables
	int tripID;
	// int here is storing stationNum (always starts at 1 for every route)
	std::unordered_map<int, Schedule> schedules; // stationNum
	Date date;

public:
	void printStationSchedule(int stationNum);
	void loadAllOtherSchedules();

	// getters
	Schedule getSchedule(int stationNum);
	void setDate(Date newDate);

	// setters
	void setSchedule(int stationNum,Schedule newSchedule);
	void setTripID(int newTripID);

	// constructors
	Trip();
	Trip(int);
};

