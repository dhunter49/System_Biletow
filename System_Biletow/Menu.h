#pragma once
#include <string> 
#include <vector>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>

struct MenuOption {
    int id;
    std::string menuText;
};


void setColor(int color);
void getConsoleDimensions(int& lines, int& columns);
void setConsoleCursorVisibility(bool visible);
void clearScreen();
void gotoXY(int x, int y);

int showMenu(std::string, const std::vector<MenuOption>);
