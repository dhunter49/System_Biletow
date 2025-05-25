#include "headers.h"
#include <SQLiteCpp/SQLiteCpp.h>

int main() {
	try {
		SQLite::Database db("database.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
		/*SQLite::Statement query1(db, "SELECT * FROM Trains");
		while (query1.executeStep()) {
			std::cout << "ID: " << query1.getColumn(0) << "; ";
			std::cout << "nazwa: " << query1.getColumn(1) << std::endl;
		}
		SQLite::Statement query(db, "SELECT * FROM TrainSets LEFT JOIN Trains ON TrainSets.TrainID = Trains.ID WHERE TrainSets.TripID IS NOT NULL");
		while (query.executeStep()) {
			std::cout << std::endl;
			std::cout << "kurs: " << query.getColumn(0) << "; ";
			std::cout << "ID: " << query.getColumn(1) << "; ";
			std::cout << "wagon: " << query.getColumn(2) << std::endl << std::endl;

		}*/

		SQLite::Statement route(db, "SELECT StationNumber, Stations.Name, Distances.Distance FROM Routes LEFT JOIN (Stations LEFT JOIN Distances ON Stations.ID = Distances.StationB) ON Routes.StationID = Stations.ID WHERE Routes.ID = 1 ORDER BY StationNumber");
		while (route.executeStep()) {
			std::cout << route.getColumn(0) << ". " << route.getColumn(1) << " - " << route.getColumn(2) << "km" << std::endl;
		}

	}
	catch (const std::exception e) {
		std::cerr << "Blad: " << e.what() << std::endl;
	}

	
	return 0;
}