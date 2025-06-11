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
	bool isTryingToReserve = false; // Used to prevent infinite loop in case of no seats available

	// Variables required before making a reservation:
	// WARNING: for 1 person all vars could be changed, but for more than 1 person only: firstClass, isCompartment, byTable.
	//          other preferences won't be taken into consideration.
	int numberOfPeople = 9;
	int fromStationNumber = 2;
	int toStationNumber = 6;
	bool firstClass{};
	Preference isCompartment{};
	Preference facingFront{};
	Preference byTable{};
	// If user chooses to get a certain position, ex. Window, values should look like this:
	// window = true,true, corridor = false, false, middle = false, false
	// only the prefered position should have true in isChosen
	Preference window{};
	Preference middle{};
	Preference corridor{};
	std::string firstName{};
	std::string lastName{};

	// Variables that will be filled after finding a seat in findASeat method:
	int carNumber{};
	int seatNumber{};
	int tripID{};
	float ticketPrice{};
public:
	bool findASeat();
	bool findASeatWithConflicts();
	bool findASeatSplit();
	bool meetsPreferences(Seat& seat);
	float calculateTicketPrice();

	bool makeAReservation();
	Preference getPreferenceValues(std::string menuTitle);

	void saveToDatabase();
	void removeFromDatabaseMultiple(std::vector<Reservation>& reservations);
	void removeFromDatabase();

	// Assign overload operator
	void operator=(const Reservation& obj);

	Reservation(const Reservation& obj);
	Reservation();
};

