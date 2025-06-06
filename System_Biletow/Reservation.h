#pragma once
#include <string>
#include "Seat.h"

// Struct will stare freferances with it's value and whether or not a user choosed, if it wasn't chosen
// value should never be taken into consideration
struct Preference {
	bool isChosen;
	bool value;
};

class Reservation {
private:
	// Variables required before making a reservation
	int numberOfPeople = 1;
	int fromStationNumber{};
	int toStationNumber{};
	bool firstClass{};
	Preference isCompartment{};
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
	float calculateTicketPrice();
};

