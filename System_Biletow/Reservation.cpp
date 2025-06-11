#include "Reservation.h"
#include "DataManager.h"
#include <limits>
#include <iostream>

bool Reservation::makeAReservation() {
	DataManager& data = DataManager::getInstance();
	clearScreen();

	// Route
	std::vector<MenuOption> menuRoutes = data.generateMenuListRoutes();
	int routeChoice = showMenu("WYBIERZ RELACJĘ (niektóre stacje są ukryte)", menuRoutes);
	if (routeChoice == -2)
		return false;
	Route chosenRoute = data.getRouteByID(routeChoice);
	chosenRoute.loadStations(true);

	// Station -> Station
	std::vector<MenuOption> menuStationsFrom = chosenRoute.generateMenuListStations(false);
	fromStationNumber = showMenu("Od jakiej stacji?", menuStationsFrom);
	if (fromStationNumber == -2)
		return false;
	std::vector<MenuOption> menuStationsTo = chosenRoute.generateMenuListStations(true, fromStationNumber);
	toStationNumber = showMenu("Do jakiej stacji?", menuStationsTo);
	if (toStationNumber == -2)
		return false;

	// Date
	Date date;
	char temp;
	std::cout << "Podaj datę przejazdu (DD.MM.YYYY): ";
	while (true) {
		std::cin >> date.day >> temp >> date.month >> temp >> date.year;
		if (std::cin.fail() || date.day < 1 || date.month < 1 || date.year < 1 || date.month > 12 || date.day > 31) {
			std::cin.clear(); // Clear the error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
			std::cout << "Nieprawidłowa data. Proszę podać datę w formacie DD.MM.YYYY: ";
		}
		else {
			try {
				data.getTripsByDateAndRouteID(date, chosenRoute.getRouteID());
			}
			catch (const std::runtime_error& e) {
				std::cout << "Błąd: " << e.what() << "\n";
				std::cout << "Podaj datę przejazdu (DD.MM.YYYY): ";
				continue;
			}
			break; // Valid input, exit the loop
		}
	}

	// Trip
	std::vector<MenuOption> menuTrips = data.generateMenuListTrips(fromStationNumber, toStationNumber);
	tripID = showMenu("Wybierz opcje", menuTrips);
	if (tripID == -2)
		return false;
	data.getTrainByTripID(tripID); // Load the train associated with the trip

	// Number of people
	std::cout << "Podaj liczbę osób do zarezerwowania: ";
	while (true) {
		std::cin >> numberOfPeople;
		if (std::cin.fail() || numberOfPeople < 1) {
			std::cin.clear(); // Clear the error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
			std::cout << "Nieprawidłowa liczba osób. Proszę podać liczbę osób: ";
		}
		else {
			break; // Valid input, exit the loop
		}
	}

	// Name
	std::cout << "Podaj imię głównego podróżnego: ";
	while (true) {
		std::cin >> firstName;
		if (firstName.empty()) {
			std::cout << "Imię nie może być puste. Proszę podać imię: ";
		}
		else {
			break; // Valid input, exit the loop
		}
	}

	// Surname
	std::cout << "Podaj nazwisko głównego podróżnego: ";
	while (true) {
		std::cin >> lastName;
		if (lastName.empty()) {
			std::cout << "Nazwisko nie może być puste. Proszę podać nazwisko: ";
		}
		else {
			break; // Valid input, exit the loop
		}
	}

	// Class
	std::vector<MenuOption> classPreference;
	classPreference.push_back(MenuOption{ 1, "Pierwsza klasa" });
	classPreference.push_back(MenuOption{ 0, "Druga klasa" });
	int choice = showMenu("Wybierz klasę", classPreference);
	if (choice == -2)
		return false;
	firstClass = static_cast<bool>(choice);
	
	// Preferences
	isCompartment = getPreferenceValues("Czy chcesz rezerwować miejsce/a w przedziale?");
	byTable = getPreferenceValues("Czy chcesz rezerwować miejsce/a przy stoliku?");
	if(numberOfPeople == 1)
		facingFront = getPreferenceValues("Czy chcesz siedzieć twarzą do kierunku jazdy?");

	// Position
	std::vector<MenuOption> positionOptions;
	positionOptions.push_back(MenuOption{ 1, "Przy oknie" });
	positionOptions.push_back(MenuOption{ 0, "Przy przejściu" });
	positionOptions.push_back(MenuOption{ 2, "Pośrodku" });
	positionOptions.push_back(MenuOption{ -2, "Dowolne" });
	choice = showMenu("Wybierz preferowane miejsce", positionOptions);
	switch (choice)
	{
	case 1: // Przy oknie
		window.isChosen = true;
		window.value = true;
		corridor.isChosen = false;
		corridor.value = false;
		middle.isChosen = false;
		middle.value = false;
		break;
	case 0: // Przy przejściu
		window.isChosen = false;
		window.value = false;
		corridor.isChosen = true;
		corridor.value = true;
		middle.isChosen = false;
		middle.value = false;
		break;
	case 2: // Pośrodku
		window.isChosen = false;
		window.value = false;
		corridor.isChosen = false;
		corridor.value = false;
		middle.isChosen = true;
		middle.value = true;
		break;
	case -2: // Dowolne
		window.isChosen = false;
		window.value = false;
		corridor.isChosen = false;
		corridor.value = false;
		middle.isChosen = false;
		middle.value = false;
		break;
	default:
		throw std::runtime_error("Błąd menu!");
		break;
	}
	return true;
}

Preference Reservation::getPreferenceValues(std::string menuTitle) {
	Preference pref{};
	std::vector<MenuOption> options;
	options.push_back(MenuOption{ 1, "Tak" });
	options.push_back(MenuOption{ 0, "Nie" });
	options.push_back(MenuOption{ -2, "Obojętne" });
	int choice = showMenu(menuTitle, options);
	if (choice == -2) {
		pref.isChosen = false; // User cancelled the menu
	}
	else {
		pref.isChosen = true; // User chose "Tak" or "Nie"
		pref.value = static_cast<bool>(choice);
	}
	return pref;
}

// Tries to find a seat. In DataManager should be already loaded Train.
// Returns true when seat is found and saved into the object variables, returns false if not found.
bool Reservation::findASeat() {
	auto& data = DataManager::getInstance();
	// check if there is enough spots in the entire train, if not instantly returns false value.
	if (data.currentTrain.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople) {
		if (numberOfPeople > 8) {
			// Number of people exceeds all compartments sizes, tries to split users
			std::cout << "Uwaga: Za dużo osób aby zrobić rezerwacje w jednym przedziale. Program traktuje jako osobne rezerwacje!";
			return findASeatSplit();
		}
		data.getCarsByTrainID(data.currentTrain.getTrainID());
		for (auto& carPair : data.currentCars) {
			// check if there is enough spots in a car
			if (carPair.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople) {
				data.getCompartmentsByCarNumber(carPair.getCarNumber());
				for (auto& compartmentPair : data.currentCompartments) {
					// check if there is enough spots in a compartment
					if (compartmentPair.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople 
						&& compartmentPair.getIsFirstClass() == firstClass
						&& !isCompartment.isChosen || isCompartment.value == compartmentPair.getIsAnActualCompartment()) {
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
	Reservation startingProperties(*this); // Saves current properties of the reservation, so it can be restored later if needed.

	// Trying to change the preferences without changing class
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
		*this = startingProperties; // Restore original properties of the reservation.
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

	// Trying to reserve individually for each person in the reservation.
	if (numberOfPeople > 1) {
		std::vector<MenuOption> yesOrNo;
		yesOrNo.push_back(MenuOption{ 1, "Tak, spróbuj zarezerwować miejsca, które są oddalone od siebie" });
		yesOrNo.push_back(MenuOption{ 0, "Nie, zrezygnuj z rezerwacji" });
		std::string menuTitle = "Nie udało się znaleźć wystarczającej liczby miejsc w przedziale. Czy chcesz zarezerwować miejsca oddalone?";
		if (static_cast<bool>(showMenu(menuTitle, yesOrNo))) {
			// User wants to reserve seats individually for each person.
			*this = startingProperties; // Restore original properties of the reservation.
			return findASeatSplit();
		}
		else {
			return false; // User doesn't want to continue with reservation.
		}
	}
}

// Finds a seat for each person in the reservation, if there is no seat for one of the people, returns false.
bool Reservation::findASeatSplit() {
	Reservation person;
	std::vector<Reservation> people;
	for (int i = numberOfPeople; i > 0; i--) {
		person = Reservation(*this);
		person.numberOfPeople = 1;
		if (person.findASeat()) {
			*this = person; // Ensure that the current object gotten the changes from the person object. For example removed preferences.
			numberOfPeople = i + people.size();
			people.push_back(person);
			//person.saveToDatabase(); // Save each person reservation to database
		}
		else {
			// We didn't find a seat for one of the people, we need to restore numberOfPeople and return false.
			numberOfPeople = i + people.size();
			//removeFromDatabaseMultiple(people);
			return false;
		}
	}
}

void Reservation::removeFromDatabaseMultiple(std::vector<Reservation>& reservations) {
	// Removes all reservations from database
	for (auto& reservation : reservations) {
		//reservation.removeFromDatabase();
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

Reservation::Reservation() {}

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