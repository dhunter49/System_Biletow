#include "Menu.h"
#include "DataManager.h"
#include <SQLiteCpp/SQLiteCpp.h>

void showLookupMenuRoutes() {
    clearScreen();

    // Loads all routes
    DataManager routesList;
    try {
        routesList.loadAllRoutesFromDatabase();
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

    auto& dm = DataManager::getInstance();
    std::vector<MenuOption> menu = routesList.generateMenuListRoutes();
    int choice = showMenu("Wybierz trasê, o której chcesz wyœwietliæ informacje", menu);
    if (choice == -2)
        return;
    Route chosenRoute = routesList.routes[choice];
    chosenRoute.showInfo();
}

void showLookupMenuTrains() {
    clearScreen();

    // Loads all trains
    DataManager trains;
    try {
        trains.loadAllTrainsFromDatabase();
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

    auto& dm = DataManager::getInstance();
    std::vector<MenuOption> menu = trains.generateMenuListTrains();
    std::cout << showMenu("Wybierz poci¹g, o którym chcesz wyœwietliæ informacje", menu);
}

void showLookupMenuPassengers() {
    clearScreen();
    std::cout << "Tutaj info o pasa¿erze";
}

void showLookupMenu() {
    std::vector<MenuOption> lookupMenu = { {0, "o trasie"}, {1, "o poci¹gu"}, {2, "o pasa¿erze"} };
    int lookupChoice{};

    do {
        clearScreen();
        lookupChoice = showMenu("Poka¿ informacje:", lookupMenu);
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
            return;
        default:
            break;
        }
        waitForEsc();
    } while (lookupChoice != -2);
}

