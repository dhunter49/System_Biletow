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

Station Route::getStation(int stationNum) {
    return stationList[stationNum];
}

void Route::setRouteID(int routeID) {
    this->routeID = routeID;
}