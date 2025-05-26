#include "StationManager.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

void StationManager::loadFromDatabase(){
	try {
		SQLite::Database db("database.db3", SQLite::OPEN_READONLY);
		SQLite::Statement query(db, "SELECT ID, Name FROM Stations");

		while (query.executeStep()) {
			Station station;
			station.id = query.getColumn(0).getInt();
			station.name = query.getColumn(1).getString();

			stations[station.id] = station;
		}
	}
	catch (SQLite::Exception& e) {
		std::cerr << "SQLite error: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Nieznany blad!" << std::endl;
	}
}

const Station* StationManager::findByID(int id) const {
	auto it = stations.find(id);
	if (it != stations.end())
		return &it->second;
	else
		return nullptr;
}