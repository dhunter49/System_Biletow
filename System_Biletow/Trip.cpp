#include "Trip.h"
#include "GlobalConsts.h"
#include <SQLiteCpp/SQLiteCpp.h>

// Will load all trips on a certain date
void initializeTripsByDate(Date date, int routeID) {
	std::vector<Trip> trips;
	SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db,
        "SELECT ID, "
        "CAST(strftime('%H', Time) AS INTEGER) AS hour, "
        "CAST(strftime('%M', Time) AS INTEGER) AS minute "
        "FROM Trips "
        "WHERE RouteID = ? "
        "AND strftime('%d', Date) = ? "
        "AND strftime('%m', Date) = ? "
        "AND strftime('%Y', Date) = ?");

    // Bind parameters
    query.bind(1, routeID);
    query.bind(2, date.day < 10 ? "0" + std::to_string(date.day) : std::to_string(date.day)); // Ensure 2-digit format
    query.bind(3, date.month < 10 ? "0" + std::to_string(date.month) : std::to_string(date.month)); // Ensure 2-digit format
    query.bind(4, std::to_string(date.year));
    if (!query.executeStep())
        throw std::runtime_error("Brak przejazdów danego dnia tej relacji!");
    int currentTripID = query.getColumn(0).getInt();
    Trip currentTrip;
    do {
        currentTrip = Trip(currentTripID);
        currentTrip.date = date;
        currentTrip.loadStations(true);
        currentTrip.schedules[currentTrip.stationList[1].id] = Schedule{
            currentTrip.stationList[1].id,
            Time{query.getColumn(1).getUInt(), query.getColumn(2).getUInt()}, // arrival
            Time{query.getColumn(1).getUInt(), query.getColumn(2).getUInt()}  // departure
        };        
        trips.push_back(currentTrip);
    } while (query.executeStep());
}

Trip::Trip() :tripID(0) {};
Trip::Trip(int tripID) :tripID(tripID) {};

