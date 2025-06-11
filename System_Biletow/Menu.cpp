#include "Menu.h"
#include <vector>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>

const int MAX_OPTIONS_ON_MENU = 20; // represents the maximum amount of options shown on the menu. Adjustable to liking

//Calculates first and last indexes of elements on each page
//ONLY USE IN SHOWMENU FUNCTION
void calculateIndexesOfMenu(int* firstIndexes, int* lastindexes, const std::vector<MenuOption> menuOptions) {
    int lines, columns;
    firstIndexes[0] = 0;
    getConsoleDimensions(lines, columns);
    int spaceLeftForOptions = MAX_OPTIONS_ON_MENU;
    int page{};
    for (int i = 0; i < menuOptions.size(); i++) {
        spaceLeftForOptions -= (static_cast<int>(menuOptions[i].menuText.length())-1 + 2) / columns; // +2 because there is always 2 elements before option" >"
        if (i > firstIndexes[page] + spaceLeftForOptions-1) {
            lastindexes[page] = i - 1;
            firstIndexes[page + 1] = i; // validation not required because there is no way that there wouldn't be next page if last element is too long. It comes from calculation of MAX_PAGES in showMenu function
            page++;
            spaceLeftForOptions = MAX_OPTIONS_ON_MENU - (static_cast<int>(menuOptions[i].menuText.length())-1+2) / columns;// +2 because there is always 2 elements before option" >"
            continue;
        }
        if (i == menuOptions.size() - 1) {
            lastindexes[page] = i;
            break;
        }
    }
}

//refreshes menu options, based on given firstIndex of page, lastIndex of page, currentSeleciton, and menu options
void refreshMenuOptions(int firstIndex,int lastIndex ,int currentSelection, const std::vector<MenuOption> menuOptions) {
    int lines, columns;
    getConsoleDimensions(lines, columns);
    std::string spaces(columns, ' ');
    for (int i = 0; i < MAX_OPTIONS_ON_MENU; i++) {
        gotoXY(0, i+2);
        std::cout << spaces <<std::endl;
    }
    gotoXY(0, 2);
    for (int i=firstIndex; i <=lastIndex; i++) {
        if (i == currentSelection) {
            setColor(12); // sets to red when chosen
            std::cout << "> " << menuOptions[i].menuText << std::endl;
            setColor(7); // resets to default color
        }
        else {
            std::cout << "  " << menuOptions[i].menuText << std::endl;
        }
    }
}

// Displays menu interface with set amount of options and set title, return choosen option
// If string is empty ("") it won't print any title
// ONLY WINDOWS
int showMenu(std::string menuTitle, const std::vector<MenuOption> menuOptions) {
    if (menuOptions.empty()) throw std::invalid_argument("Menu options can't be empty");
    clearScreen();
    int lines, columns;
    getConsoleDimensions(lines, columns);
    int count{};
    for (int i = 0; i < menuOptions.size(); i++) {
        if ((menuOptions[i].menuText.length() - 1+2)/ columns >= MAX_OPTIONS_ON_MENU) throw std::invalid_argument("Menu option exceeds maximum lines available, OptionLines >= MAX_OPTIONS_ON_MENU");
        count += 1 + (static_cast<int>(menuOptions[i].menuText.length())-1 + 2) / columns; // +2 because there is always 2 elements before option" >"
    }
    const int MAX_PAGES = (count-1) / MAX_OPTIONS_ON_MENU + 1;
    int* firstIndexes = new int[MAX_PAGES] {}; // dynamic table that will store first indexes of elements on each page, index of element represents number of page
    int* lastIndexes = new int[MAX_PAGES] {}; // dynamic table tat will store last indexes of elements on each page, index of element represents number of page
    calculateIndexesOfMenu(firstIndexes,lastIndexes, menuOptions);
    int currentSelection{};
    int key = -1;
    int menuPage{};
    setConsoleCursorVisibility(false);
    try {
        if (menuTitle == "")
            std::cout << " ========== " << std::endl << std::endl;
        else
            std::cout << " ===== " << menuTitle << " ===== " << std::endl << std::endl;
        while (true) {
            if (menuPage<0 || menuPage>=MAX_PAGES) menuPage = 0;
            refreshMenuOptions(firstIndexes[menuPage], lastIndexes[menuPage], currentSelection, menuOptions);
            gotoXY(columns-19, 23);
            if (menuPage >= 999) std::cout << "Strona nr. 999+/";
            else std::cout << "Strona nr. " << std::setw(3) << menuPage + 1 << "/";
            if (MAX_PAGES >= 999) std::cout << "999+";
            else std::cout << MAX_PAGES;
            key = _getch();
            if (key == 0 || key == 224) {
                switch (_getch()) {
                case 72: // up arrow pressed
                    currentSelection = firstIndexes[menuPage] + ((currentSelection- firstIndexes[menuPage]) - 1 + (lastIndexes[menuPage] - firstIndexes[menuPage] + 1)) % (lastIndexes[menuPage] - firstIndexes[menuPage] + 1);
                    break;
                case 80: // down arrow pressed
                    currentSelection = firstIndexes[menuPage] + ((currentSelection - firstIndexes[menuPage]) + 1) % (lastIndexes[menuPage] - firstIndexes[menuPage] + 1);
                    break;
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
            else if (key == 13) { // ENTER pressed
                setConsoleCursorVisibility(true);
                delete[] firstIndexes;
                delete[] lastIndexes;
                clearScreen();
                return menuOptions[currentSelection].id;
            }
            else if (key == 27) { // ESC pressed
                setConsoleCursorVisibility(true);
                delete[] firstIndexes;
                delete[] lastIndexes;
                clearScreen();
                return -2;
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

