#pragma once
#include "Route.h"
#include <unordered_map>

struct Date {
	unsigned int day;
	unsigned int month;
	unsigned int year;
	std::string getDateFormat() {
		std::string stringDay, stringMonth, out;
		if (day < 10) // ensure 2 digit hour
			stringDay = std::to_string(0) + std::to_string(day);
		else
			stringDay = std::to_string(day);
		if (month < 10) // ensure 2 digit minutes
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

