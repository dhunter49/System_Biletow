#include "Menu.h"
#include "DataManager.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "GlobalConsts.h"

// Displays a menu, user chooses route to see info about
void DataManager::showLookupMenuRoutes() {
    DataManager& data = DataManager::getInstance();
    std::vector<MenuOption> menu = data.generateMenuListRoutes();

    while (true) {
        clearScreen();
        int choice = showMenu("Wybierz trasę, o której chcesz wyświetlić informacje", menu);
        if (choice == -2)
            return;
        data.routes[choice].showInfo();
    }
}

void DataManager::showLookupMenuTrains() {
    // Loads all trains
    DataManager& data = DataManager::getInstance();
    data.loadAllTrainsFromDatabase();

    std::vector<MenuOption> menu = data.generateMenuListTrains();
    while (true) {
        clearScreen();
        int choice = showMenu("Wybierz pociąg, o którym chcesz wyświetlić informacje", menu);
        if (choice == -2)
            return;
        if (choice < 0)
            throw std::runtime_error("Wystąpił błąd!");
        trains[choice].showInfo();
        waitForEsc();
    }
}

void DataManager::showLookupMenuPassengers() {
    while (true) {
        clearScreen();
		std::cout << "Podaj numer biletu/numer pasażera, o którym chcesz wyświetlić informacje: ";
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << "Nie podano numeru biletu/numeru pasażera! Kliknij przycisk aby kontynuować." << std::endl;
            char key = _getch();
            if (key == 27) { // Escape key
                return;
            }
            continue;
        }
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
        SQLite::Statement query(db, "SELECT TripID, CarNumber, SeatNumber, FromStation, ToStation, Name, Surname, Price FROM Passengers WHERE ID = ?");
		query.bind(1, input);

        if(!query.executeStep()) {
            std::cout << "Nie znaleziono pasażera o podanym numerze! Kliknij przycisk aby kontynuować." << std::endl;
            char key = _getch();
            if( key == 27) { // Escape key
                return;
			}
            continue;
		}
        
		DataManager& data = DataManager::getInstance();
		data.loadTripByID(query.getColumn(0).getInt());
		Trip trip = data.getTripByID(query.getColumn(0).getInt());

		std::string fullName = query.getColumn(5).getString() + " " + query.getColumn(6).getString();
        std::string fromStationName = trip.getStation(query.getColumn(3).getInt()).name;
		std::string toStationName = trip.getStation(query.getColumn(4).getInt()).name;
        int carNumber = query.getColumn(1).getInt();
		int seatNumber = query.getColumn(2).getInt();
		float price = query.getColumn(7).getDouble();

        std::cout << "Informacje o pasażerze:" << std::endl;
        std::cout << "Numer biletu: " << input << std::endl;
        std::cout << "Imię i nazwisko: " << fullName << std::endl;
        std::cout << "Trasa: " << fromStationName << " - " << toStationName << std::endl;
        std::cout << "Numer wagonu: " << carNumber << ", Numer miejsca: " << seatNumber << std::endl;
        std::cout << "Cena biletu: " << price << " zł" << std::endl;
		waitForEsc();
        return;
    }
}

void DataManager::showLookupMenu() {
    std::vector<MenuOption> lookupMenu = { {0, "trasie"}, {1, "pociągu"}, {2, "pasażerze"} };
    int lookupChoice{};

    do {
        clearScreen();
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
    } while (true);
}

