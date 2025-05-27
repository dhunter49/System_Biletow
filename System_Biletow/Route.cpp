#include "Route.h"
#include "GlobalConsts.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <queue>
#include <unordered_map>
#include "StationManager.h"

Route::Route() : routeID(1) {}

Route::Route(int routeID, std::map<int, Station> listOfStations) :routeID(routeID) {
	for (auto& i : listOfStations) {
		stationList[i.first] = i.second;
	}
};

// Returns string in a format "StationA - StationB - ... - StationZ"
MenuOption Route::getMenuOption() {
	std::string out{};
	size_t count{};
	for (auto& stationPair : stationList) {
		out += stationPair.second.name;
		if (++count < stationList.size()) {
			out += " - ";
		}
	}
	return {routeID,out};
}

// Used to load all routes from database
void RoutesManager::loadRoutesFromDatabase() {
	try {
		SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
		SQLite::Statement query(db, "SELECT ID, StationNumber, StationID FROM Routes WHERE IsShowing = 1");

		int stationID{}; // Station unique ID
		int stationNum{}; // Station number in list, always starts at 1, for every route
		std::map<int, Station> listOfStations;

		if (!query.executeStep())
			throw std::runtime_error("No routes found in database!");
		int id = query.getColumn(0).getInt();

		do {
			if (id != query.getColumn(0).getInt()) {
				Route route(id, listOfStations);
				routes[id] = route;
				id = query.getColumn(0).getInt();
				listOfStations.clear();
			}
			stationNum = query.getColumn(1).getInt();
			stationID = query.getColumn(2).getInt();
			Station station = findInDatabase(stationID);
			listOfStations[stationNum] = { stationID, station.name };
		} while (query.executeStep());

		Route route(id, listOfStations);
		routes[id] = route;
	}
	catch (std::exception& e) {
		throw e;
	}
	catch (SQLite::Exception& e) {
		throw e;
	}
	catch (...) {
		throw;
	}
}