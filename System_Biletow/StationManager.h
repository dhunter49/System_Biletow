#pragma once
#include <string>
#include <unordered_map>

struct Station {
	int id;
	std::string name;
};

Station findInDatabase(int);

//class StationManager
//{
//public:
//	void loadFromDatabase();
//	const Station* findByID(int id) const;
//private:
//	std::unordered_map<int, Station> stations;
//};

