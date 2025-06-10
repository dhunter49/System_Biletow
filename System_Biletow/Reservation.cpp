#include "Reservation.h"
#include "DataManager.h"

// Tries to find a seat. In DataManager should be already loaded Train.
// Returns true when seat is found and saved into the object variables, returns false if not found.
bool Reservation::findASeat() {
	auto& data = DataManager::getInstance();
	bool isFirstClassPreferenceAProblem = false;
	// check if there is enough spots in the entire train, if not instantly returns false value.
	if (data.currentTrain.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople) {
		isFirstClassPreferenceAProblem = true;
		data.getCarsByTrainID(data.currentTrain.getTrainID());
		for (auto& carPair : data.currentCars) {
			// check if there is enough spots in a car
			if (carPair.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople) {
				data.getCompartmentsByCarNumber(carPair.getCarNumber());
				for (auto& compartmentPair : data.currentCompartments) {
					// check if there is enough spots in a compartment
					if (compartmentPair.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople && compartmentPair.getIsFirstClass() == firstClass) {
						// As of right now there should be at least numberOfPeople seats with the matching class preference
						isFirstClassPreferenceAProblem = false;
						if (!isCompartment.isChosen || isCompartment.value == compartmentPair.getIsAnActualCompartment()) {
							data.getFreeSeatsByCompartmentNumber(compartmentPair.getCompartmentNumber(), carPair.getCarNumber(), fromStationNumber, toStationNumber);
							for (auto& seatPair : data.currentSeats) {
								// check if seat meets preferences
								if (meetsPreferences(seatPair)) {
									// Saves seat info to an object.
									carNumber = seatPair.getCarNumber();
									seatNumber = seatPair.getSeatNumber();
									tripID = seatPair.getTripID();
									//calculateTicketPrice();
									return true;
								}
							}
						}
					}
				}
			}
		}
		// If we are here, it means that there is no compartment fitting all numberOfPeople with matching preferences.
		if (!isTryingToReserve) {
			// If we are here, it means we are not yet checking for other preferences, so we run procedure of finding seats without matching prefences.
			isTryingToReserve = true;
			if (findASeatWithConflicts()) {
				// We found a seat, and user agreed to continue with reservation without matching preferences.
				return true;
			}
			else {
				// We didn't find a seat or user didn't agree to continue with reservation without matching preferences.
				std::cout << "Pociąg nie ma wolnych miejsc!\n";
				return false;
			}
		}
		else
			// If we are here, we are checking for a seat with changed preferences, but still no seat was found.
			return false; // That's info for findASeatWithConflicts method.
	}
	else {
		// If we are here, it means that there is not enough free seats in the entire train.
		std::cout << "Pociąg nie ma wolnych miejsc!\n";
		return false;
	}

	return false; // Should never get here, backup return value.
}

bool Reservation::findASeatWithConflicts() {
	Reservation startingProperties = *this; // Saves current properties of the reservation, so it can be restored later if needed.

	// If there are multiple people program should treat this differently
	if (numberOfPeople != 1) {

		// Trying to change the preferences
		if (!byTable.isChosen || !isCompartment.isChosen) {
			byTable.isChosen = false;
			isCompartment.isChosen = false;
			if (findASeat()) {
				// There is a compartment fitting all people, but without preferences. 
				// Ask user if they want to continue with reservation without table preference.
				std::vector<MenuOption> yesOrNo;
				yesOrNo.push_back(MenuOption{ 1, "Tak, zarezerwuj mimo wszystko" });
				yesOrNo.push_back(MenuOption{ 0, "Nie, zrezygnuj z rezerwacji" });
				std::string menuTitle = "Nie znaleziono miejsc zgodnie z preferencjami. Czy chcesz zarezerwować miejsce bez preferencji?";
				if (static_cast<bool>(showMenu(menuTitle, yesOrNo))) {
					return true; // Returns true, because there is a compartment fitting all people. Should be already saved in object variables.
				}
				else {
					return false; // User doesn't want to continue with reservation.
				}
			}
			// RESET TO STARTING
		}
	}
	else {
		if (!byTable.isChosen ||
			!isCompartment.isChosen ||
			!window.isChosen ||
			!corridor.isChosen ||
			!middle.isChosen ||
			!facingFront.isChosen) {
			byTable.isChosen = false;
			isCompartment.isChosen = false;
			window.isChosen = false;
			corridor.isChosen = false;
			middle.isChosen = false;
			facingFront.isChosen = false;
			if (findASeat()) {
				std::vector<MenuOption> yesOrNo;
				yesOrNo.push_back(MenuOption{ 1, "Tak, zarezerwuj mimo wszystko" });
				yesOrNo.push_back(MenuOption{ 0, "Nie, zrezygnuj z rezerwacji" });
				std::string menuTitle = "Nie znaleziono miejsc zgodnie z preferencjami. Czy chcesz zarezerwować miejsce bez preferencji?";
				if (static_cast<bool>(showMenu(menuTitle, yesOrNo))) {
					return true; // Returns true, because there is a compartment fitting all people. Should be already saved in object variables.
				}
				else {
					return false; // User doesn't want to continue with reservation.
				}
			}
			// RESET TO STARTING
		}
	}

	// Trying to change the class without changing preferences
	firstClass = !firstClass;
	if (findASeat()) {
		std::vector<MenuOption> yesOrNo;
		yesOrNo.push_back(MenuOption{ 1, "Tak, zarezerwuj mimo wszystko" });
		yesOrNo.push_back(MenuOption{ 0, "Nie, zrezygnuj z rezerwacji" });
		std::string menuTitle = "Nie znaleziono miejsc w danej klasie. Czy chcesz zarezerwować miejsce w innej klasie?";
		if (static_cast<bool>(showMenu(menuTitle, yesOrNo))) {
			return true; // Returns true, because there is a compartment fitting all people. Should be already saved in object variables.
		}
		else {
			return false; // User doesn't want to continue with reservation.
		}
	}

	// Trying to change class and reset preferences
	if (numberOfPeople != 1) {

		// Trying to change the preferences
		if (!byTable.isChosen || !isCompartment.isChosen) {
			byTable.isChosen = false;
			isCompartment.isChosen = false;
			if (findASeat()) {
				// There is a compartment fitting all people, but without preferences. 
				// Ask user if they want to continue with reservation without table preference.
				std::vector<MenuOption> yesOrNo;
				yesOrNo.push_back(MenuOption{ 1, "Tak, zarezerwuj mimo wszystko" });
				yesOrNo.push_back(MenuOption{ 0, "Nie, zrezygnuj z rezerwacji" });
				std::string menuTitle = "Nie znaleziono miejsc zgodnie z preferencjami oraz daną klasą. Czy chcesz zarezerwować miejsce bez preferencji oraz w innej klasie?";
				if (static_cast<bool>(showMenu(menuTitle, yesOrNo))) {
					return true; // Returns true, because there is a compartment fitting all people. Should be already saved in object variables.
				}
				else {
					return false; // User doesn't want to continue with reservation.
				}
			}
		}
		// Split all buyers
		//CODE
	}
	else {
		if (!byTable.isChosen ||
			!isCompartment.isChosen ||
			!window.isChosen ||
			!corridor.isChosen ||
			!middle.isChosen ||
			!facingFront.isChosen) {
			byTable.isChosen = false;
			isCompartment.isChosen = false;
			window.isChosen = false;
			corridor.isChosen = false;
			middle.isChosen = false;
			facingFront.isChosen = false;
			if (findASeat()) {
				std::vector<MenuOption> yesOrNo;
				yesOrNo.push_back(MenuOption{ 1, "Tak, zarezerwuj mimo wszystko" });
				yesOrNo.push_back(MenuOption{ 0, "Nie, zrezygnuj z rezerwacji" });
				std::string menuTitle = "Nie znaleziono miejsc zgodnie z preferencjami oraz daną klasą. Czy chcesz zarezerwować miejsce bez preferencji oraz w innej klasie?";
				if (static_cast<bool>(showMenu(menuTitle, yesOrNo))) {
					return true; // Returns true, because there is a compartment fitting all people. Should be already saved in object variables.
				}
				else {
					return false; // User doesn't want to continue with reservation.
				}
			}
		}
	}
}

// Finds a seat for each person in the reservation, if there is no seat for one of the people, returns false.
bool Reservation::findASeatSplit() {
	Reservation person;
	std::vector<Reservation> people;
	while (numberOfPeople > 0) {
		person = Reservation(*this);
		if (person.findASeat()) {
			people.push_back(person);
			numberOfPeople--; // Decrease number of people, because we found a seat for one person.
			// We found a seat for 1 person, we are not saving it until we ensure all people got their spot.
		}
		else {
			// We didn't find a seat for one of the people, we need to restore numberOfPeople and return false.
			numberOfPeople += people.size();
			return false;
		}
	}
	for (auto& personPair : people) {
		// Save each person reservation to database
		// If all people got their spot
		personPair.saveReservationToDatabase();
	}
}

void Reservation::operator=(const Reservation& obj) {
	isTryingToReserve = obj.isTryingToReserve;
	numberOfPeople = obj.numberOfPeople;
	fromStationNumber = obj.fromStationNumber;
	toStationNumber = obj.toStationNumber;
	firstClass = obj.firstClass;
	isCompartment = obj.isCompartment;
	facingFront = obj.facingFront;
	byTable = obj.byTable;
	window = obj.window;
	middle = obj.middle;
	corridor = obj.corridor;
	firstName = obj.firstName;
	lastName = obj.lastName;
	carNumber = obj.carNumber;
	seatNumber = obj.seatNumber;
	tripID = obj.tripID;
	ticketPrice = obj.ticketPrice;
}

// Checks if preferations declared in object meet seat real values.
bool Reservation::meetsPreferences(Seat& seat) {
	if (!facingFront.isChosen || facingFront.value == seat.getIsFacingFront()
		&& !byTable.isChosen || byTable.value == seat.getIsByTable()
		&& !window.isChosen || window.value == seat.getIsWindow()
		&& !middle.isChosen || middle.value == seat.getIsMiddle()
		&& !corridor.isChosen || corridor.value == seat.getIsCorridor()) {
		return true;
	}
	return false;
}

Reservation::Reservation(const Reservation& obj) {
    isTryingToReserve = obj.isTryingToReserve;
    numberOfPeople = obj.numberOfPeople;
    fromStationNumber = obj.fromStationNumber;
    toStationNumber = obj.toStationNumber;
    firstClass = obj.firstClass;
    isCompartment = obj.isCompartment;
    facingFront = obj.facingFront;
    byTable = obj.byTable;
    window = obj.window;
    middle = obj.middle;
    corridor = obj.corridor;
    firstName = obj.firstName;
    lastName = obj.lastName;
    carNumber = obj.carNumber;
    seatNumber = obj.seatNumber;
    tripID = obj.tripID;
    ticketPrice = obj.ticketPrice;
}