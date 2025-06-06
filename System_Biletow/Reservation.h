#pragma once
#include <string>

struct Preference {
	bool isChosen;
	bool value;
};

class Reservation {
private:
	// before reservation
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

	// after reservation
	int carNumber{};
	int seatNumber{};
	int tripID{};
	float ticketPrice{};
public:
	bool findASeat();
	bool meetsPreferences(Seat& seat);
};

