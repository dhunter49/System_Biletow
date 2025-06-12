#include "StationManager.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include "GlobalConsts.h"

// Find a station in database by it's ID, returns struct Station ({stationID, stationName})
// If not found throws an exception
Station findInDatabase(int stationID) {
	SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
	SQLite::Statement query(db, "SELECT Name FROM Stations WHERE ID = ?");
	query.bind(1, stationID); // Replaces '?' with stationID
	if (query.executeStep()) {
		return { stationID, query.getColumn(0).getString() };
	}
	else
		throw std::runtime_error("Numer stacji nie istnieje w bazie danych!");
}