#include "headers.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <unordered_map>
#include "Menu.h"
#include "StationManager.h"
#include "Route.h"
#include "DataManager.h"
#include "Trip.h"
#include <Windows.h>
int main() {
	// Allows to display polish characters in console (user should ensure that their console is using font that is compatible with polish chars)
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	ShowScrollBar(GetConsoleWindow(), SB_BOTH, 0); //disables scrollbar // TO MUSI BYĆ W PĘTLI WHILE ŻEBY DZIAŁAŁO, BO JAK UŻYTKOWNIK ZESCROLLUJE MYSZKĄ, TO SIĘ POKAZUJE SCROLLBAR

	// Loads all routes
	DataManager routes;
	try {
		routes.loadAllRoutesFromDatabase();
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
	menu[14] = { 14,"Opcja 14 sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" };
	menu[16] = { 16,"Opcja 16 sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" };

	auto& dm = DataManager::getInstance();

	dm.getTripsByDateAndRouteID({ 24, 6, 2025 },2);
	dm.getTrainByTripID(2);
	dm.getCarsByTrainID(dm.getTrain().getTrainID());

	std::cout<<dm.currentTrain.getFreeSeats(2, 6)<<std::endl<<std::endl;
	for (auto& car : dm.currentCars) {
		dm.getCompartmentsByCarNumber(car.getCarNumber());
		std::cout<<car.getCarNumber()<<" "<< car.getFreeSeats(2,6)<<std::endl;
		for (auto& comp : dm.currentCompartments) {
			std::cout << "    " << comp.getCompartmentNumber() << " " << comp.getFreeSeats(2, 6) << std::endl;
		}
	}
	
	//std::vector<MenuOption> menu = generateMenuList(routes);
	//std::cout<<showMenu("WYBIERZ RELACJĘ (niektóre stacje są ukryte)", menu);


	
	return 0;
}