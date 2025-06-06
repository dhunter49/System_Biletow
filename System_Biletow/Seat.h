#pragma once
#include "Compartment.h"
#include <map>

class Seat : public Compartment {
protected:
	// variables
	bool isFacingFront{}, isByTable{};
	bool isWindow{}, isMiddle{}, isCorridor{};
	char special{}; // 'b' - for passengers with bikes, 'd' - for passengers with disabilities, TBA
	int seatNumber{};
	std::map<int, int> takenBy; // stationIDOnRoute, PassengerID

	
public:
	// getters
	bool getIsFirstClass();
	bool getIsFacingFront();
	bool getIsByTable();
	bool getIsWindow();
	bool getIsMiddle();
	bool getIsCorridor();
	char getSpecial();
	int getSeatNumber();
	std::map<int, int> getTakenBy();

	// setters
	void setIsFirstClass(bool newIsFirstClass);
	void setIsFacingFront(bool newIsFacingFront);
	void setIsByTable(bool newIsByTable);
	void setIsWindow(bool newIsWindow);
	void setIsMiddle(bool newIsMiddle);
	void setIsCorridor(bool newIsCorridor);
	void setSpecial(char newSpecial);
	void setSeatNumber(int newSeatNumber);
	void setTakenBy(std::map<int, int> newTakenBy);

	// constructors
	Seat();
	Seat(int);
	Seat(Compartment);
};
