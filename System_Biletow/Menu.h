#pragma once
#include <string> 
#include <vector>

// Forward declaration
struct RoutesManager;

struct MenuOption {
    int id;
    std::string menuText;
};

int showMenu(std::string, const std::vector<MenuOption>);

std::vector<MenuOption> generateMenuList(RoutesManager);