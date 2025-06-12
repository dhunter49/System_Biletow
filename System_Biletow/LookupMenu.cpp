#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include <conio.h>
#include "Menu.h"
#include "DataManager.h"
#include "GlobalConsts.h"

// Displays a menu, user chooses a route to see info about - all stations
void DataManager::showLookupMenuRoutes() {
    // Load all routes
	loadAllRoutesFromDatabase();
    std::vector<MenuOption> menu = generateMenuListRoutes();

    while (true) {
        int choice = showMenu("Wybierz trasę, o której chcesz wyświetlić informacje", menu);
        if (choice == -2)
            return;
        routes[choice].showInfo();
    }
}

// Displays a menu, user chooses a train to see info about - car models
void DataManager::showLookupMenuTrains() {
    // Loads all trains
    loadAllTrainsFromDatabase();
    std::vector<MenuOption> menu = generateMenuListTrains();

    while (true) {
        int choice = showMenu("Wybierz pociąg, o którym chcesz wyświetlić informacje", menu);
        if (choice == -2)
            return;
        if (choice < 0)
            continue;
        trains[choice].showInfo();
        waitForEsc();
    }
}

// Displays information about a passenger - based on ticket number
void DataManager::showLookupMenuPassengers() {
    // Check if there are any passengers saved, if not leave this method
	SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
	SQLite::Statement queryCheck(db, "SELECT COUNT(ID) FROM Passengers");
	if (!queryCheck.executeStep() || queryCheck.getColumn(0).getInt() == 0) {
        std::cout << "Brak pasażerów w bazie danych! Kliknij przycisk aby kontynuować..." << std::endl;
        (void)_getch();
        return;
	}

    // There is at least one passenger
    while (true) {
        clearScreen();
		std::cout << "Podaj numer biletu/numer pasażera, o którym chcesz wyświetlić informacje (wpisz \"exit\" aby wyjść): ";
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << "Nie podano numeru biletu/numeru pasażera! Kliknij przycisk aby kontynuować (lub ESC aby anulować)..." << std::endl;
            char key = _getch();
            if (key == 27) { // Escape key
                return;
            }
            continue;
        }
        if(input == "exit") {
            return;
		}
        try {
            SQLite::Statement query(db, "SELECT TripID, CarNumber, SeatNumber, FromStation, ToStation, Name, Surname, Price FROM Passengers WHERE ID = ?");
            query.bind(1, input);

            if (!query.executeStep()) {
                std::cout << "Nie znaleziono pasażera o podanym numerze! Kliknij przycisk aby kontynuować (lub ESC aby anulować)..." << std::endl;
                char key = _getch();
                if (key == 27) { // Escape key
                    return;
                }
                continue;
            }

            int tripID = query.getColumn(0).getInt();
            loadTripByID(tripID);
            Trip trip = getTripByID(tripID);

            std::string fullName = query.getColumn(5).getString() + " " + query.getColumn(6).getString();
            std::string fromStationName = trip.getStation(query.getColumn(3).getInt()).name;
            std::string toStationName = trip.getStation(query.getColumn(4).getInt()).name;
            int carNumber = query.getColumn(1).getInt();
            int seatNumber = query.getColumn(2).getInt();
            float price = query.getColumn(7).getDouble();

            // Show info
            clearScreen();
            std::cout << "Informacje o pasażerze:" << std::endl;
            std::cout << "Numer biletu: " << input << std::endl;
            std::cout << "Imię i nazwisko: " << fullName << std::endl;
            std::cout << "Trasa: " << fromStationName << " - " << toStationName << std::endl;
            std::cout << "Numer wagonu: " << carNumber << ", Numer miejsca: " << seatNumber << std::endl;
            std::cout << "Cena biletu: " << price << " zł" << std::endl<<std::endl;
			std::cout << "Kliknij ESC aby wyjść..." << std::endl;
            waitForEsc();
            return;
        }
        catch (const SQLite::Exception& e) {
            std::cerr << "Błąd bazy danych: " << e.what() << std::endl;
            std::cout << "Kliknij przycisk aby kontynuować..." << std::endl;
            (void)_getch();
            return;
        } catch (const std::exception& e) {
            std::cerr << "Wystąpił błąd: " << e.what() << std::endl;
            std::cout << "Kliknij przycisk aby kontynuować..." << std::endl;
            (void)_getch();
            return;
		}
    }
}

// Shows menu for showing information about smth (a train, a route or a passenger)
void DataManager::showLookupMenu() {
    std::vector<MenuOption> lookupMenu = { {0, "trasie"}, {1, "pociągu"}, {2, "pasażerze"} };
    int lookupChoice{};

    while(true) {
        lookupChoice = showMenu("Pokaż informacje o", lookupMenu);
        switch (lookupChoice) {
        case 0:
            showLookupMenuRoutes();
            break;
        case 1:
            showLookupMenuTrains();
            break;
        case 2:
            showLookupMenuPassengers();
            break;
        case -2:
            lookupChoice = 0;
            return;
        default:
            break;
        }
    }
}

