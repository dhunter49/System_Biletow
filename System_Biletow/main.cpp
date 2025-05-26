#include "headers.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <unordered_map>
//#include "Route.h"
#include "Menu.h"
#include "StationManager.h"
#include <Windows.h>

const std::string DATABASE_PATH = "database.db3";

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	//try {
	//	SQLite::Database db("database.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	//	SQLite::Statement query1(db, "SELECT * FROM Trains");
	//	while (query1.executeStep()) {
	//		std::cout << "ID: " << query1.getColumn(0) << "; ";
	//		std::cout << "nazwa: " << query1.getColumn(1) << std::endl;
	//	}
	//	SQLite::Statement query(db, "SELECT * FROM TrainSets LEFT JOIN Trains ON TrainSets.TrainID = Trains.ID WHERE TrainSets.TripID IS NOT NULL");
	//	while (query.executeStep()) {
	//		std::cout << std::endl;
	//		std::cout << "kurs: " << query.getColumn(0) << "; ";
	//		std::cout << "ID: " << query.getColumn(1) << "; ";
	//		std::cout << "wagon: " << query.getColumn(2) << std::endl << std::endl;

	//	}
	//}
	//catch (const std::exception e) {
	//	std::cerr << "Blad: " << e.what() << std::endl;
	//}
	
	//try {
	//	SQLite::Database db("database.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	//	/*SQLite::Statement query1(db, "SELECT * FROM Trains");
	//	while (query1.executeStep()) {
	//		std::cout << "ID: " << query1.getColumn(0) << "; ";
	//		std::cout << "nazwa: " << query1.getColumn(1) << std::endl;
	//	}
	//	SQLite::Statement query(db, "SELECT * FROM TrainSets LEFT JOIN Trains ON TrainSets.TrainID = Trains.ID WHERE TrainSets.TripID IS NOT NULL");
	//	while (query.executeStep()) {
	//		std::cout << std::endl;
	//		std::cout << "kurs: " << query.getColumn(0) << "; ";
	//		std::cout << "ID: " << query.getColumn(1) << "; ";
	//		std::cout << "wagon: " << query.getColumn(2) << std::endl << std::endl;

	//	}*/

	//	SQLite::Statement route(db, "SELECT StationNumber, Stations.Name, Distances.Distance FROM Routes LEFT JOIN (Stations LEFT JOIN Distances ON Stations.ID = Distances.StationB) ON Routes.StationID = Stations.ID WHERE Routes.ID = 1 ORDER BY StationNumber");
	//	while (route.executeStep()) {
	//		std::cout << route.getColumn(0) << ". " << route.getColumn(1) << " - " << route.getColumn(2) << "km" << std::endl;
	//	}
	//}
	//catch (const std::exception e) {
	//	std::cerr << "Error: " << e.what() << std::endl;
	//}
	//catch (...) {
	//	std::cerr << "Error: unknown" << std::endl;
	//}

	// Init stations
	StationManager stations;
	stations.loadFromDatabase();

	// Display of initialized stations
	for (int i = 0;i < 45;i++) {
		if (const Station* s = stations.findByID(i)) {
			std::cout << s->name << std::endl;
		}
		else
			std::cout << "Nieznaleziono!\n";
	}

	
	return 0;
}