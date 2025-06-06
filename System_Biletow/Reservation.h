#pragma once
#include <string>

// Struct will stare freferances with it's value and whether or not a user choosed, if it wasn't chosen
// value should never be taken into consideration
struct Preference {
	bool isChosen;
	bool value;
};

class Reservation {
private:
	// Variables required before making a reservation
	int fromStationNumber{};
	int toStationNumber{};
	bool firstClass{};
	Preference facingFront{};
	Preference byTable{};
	Preference window{};
	Preference middle{};
	Preference corridor{};
	std::string firstName{};
	std::string lastName{};

	// Variables that will be filled after finding a seat in findASeat method
	int carNumber{};
	int seatNumber{};
	int tripID{};
	float ticketPrice{};
public:
	bool findASeat();
	bool meetsPreferences(Seat& seat);
};

