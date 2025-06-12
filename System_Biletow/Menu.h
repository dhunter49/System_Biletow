#pragma once
#include <string> 
#include <vector>

struct MenuOption {
    int id{};
    std::string menuText{};
};


void setColor(int color);
void getConsoleDimensions(int& lines, int& columns);
void setConsoleCursorVisibility(bool visible);
void clearScreen();
void gotoXY(int x, int y);
void waitForEsc();

int showMenu(std::string, const std::vector<MenuOption>);
