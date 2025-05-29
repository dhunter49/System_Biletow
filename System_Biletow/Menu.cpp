#include <iostream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include "Menu.h"

// Sets console output text to given color
// ONLY WINDOWS
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Couldn't get console handle");
    }
    if (!SetConsoleTextAttribute(hConsole, color)) {
        throw std::runtime_error("Couldn't set console text color");
    }
}

//Gets the console dimensions
//ONLY WINDOWS
void getConsoleDimensions(int& lines, int& columns) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // Get the handle to the console output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        lines = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    else {
        // Fallback values if the API call fails
        columns = 80;
        lines = 25;
    }
}


// Hides/Shows visibility of text cursor
// true - visible, false - hidden
// ONLY WINDOWS
void setConsoleCursorVisibility(bool visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Clears console screen 
// ONLY WINDOWS
void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

// Sets cursor position to (x, y)
// ONLY WINDOWS
void gotoXY(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { x, y };
    SetConsoleCursorPosition(hConsole, coordScreen);
}

//refreshes menu options, of given menuPage, current selection, and MenuOptions vector
void refreshMenuOptions(int page,int currentSelection , const std::vector<MenuOption> menuOptions) {
    clearScreen();// NA RAZIE
    int lines, columns;
    getConsoleDimensions(lines, columns);
    const int MAX_OPTIONS = 20;
    int spaceLeftForOptions = MAX_OPTIONS;
    size_t i = page * MAX_OPTIONS;
    if (page > 0 && menuOptions[i - 1].menuText.length() > columns) { // if on previous page, the last element was too long, it should be displayed in the next page.
        i--;
        spaceLeftForOptions--;
    }
    for (; i < menuOptions.size() && i<page * MAX_OPTIONS + spaceLeftForOptions; i++) {
        if (menuOptions[i].menuText.length() > columns) {
            if (i == page * MAX_OPTIONS + spaceLeftForOptions - 1) { // the last element shouldn't be displayed if it is too long
                std::cout << std::endl;
                break;
            }
            spaceLeftForOptions -= menuOptions[i].menuText.length()/columns;
        }
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
    if (menuOptions.empty())
        throw std::invalid_argument("Menu options can't be empty");
    int currentSelection{};
    int key = -1;
    int menuPage{};
    setConsoleCursorVisibility(false);
    try {
        clearScreen();
        if (menuTitle == "")
            std::cout << " ========== " << std::endl << std::endl;
        else
            std::cout << " ===== " << menuTitle << " ===== " << std::endl << std::endl;
        refreshMenuOptions(menuPage, currentSelection, menuOptions);
        while (true) {
            gotoXY(0, 2);
            int key = _getch();
            if (key == 0 || key == 224) {
                switch (_getch()) {
                case 72: // up arrow pressed
                    currentSelection = (currentSelection - 1 + menuOptions.size()) % menuOptions.size();
                    refreshMenuOptions(menuPage, currentSelection, menuOptions);
                    break;
                case 80: // down arrow pressed
                    currentSelection = (currentSelection + 1) % menuOptions.size();
                    refreshMenuOptions(menuPage, currentSelection, menuOptions);
                    break;
                case 75: // left arrow pressed
                    currentSelection = 0;
                    menuPage--;//DODAÆ ¯EBY SIÊ ZAPÊTLA£O JAK DOJDZIE DO KONCA, TAK JAK CURRENTSELECTION??
                    refreshMenuOptions(menuPage, currentSelection, menuOptions);
                    break;
                case 77: // right arrow pressed
                    currentSelection = 0;
                    menuPage++;//DODAÆ ¯EBY SIÊ ZAPÊTLA£O JAK DOJDZIE DO KONCA, TAK JAK CURRENTSELECTION??
                    refreshMenuOptions(menuPage, currentSelection, menuOptions);
                    break;
                }
            }
            else if (key == 13) { // ENTER pressed
                setConsoleCursorVisibility(true);
                return menuOptions[currentSelection].id;
            }
            else if (key == 27) { // ESC pressed
                setConsoleCursorVisibility(true);
                return -2;
            }
        }
    }
    catch (...) {
        setConsoleCursorVisibility(true);
        system("CLS");
        setColor(7);
        throw;
    }
}