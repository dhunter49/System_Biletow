#include "Route.h"
#include "GlobalConsts.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <queue>
#include <unordered_map>
#include "StationManager.h"

Route::Route(int routeID, std::unordered_map<int, Station> listOfStations) :routeID(routeID) {
	for (auto& i : listOfStations) {
		stationList[i.first] = i.second;
	}
};

void RoutesManager::loadRoutesFromDatabase(StationManager stations) {
	try {
		SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
		SQLite::Statement query(db, "SELECT ID, StationNumber, StationID FROM Routes");

		int id = 1;
		int stationID{}; // Station unique ID
		int stationNum{}; // Station number in list, always starts at 1
		std::unordered_map<int, Station> listOfStations;
		while (query.executeStep()) {
			if (id != query.getColumn(0).getInt()) {
				Route route(id, listOfStations);
				routes[id] = route;
				id = query.getColumn(0).getInt();
			}
			stationNum = query.getColumn(1).getInt();
			stationID = query.getColumn(2).getInt();
			const Station* station = stations.findByID(stationID);
			listOfStations[stationNum] = { stationID, station->name };
		}
		Route route(id, listOfStations);
		routes[id] = route;
	}
	catch (SQLite::Exception& e) {
		throw;
	}
	catch (...) {
		throw;
	}
}