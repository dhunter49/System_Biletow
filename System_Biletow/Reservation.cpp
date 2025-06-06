#include "Reservation.h"
#include "DataManager.h"

// Tries to find a seat. In DataManager should be already loaded Train.
// Returns true when seat is found and saved into the object variables, returns false if not found.
bool Reservation::findASeat() {
	auto& data = DataManager::getInstance();
	// check if there is enough spots in the entire train, if not instantly returns false value.
	bool isFirstClassPreferenceAProblem = true;
	if (data.currentTrain.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople) {
		data.getCarsByTrainID(data.currentTrain.getTrainID());
		for (auto& carPair : data.currentCars) {
			// check if there is enough spots in a car
			if (carPair.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople) {
				data.getCompartmentsByCarNumber(carPair.getCarNumber());
				for (auto& compartmentPair : data.currentCompartments) {
					// check if there is enough spots in a compartment
					if (compartmentPair.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople && compartmentPair.getIsFirstClass()) {
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
		// creating vector to use in showMenu func
		if (isFirstClassPreferenceAProblem) {
			std::string classToChangeTo;
			if (firstClass)
				classToChangeTo = "drugą";
			else
				classToChangeTo = "pierwszą";
			std::vector<MenuOption> yesOrNo;
			yesOrNo.push_back(MenuOption{ 1, "Tak, zmień klasę na " +  classToChangeTo + "."});
			yesOrNo.push_back(MenuOption{ 0, "Nie, zrezygnuj z rezerwacji" });
			std::string menuTitle = "Pociąg nie ma miejsc w wybranej klasie. Czy chcesz zmienić klasę?";
			// Lets user choose if they want to make a reservation in different class, if so it changes prefered class
			// and calls this method once again.
			if (static_cast<bool>(showMenu(menuTitle, yesOrNo))) {
				firstClass = !firstClass;
				return findASeat();
			}
			else {
				return false;
			}
		}
		else {
			std::vector<MenuOption> yesOrNo;
			yesOrNo.push_back(MenuOption{ 1, "Tak, wybierz dowolne miejsce w pociągu" });
			yesOrNo.push_back(MenuOption{ 0, "Nie, zrezygnuj z rezerwacji" });
			std::string menuTitle = "Pociąg nie ma miejsc spełniających preferecje, lecz są dostępne miejsca. Czy chcesz wyczyścić preferencje?";
			// Lets user choose if they want to make a reservation without initial preferences, if so it removes preferences
			// and calls this method once again.
			if (static_cast<bool>(showMenu(menuTitle, yesOrNo))) {
				facingFront.isChosen = false;
				byTable.isChosen = false;
				window.isChosen = false;
				middle.isChosen = false;
				corridor.isChosen = false;
				return findASeat();
			}
			else {
				return false;
			}
		}
	}
	else {
		std::cout << "Pociąg nie ma wolnych miejsc!\n";
		return false;
	}
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