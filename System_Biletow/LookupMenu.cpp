#include "Menu.h"
#include "DataManager.h"
#include <SQLiteCpp/SQLiteCpp.h>

void showLookupMenuRoutes() {
    clearScreen();

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

    auto& dm = DataManager::getInstance();
    std::vector<MenuOption> menu = routes.generateMenuListRoutes();
    std::cout << showMenu("Wybierz trasê, o której chcesz wyœwietliæ informacje", menu);
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

}

void showLookupMenu() {
    std::vector<MenuOption> lookupMenu = { {0, "o trasie"}, {1, "o poci¹gu"}, {2, "o pasa¿erze"} };
    int lookupChoice{};

    do {
        clearScreen();
        lookupChoice = showMenu("Poka¿ informacje:", lookupMenu);
        switch (lookupChoice) {
        case 0:
            std::cout << "Tutaj info o trasie";
            showLookupMenuRoutes();
            break;
        case 1:
            std::cout << "Tutaj info o poci¹gu";
            showLookupMenuTrains();
            break;
        case 2:
            std::cout << "Tutaj info o pasa¿erze";
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

