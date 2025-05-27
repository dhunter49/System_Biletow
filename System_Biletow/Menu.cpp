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

// Displays menu interface with set amount of options and set title, return choosen option
// If string is empty ("") it won't print any title
// ONLY WINDOWS
int showMenu(std::string menuTitle, const std::vector<MenuOption> menuOptions) {
    if (menuOptions.empty())
        throw std::invalid_argument("Menu options can't be empty");
    int currentSelection{};
    int key = -1;
    setConsoleCursorVisibility(false);
    try {
        while (true) {
            clearScreen();
            if (menuTitle == "")
                std::cout << " ========== " << std::endl << std::endl;
            else 
                std::cout << " ===== " << menuTitle << " ===== " << std::endl << std::endl;
            for (size_t i = 0; i < menuOptions.size(); i++) {
                if (i == currentSelection) {
                    setColor(12); // sets to red when choosen
                    std::cout << "> " << menuOptions[i].menuText << std::endl;
                    setColor(7); // resets to default color
                }
                else {
                    std::cout << "  " << menuOptions[i].menuText << std::endl;
                }
            }
            int key = _getch();
            if (key == 0 || key == 224) {
                switch (_getch()) {
                case 72: // up arrow pressed
                    currentSelection = (currentSelection - 1 + menuOptions.size()) % menuOptions.size();
                    break;
                case 80: // down arrow pressed
                    currentSelection = (currentSelection + 1) % menuOptions.size();
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