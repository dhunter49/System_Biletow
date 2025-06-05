#include "Reservation.h"
#include "DataManager.h"

bool Reservation::findASeat() {
	auto& data = DataManager::getInstance();
	// check if there is enough spots in the entire train
	if (data.currentTrain.getFreeSeats(fromStationNumber, toStationNumber) != 0) {
		data.getCarsByTrainID(data.currentTrain.getTrainID());
		for (auto& carPair : data.currentCars) {
			// check if there is enough spots in a car
			if (carPair.getFreeSeats(fromStationNumber, toStationNumber) != 0) {
				data.getCompartmentsByCarNumber(carPair.getCarNumber());
				for (auto& compartmentPair : data.currentCompartments) {
					// check if there is enough spots in a compartment
					if (compartmentPair.getFreeSeats(fromStationNumber, toStationNumber) != 0) {
						data.getSeatsByCompartmentNumber(compartmentPair.getCompartmentNumber(), carPair.getCarNumber());
						for (auto& seatPair : data.currentSeats) {
							// check if seat meets preferences
							if (meetsPreferences(seatPair)) {
								carNumber = carPair.getCarNumber();
								seatNumber = seatPair.getSeatNumber();
								tripID = seatPair.getTripID();
								//ticketPrice = getPrice();
								return true;
							}
						}
					}
				}
			}
		}
		std::vector<MenuOption> yesOrNo;
		yesOrNo.push_back(MenuOption{ 1, "Tak" });
		yesOrNo.push_back(MenuOption{ 0, "Nie" });
		showMenu("Pociąg nie ma miejsc spełniającyh preferecje, lecz są dostępne miejsca. Czy chcesz wyczyścić preferencje?", yesOrNo);
	}
	else {
		std::cout << "Pociąg nie ma wolnych miejsc!\n";
		return false;
	}
}

bool Reservation::meetsPreferences(Seat seat) {
	if (!facingFront.isChosen || facingFront.value == seat.getIsFacingFront()
		&& !byTable.isChosen || byTable.value == seat.getIsByTable()
		&& !window.isChosen || window.value == seat.getIsWindow()
		&& !middle.isChosen || middle.value == seat.getIsMiddle()
		&& !corridor.isChosen || corridor.value == seat.getIsCorridor()) {
		return true;
	}
	return false;
}