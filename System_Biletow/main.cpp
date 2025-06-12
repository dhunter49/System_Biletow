#include <Windows.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <vector>
#include "Menu.h"
#include "DataManager.h"
#include "Reservation.h"

int main() {
	try {
		// Allows to display polish characters in console (user should ensure that their console is using font that is compatible with polish chars)
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);

		// Loads all routes
		auto& data = DataManager::getInstance();
		data.loadAllRoutesFromDatabase();

		// Launch Main Menu
		std::vector<MenuOption> mainMenu = { { 0, "Zarezerwuj bilet na przejazd" }, { 1, "Pokaż informacje o danym obiekcie" } };
		Reservation reservation;
		while (true) {
			int choice = showMenu("Witaj w systemie Polskich Kolei Politechnicznych (PKP)", mainMenu);
			if (choice == -2) { // ESC pressed
				// Ask user if they want to really quit the program
				std::vector<MenuOption> yesOrNo = { { 0 , "Nie wychodź"}, { 1, "Tak, wyjdź z programu"} };
				int leaveChoice{};
				leaveChoice = showMenu("Czy na pewno chcesz wyjść z programu?", yesOrNo);
				if (leaveChoice == -2) { // ESC pressed - not quiting
					continue;
				}
				else if (static_cast<bool>(leaveChoice))
					return 0;
				else
					continue;
			}
			else {
				switch (choice) {
				case 0:
					// User chosed to make a reservation
					reservation.makeAReservation();
					break;
				case 1:
					// User chosed to show information on smth
					data.showLookupMenu();
					break;
				default:
					// Should never get here
					break;
				}
			}
		}
	} catch (SQLite::Exception& e) {
		// If there is an error with database, show it to user
		std::cerr << "Błąd bazy danych: " << e.what() << std::endl;
		return 1;
	} catch (std::exception& e) {
		// If there is any other error, show it to user
		std::cerr << "Błąd: " << e.what() << std::endl;
		return 2;
	} catch (...) {
		// Catch all other exceptions
		std::cerr << "Nieoczekiwany błąd!" << std::endl;
		return 3;
	}
}