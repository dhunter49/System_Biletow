#pragma once
#include "Route.h"
#include <unordered_map>

struct Date {
	unsigned int day;
	unsigned int month;
	unsigned int year;
	int getAmountOfDaysInMonth() {
		if (month == 2) { // February
			if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) // leap year
				return 29;
			else
				return 28;
		}
		else if (month == 4 || month == 6 || month == 9 || month == 11) // April, June, September, November
			return 30;
		else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12 )// January, March, May, July, August, October, December
			return 31;
		else  
			return -1; // Invalid month
	}
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
	void loadAllOtherSchedules();
	MenuOption getMenuOptionTrip(int stationNumberStart, int stationNumberEnd);

	// getters
	int getTripID();
	std::unordered_map<int, Schedule> getSchedules();
	Date getDate();
	Schedule getSchedule(int stationNum);

	// setters
	void setDate(Date newDate);
	void setSchedule(int stationNum,Schedule newSchedule);
	void setTripID(int newTripID);

	// constructors
	Trip();
	Trip(int);
};

