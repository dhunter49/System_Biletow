#include "Route.h"
#include "GlobalConsts.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <queue>
#include <unordered_map>
#include "StationManager.h"

Route::Route() :routeID(0){};
Route::Route(int routeID, std::unordered_map<int, Station> listOfStations) :routeID(routeID) {
	for (auto& i : listOfStations) {
		stationList[i.first] = i.second;
	}
};

void RoutesManager::loadRoutesFromDatabase(StationManager stations) {
	try {
		SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
		SQLite::Statement query(db, "SELECT ID, StationNumber, StationID FROM Routes");

		int amountOfRoutes{};
		int id{};
		int stationID{};
		std::unordered_map<int, Station> listOfStations;
		while (query.executeStep()) {
			if (id != query.getColumn(0).getInt()) {
				amountOfRoutes++;
				Route route(id, listOfStations);
				routes[id] = route;
				id = query.getColumn(0).getInt();
			}
			stationID = query.getColumn(1).getInt();
			const Station* station = stations.findByID(stationID);
			listOfStations[stationID] = {stationID, station->name};
		}
		amountOfRoutes++;
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