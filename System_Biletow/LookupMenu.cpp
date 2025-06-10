#include "Menu.h"
#include "DataManager.h"
#include <SQLiteCpp/SQLiteCpp.h>

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
                throw std::runtime_error("wyst¹pi³ b³¹d");
            throw std::runtime_error("wystąpił błąd");
        trains[choice].showInfo();
        waitForEsc();
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
            waitForEsc();
            break;
        case 2:
            showLookupMenuPassengers();
            waitForEsc();
            break;
        case -2:
            lookupChoice = 0;
            return;
        default:
            break;
        }
    } while (true);
}

