#include <SQLiteCpp/SQLiteCpp.h>
#include "Trip.h"
#include "GlobalConsts.h"

Schedule Trip::getSchedule(int stationNum) {
    return schedules[stationNum];
}

std::unordered_map<int, Schedule> Trip::getSchedules() {
    return schedules;
}

Date Trip::getDate() {
    return date;
}

int Trip::getTripID() {
    return tripID;
}

void Trip::setDate(Date newDate) {
    date = newDate;
}

void Trip::setSchedule(int stationNum, Schedule newSchedule) {
    schedules[stationNum] = newSchedule;
}

void Trip::setTripID(int newTripID) {
    tripID = newTripID;
}

// Load schedules based on schedule of first station
void Trip::loadAllOtherSchedules() {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT StationNumber, StationID, ArrivalTime, DepartureTime FROM Routes WHERE ID = ? ORDER BY StationNumber");
    query.bind(1, routeID);

    if (!query.executeStep())
        throw std::runtime_error("Niespodziewany błąd! Wczytany przejazd nie istnieje w relacjach!");

    unsigned int hoursArrival{}, minutesArrival{}, hoursDeparture{}, minutesDeparture{}, additionalMinutesArrival{}, additionalMinutesDeparture{};
    int stationNum{}, stationID{};
    while (query.executeStep()) {
        stationNum = query.getColumn(0).getInt();
        stationID = query.getColumn(1).getInt();
        additionalMinutesArrival = query.getColumn(2).getUInt();
        additionalMinutesDeparture = query.getColumn(3).getUInt();
        minutesArrival = (schedules[1].arrival.minutes + additionalMinutesArrival) % 60;
        hoursArrival = schedules[1].arrival.hours + additionalMinutesArrival / 60;
        minutesDeparture = (schedules[1].departure.minutes + additionalMinutesDeparture) % 60;
        hoursDeparture = schedules[1].departure.hours + additionalMinutesDeparture / 60;

        schedules[stationNum] = Schedule{
            stationID,
            Time{hoursArrival, minutesArrival},
            Time{hoursDeparture, minutesDeparture}
        };
    }
}

// Get menu option for trip between two stations in format: "StationFrom Time - StationTo Time"
MenuOption Trip::getMenuOptionTrip(int stationNumberStart, int stationNumberEnd) {
    MenuOption out;
    std::string text;
    Schedule from, to;
	from = getSchedule(stationNumberStart);
	to = getSchedule(stationNumberEnd);
	text = getStation(stationNumberStart).name + " " + from.departure.getTimeFormat() + " - " +
		getStation(stationNumberEnd).name + " " + to.arrival.getTimeFormat();
	out.menuText = text;
	out.id = tripID;
	return out;
}

Trip::Trip() :tripID(0) {};
Trip::Trip(int tripID) :tripID(tripID) {};