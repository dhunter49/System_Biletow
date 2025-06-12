#include "Route.h"
#include "GlobalConsts.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <queue>
#include <unordered_map>
#include "StationManager.h"
#include <iomanip>
#include <conio.h>

Route::Route() : routeID(1) {}

Route::Route(int routeID) : routeID(routeID) {}

MenuOption Route::getMenuOptionRoute() {
    std::string optionName{};
    bool first = true;
    for (auto& stationPair : stationList) {
        if (!first) {
            optionName += " - ";
        }
        optionName += stationPair.second.name;
        first = false;
    }
    return MenuOption{ routeID, optionName };
}

void Route::addStation(int stationNum, Station newStation) {
	if (stationList.count(stationNum) != 0) return;
	stationList[stationNum] = newStation;
}

// Loads all stations for route; takes a bool which decides if it should load all stations or just "main" stations.
void Route::loadStations(bool all) {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, all
        ? "SELECT StationNumber, StationID FROM Routes WHERE ID = ? ORDER BY StationNumber"
        : "SELECT StationNumber, StationID FROM Routes WHERE ID = ? AND IsShowing = 1 ORDER BY StationNumber");
    query.bind(1, routeID);

    if (!query.executeStep()) {
        throw std::runtime_error("Nie znaleziono połączenia w bazie");
    }

    do {
        int stationNum = query.getColumn(0).getInt();
        int stationID = query.getColumn(1).getInt();

        Station station = findInDatabase(stationID);
        addStation(stationNum, station);
    } while (query.executeStep());

}

int Route::getRouteID() {
    return routeID;
}

std::map<int, Station> Route::getStationList() {
    return stationList;
}

Station Route::getStation(int stationNum) {
    return stationList[stationNum];
}

void Route::setRouteID(int routeID) {
    this->routeID = routeID;
}

// Displays info for lookup menu
void Route::showInfo() {
    clearScreen();
    loadStations(1);
    int i=1;
    int lines, columns;
    getConsoleDimensions(lines, columns);
    int count{};
    const int MAX_STATIONS = 20; //analogical to MAX_OPTIONS_MENU
    for (auto station : stationList) {
        if ((station.second.name.length() - 1 + 2) / columns >= MAX_STATIONS) throw std::invalid_argument("Station exceeds maximum lines available, OptionLines >= MAX_OPTIONS_ON_MENU");
        count += 1 + (static_cast<int>(station.second.name.length()) - 1 + 2) / columns; // +2 because there is always 2 elements before option" >"
    }
    
    const int MAX_PAGES = (count - 1) / MAX_STATIONS + 1;
    int* firstIndexes = new int[MAX_PAGES] {}; // dynamic table that will store first indexes of elements on each page, index of element represents number of page
    int* lastIndexes = new int[MAX_PAGES] {}; // dynamic table tat will store last indexes of elements on each page, index of element represents number of page
    //calculateIndexesOfMenu(firstIndexes, lastIndexes, menuOptions);
    int currentSelection{};
    int key = -1;
    int menuPage{};
    setConsoleCursorVisibility(false);
    try {
        std::cout << "Stacje na wybranej trasie:" << std::endl << std::endl;
        while (true) {
            if (menuPage < 0 || menuPage >= MAX_PAGES) menuPage = 0;
            std::string spaces(columns, ' ');
            for (int i = 0; i < MAX_STATIONS; i++) {
                gotoXY(0, i + 2);
                std::cout << spaces << std::endl;
            }
            gotoXY(0, 2);
            for (int i = MAX_STATIONS*menuPage; i < MAX_STATIONS*(menuPage+1); i++) {
                if (stationList[i+1].name.empty()) {
                    continue;
                }
                std::cout << std::setw(2) << i+1 << ". " << stationList[i+1].name << std::endl;
            }
            gotoXY(columns - 19, 23);
            if (menuPage >= 999) std::cout << "Strona nr. 999+/";
            else std::cout << "Strona nr. " << std::setw(3) << menuPage + 1 << "/";
            if (MAX_PAGES >= 999) std::cout << "999+";
            else std::cout << MAX_PAGES;
            key = _getch();
            if (key == 0 || key == 224) {
                switch (_getch()) {
                case 75: // left arrow pressed
                    menuPage = (menuPage - 1 + MAX_PAGES) % MAX_PAGES;
                    if (menuPage < 0 || menuPage >= MAX_PAGES) menuPage = 0;
                    currentSelection = firstIndexes[menuPage];
                    break;
                case 77: // right arrow pressed
                    menuPage = (menuPage + 1) % MAX_PAGES;
                    if (menuPage < 0 || menuPage >= MAX_PAGES) menuPage = 0;
                    currentSelection = firstIndexes[menuPage];
                    break;
                }
            }
            else if (key == 27) { // ESC pressed
                setConsoleCursorVisibility(true);
                delete[] firstIndexes;
                delete[] lastIndexes;
                return;
            }
        }
        delete[] firstIndexes;
        delete[] lastIndexes;
    }
    catch (...) {
        setConsoleCursorVisibility(true);
        system("CLS");
        setColor(7);
        throw;
    }
}

std::vector<MenuOption> Route::generateMenuListStations(bool withLast, int from) {
    MenuOption oneOption;
    std::vector<MenuOption> out;
    int subtract;
    if (withLast)
        subtract = 0;
    else
        subtract = 1;
    for (int i = from+1; i <= stationList.size() - subtract; i++) {
        oneOption.id = i;
        oneOption.menuText = stationList[i].name;
        out.push_back(oneOption);
    }
    return out;
}