#include <vector> 
#include <string>
#include "Route.h"

// Generates vector to use in function show menu based of routes in routes
std::vector<std::string> generateMenuList(RoutesManager routes) {
	std::vector<std::string> out;
	std::string one;
	for (auto& routePair : routes.routes) {
		one = routePair.second.getStringAsMenuOption();
		out.push_back(one);
	}
	return out;
}