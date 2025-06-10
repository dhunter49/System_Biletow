#include "Menu.h"
#include "DataManager.h"
#include <SQLiteCpp/SQLiteCpp.h>

// Displays a menu, user chooses route to see info about
void DataManager::showLookupMenuRoutes() {
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

void DataManager::showLookupMenuTrains() {
    clearScreen();

    // Loads all trains
    DataManager trainsList;
    try {
        trainsList.loadAllTrainsFromDatabase();
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

    std::vector<MenuOption> menu = trainsList.generateMenuListTrains();
    int choice = showMenu("Wybierz poci¹g, o którym chcesz wyœwietliæ informacje", menu);
    if (choice == -2)
        return;
    try {
        if (choice < 0)
            throw std::runtime_error("wyst¹pi³ b³¹d");
        trains[choice].showInfo();
    }
    catch (std::runtime_error& e) {
        std::cerr << "err: " << e.what() << std::endl;
    }
        
}

void DataManager::showLookupMenuPassengers() {
    clearScreen();
    std::cout << "Tutaj info o pasa¿erze";
}

void DataManager::showLookupMenu() {
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

