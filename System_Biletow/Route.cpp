#include "Route.h"
#include "GlobalConsts.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <queue>
#include <unordered_map>
#include "StationManager.h"

Route::Route() : routeID(1) {}

Route::Route(int routeID) : routeID(routeID) {}

MenuOption Route::getMenuOptionRoute() {
    std::string optionName{};
    bool first = true;
    for (auto& stationPair : stationList) {
        if (!first) {
            optionName += " - ";
        }
        optionName += stationPair.second.name;
        first = false;
    }
    return MenuOption{ routeID, optionName };
}

void Route::addStation(int stationNum, Station newStation) {
	if (stationList.count(stationNum) != 0) return;
	stationList[stationNum] = newStation;
}

// Loads all stations for route; takes a bool which decides if it should load all stations or just "main" stations.
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

int Route::getRouteID() {
    return routeID;
}

std::map<int, Station> Route::getStationList() {
    return stationList;
}

Station Route::getStation(int stationNum) {
    return stationList[stationNum];
}

void Route::setRouteID(int routeID) {
    this->routeID = routeID;
}

void Route::showInfo() {
    clearScreen();
    loadStations(1);
    int i{};
    std::cout << "Stacje: " << std::endl;
    for (auto station : stationList) {
        std::cout << i << ". " << station.second.name << std::endl;
        i++;
    }
}