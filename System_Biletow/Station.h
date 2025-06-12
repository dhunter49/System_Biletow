#pragma once
#include <string>
#include <unordered_map>

// Struct used to store stations
struct Station {
	int id{};
	std::string name{};
};

// Declaration
Station findInDatabase(int);

