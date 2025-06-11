#include "headers.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <unordered_map>
#include "Menu.h"
#include "StationManager.h"
#include "Route.h"
#include "DataManager.h"
#include "Trip.h"
#include "Reservation.h"
#include <Windows.h>
#include <vector>
int main() {
	// Allows to display polish characters in console (user should ensure that their console is using font that is compatible with polish chars)
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	ShowScrollBar(GetConsoleWindow(), SB_BOTH, 0); //disables scrollbar // TO MUSI BYĆ W PĘTLI WHILE ŻEBY DZIAŁAŁO, BO JAK UŻYTKOWNIK ZESCROLLUJE MYSZKĄ, TO SIĘ POKAZUJE SCROLLBAR

	// Loads all routes
	auto& data = DataManager::getInstance();
	data.loadAllRoutesFromDatabase();

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

	auto& dm = DataManager::getInstance();

	Reservation reservation;
	while (true) {
		clearScreen();
		switch (showMenu("Witaj w systemie rezerwacji biletów kolejowych", mainMenu)) {
		case 0:
			clearScreen();
			reservation.makeAReservation();
			waitForEsc();
			break;
		case 1:
			clearScreen();
			dm.showLookupMenu();
			break;
		default: 
			break;
		}
	}
	return 0;
}