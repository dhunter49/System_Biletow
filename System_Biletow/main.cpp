#include "headers.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <unordered_map>
//#include "Route.h"
#include "Menu.h"
#include "StationManager.h"
#include "Route.h"
#include <Windows.h>
int main() {
	// Allows to display polish characters in console (user should ensure that their console is using font that is compatible with polish chars)
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	ShowScrollBar(GetConsoleWindow(), SB_BOTH, 0); //disables scrollbar // TO MUSI BYĆ W PĘTLI WHILE ŻEBY DZIAŁAŁO, BO JAK UŻYTKOWNIK ZESCROLLUJE MYSZKĄ, TO SIĘ POKAZUJE SCROLLBAR

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

	// Loads all routes
	RoutesManager routes;
	try {
		routes.loadRoutesFromDatabase();
	}
	catch (std::exception& e) {
		std::cerr << "err: " << e.what() << std::endl;
	}
	catch (SQLite::Exception& e) {
		std::cerr << "err: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "nieznany problem" << std::endl;
	}

	std::vector<MenuOption> menu; //= generateMenuList(routes);
	for (int i = 0; i < 100000; i++) {
		menu.push_back({ i,"Opcja " +  std::to_string(i)});
	}
	menu[14] = { 16,"Opcja 16 sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" };
	menu[16] = { 16,"Opcja 16 sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" };
	std::cout<<showMenu("WYBIERZ RELACJĘ (niektóre stacje są ukryte)", menu);

	
	return 0;
}