#include "Route.h"
#include "GlobalConsts.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <queue>
#include <unordered_map>
#include "StationManager.h"

Route::Route() : routeID(1) {}

Route::Route(int routeID) : routeID(routeID) {}

void Route::addStation(int stationNum, Station newStation) {
	if (stationList.count(stationNum) != 0) return;
	stationList[stationNum] = newStation;
}

void Route::loadStations(bool all) {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, all
        ? "SELECT StationNumber, StationID FROM Routes WHERE ID = ? ORDER BY StationNumber"
        : "SELECT StationNumber, StationID FROM Routes WHERE ID = ? AND IsShowing = 1 ORDER BY StationNumber");
    query.bind(1, routeID);

    if (!query.executeStep()) {
        throw std::runtime_error("Couldn't find this route");
    }

    do {
        int stationNum = query.getColumn(0).getInt();
        int stationID = query.getColumn(1).getInt();

        Station station = findInDatabase(stationID);
        addStation(stationNum, station);
    } while (query.executeStep());

}

Route RoutesManager::findRouteByID(int routeID) {
	return routes[routeID];
}

// Used to load all routes from database
void RoutesManager::loadRoutesFromDatabase() {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT ID, StationNumber, StationID FROM Routes WHERE IsShowing = 1 ORDER BY ID, StationNumber");

    if (!query.executeStep()) {
        throw std::runtime_error("No routes found in database!");
    }

    int currentRouteID = query.getColumn(0).getInt();
    Route currentRoute(currentRouteID);

    do {
        int routeID = query.getColumn(0).getInt();
        int stationNum = query.getColumn(1).getInt();
        int stationID = query.getColumn(2).getInt();

        // If we encounter a new route, store the previous one and start a new route
        if (routeID != currentRouteID) {
            routes[currentRouteID] = currentRoute;
            currentRouteID = routeID;
            currentRoute = Route(currentRouteID);
        }

        Station station = findInDatabase(stationID);
        currentRoute.addStation(stationNum, station);
    } while (query.executeStep());

    // Store the last route after the loop ends
    routes[currentRouteID] = currentRoute;
}