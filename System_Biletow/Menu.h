#pragma once
#include <string> 
#include <vector>

struct MenuOption {
    int id;
    std::string menuText;
};

int showMenu(std::string, const std::vector<MenuOption>);