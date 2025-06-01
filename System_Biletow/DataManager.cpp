#include "DataManager.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "GlobalConsts.h"

DataManager* DataManager::instance = nullptr;

DataManager& DataManager::getInstance() {
    if (!instance) {
        instance = new DataManager();
    }
    return *instance;
}

// Loades all routes to a map. Station List in only filled with station's that are supposed to show in menu - skipped unimportant stations.
void DataManager::loadAllRoutesFromDatabase() {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT ID, StationNumber, StationID FROM Routes WHERE IsShowing = 1 ORDER BY ID, StationNumber");

    if (!query.executeStep()) {
        throw std::runtime_error("No routes found in database!");
    }

    int currentRouteID = query.getColumn(0).getInt();
    Route currentRoute(currentRouteID);

    do {
        int routeID = query.getColumn(0).getInt();

        // If we encounter a new route, store the previous one and start a new route
        if (routeID != currentRouteID) {
            routes[currentRouteID] = currentRoute;
            currentRouteID = routeID;
            currentRoute = Route(currentRouteID);
        }

        currentRoute.loadStations(false);
    } while (query.executeStep());

    // Store the last route after the loop ends
    routes[currentRouteID] = currentRoute;
}

// Gets all trips in a vector that are matching routeID and matching inputted date. Supposedly should find one Trip, although not always.
// Returns vector with Trips with matching criteria.
std::vector<Trip> DataManager::getTripsByDateAndRouteID(Date date, int routeID) {
    std::vector<Trip> out; // Output vector
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
        currentTrip = Trip(currentTripID); // Clear an object
        // Set all vars in new obj
        currentTrip.setRouteID(routeID);
        currentTrip.setDate(date);
        currentTrip.loadStations(true);
        currentTrip.setSchedule(1,
            Schedule{
            currentTrip.getStation(1).id,
            Time{query.getColumn(1).getUInt(), query.getColumn(2).getUInt()}, // arrival
            Time{query.getColumn(1).getUInt(), query.getColumn(2).getUInt()}  // departure
            }
        );
        // Adds Trip to vector
        out.push_back(currentTrip);
    } while (query.executeStep());

    for (auto& itTrip : out) {
        itTrip.loadAllOtherSchedules();
    }

    return out;
}
