#include "headers.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <unordered_map>
#include "Menu.h"
#include "StationManager.h"
#include "Route.h"
#include "DataManager.h"
#include "Trip.h"
#include <Windows.h>
#include <vector>
int main() {
	// Allows to display polish characters in console (user should ensure that their console is using font that is compatible with polish chars)
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	ShowScrollBar(GetConsoleWindow(), SB_BOTH, 0); //disables scrollbar // TO MUSI BYĆ W PĘTLI WHILE ŻEBY DZIAŁAŁO, BO JAK UŻYTKOWNIK ZESCROLLUJE MYSZKĄ, TO SIĘ POKAZUJE SCROLLBAR

	//// Loads all routes
	//DataManager routes;
	//try {
	//	routes.loadAllRoutesFromDatabase();
	//}
	//catch (std::exception& e) {
	//	std::cerr << "err: " << e.what() << std::endl;
	//}
	//catch (SQLite::Exception& e) {
	//	std::cerr << "err: " << e.what() << std::endl;
	//}
	//catch (...) {
	//	std::cerr << "nieznany problem" << std::endl;
	//}

	//auto& dm = DataManager::getInstance();

	//dm.getTripsByDateAndRouteID({ 16, 6, 2025 }, 2);
	////dm.getCarsByTrainID(dm.getTrainByTripID(6).getTrainID());
	////dm.getCompartmentsByCarNumber(11);
	////try {
	////	dm.getSeatsByCompartmentNumber(1, 11);
	////}
	////catch (SQLite::Exception& e) {
	////	std::cerr << e.what();
	////}
	//
	//std::vector<MenuOption> menu = routes.generateMenuList();
	//std::cout<<showMenu("WYBIERZ RELACJĘ (niektóre stacje są ukryte)", menu);

	std::vector<MenuOption> mainMenu = { { 0, "Zarezerwuj bilet na przejazd" }, {1, "Pokaż informacje o danym obiekcie" } };
	std::vector<MenuOption> lookupMenu = { {0, "o relacji"}, {1, "o pociągu"}, {2, "o pasażerze"}, {3, "o modelu wagonu"} };
	int lookupChoice{};

	while (true) {
		clearScreen();
		switch (showMenu("Witaj w systemie rezerwacji biletów kolejowych", mainMenu)) {
		case 0:
			clearScreen();
			std::cout << "Tutaj system rezerwacji";
			waitForEsc();
			break;
		case 1:
			clearScreen();
			lookupChoice = showMenu("Pokaż informacje:", lookupMenu);
			if (lookupChoice == -2)
				break;
			clearScreen();
			switch (lookupChoice) {
			case 0:
				std::cout << "Tutaj info o pociągu";
				waitForEsc();
				break;
			case 1:
				std::cout << "Tutaj info o trasie";
				waitForEsc();
				break;
			case 2:
				std::cout << "Tutaj info o pasażerze";
				waitForEsc();
				break;
			case 3:
				std::cout << "Tutaj info o modelu wagonu";
				waitForEsc();
				break;
			default:
				break;
			}
			break;
		default: 
			break;
		}
	}

	
	return 0;
}