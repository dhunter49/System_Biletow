#include "Reservation.h"
#include "DataManager.h"
#include "GlobalConsts.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <conio.h>
#include <algorithm>
#include <sstream>

// Used to make a reservation. Returns true if reservation was made successfully, false if user cancelled the reservation or if no seats were found.
// It's a long method, but it handles all the steps of making a reservation, including loading data from the database, showing menus, and getting user input.
bool Reservation::makeAReservation() {
	DataManager& data = DataManager::getInstance();
	data.loadAllRoutesFromDatabase();
	*this = Reservation();

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
	std::string input;
	char temp;
	std::cout << "Podaj datę przejazdu (DD.MM.YYYY) (wpisz \"exit\" aby zrezygnować): ";
	while (true) {
		std::getline(std::cin, input);

		if( input == "exit" || input == "quit" ) {
			return false; // User wants to exit
		}

		// Check basic format requirements
		if (input.empty() || input.size() < 8 || input.size() > 10) {
			clearScreen();
			std::cout << "Nieprawidłowa data. Proszę podać datę w formacie DD.MM.YYYY (wpisz \"exit\" aby zrezygnować): ";
			continue;
		}

		std::istringstream inputStream(input);
		inputStream >> date.day >> temp >> date.month >> temp >> date.year;

		// Check if parsing failed or values are invalid
		if (inputStream.fail() || !inputStream.eof() || temp != '.' ||
			date.day < 1 || date.month < 1 || date.year < 2000 || date.month > 12 ||
			date.day > date.getAmountOfDaysInMonth()) {
			clearScreen();
			std::cout << "Nieprawidłowa data. Proszę podać datę w formacie DD.MM.YYYY (wpisz \"exit\" aby zrezygnować): ";
			continue;
		}

		try {
			data.loadTripsByDateAndRouteID(date, chosenRoute.getRouteID());
			break; // Valid input and successful load, exit the loop
		}
		catch (const std::runtime_error& e) {
			clearScreen();
			std::cout << "Błąd: " << e.what() << "\n";
			std::cout << "Podaj datę przejazdu (DD.MM.YYYY) (wpisz \"exit\" aby zrezygnować): ";
		}
	}

	// Trip
	std::vector<MenuOption> menuTrips = data.generateMenuListTrips(fromStationNumber, toStationNumber);
	tripID = showMenu("Wybierz opcje", menuTrips);
	if (tripID == -2)
		return false;
	data.loadTrainByTripID(tripID); // Load the train associated with the trip

	// Number of people
	std::cout << "Podaj liczbę osób do zarezerwowania: ";
	while (true) {
		std::getline(std::cin, input);
		std::istringstream inputStream(input);
		inputStream >> numberOfPeople;
		if (inputStream.fail() || numberOfPeople < 1) {
			inputStream.clear(); // Clear the error flag
			clearScreen();
			std::cout << "Nieprawidłowa liczba osób. Proszę podać liczbę osób: ";
		}
		else {
			break; // Valid input, exit the loop
		}
	}
	clearScreen();

	// Name
	std::cout << "Podaj imię głównego podróżnego: ";
	while (true) {
		std::getline(std::cin, input);
		if (input.empty()) {
			clearScreen();
			std::cout << "Imię nie może być puste. Proszę podać imię: ";
			continue; // Prompt again
		}
		std::istringstream inputStream(input);
		inputStream >> firstName;
		break; // Valid input, exit the loop
	}
	clearScreen();

	// Surname
	std::cout << "Podaj nazwisko głównego podróżnego: ";
	while (true) {
		std::getline(std::cin, input);
		if (input.empty()) {
			clearScreen();
			std::cout << "Nazwisko nie może być puste. Proszę podać nazwisko: ";
			continue; // Prompt again
		}
		std::istringstream inputStream(input);
		inputStream >> lastName;
		break; // Valid input, exit the loop
	}

	// How to aplly discount
	bool discountChoice{};
	if (numberOfPeople > 1) {
		std::vector<MenuOption> discountYesOrNo;
		discountYesOrNo.push_back(MenuOption{ 1, "Tak, wybierz dla każdego podróżnego z osobna" });
		discountYesOrNo.push_back(MenuOption{ 0, "Nie, wybierz jeden rodzaj zniżki dla całej grupy" });
		int tempChoice = showMenu("Czy chcesz wybrać zniżkę dla każdego podróżnego z osobna?", discountYesOrNo);
		if(tempChoice == -2)
			return false; // User cancelled the menu
		discountChoice = static_cast<bool>(tempChoice);
	}

	// Discount
	for (int i = 1; i <= numberOfPeople; i++) {
		std::vector<MenuOption> discountOptions;
		discountOptions.push_back(MenuOption{ 0, "Brak zniżki (0%)" });
		discountOptions.push_back(MenuOption{ 33, "Nauczyciele (33%)" });
		discountOptions.push_back(MenuOption{ 37, "Dzieci i uczniowie (37%)" });
		discountOptions.push_back(MenuOption{ 37, "Karta Dużej Rodziny (37%)" });
		discountOptions.push_back(MenuOption{ 37, "Karta Polaka (37%)" });
		discountOptions.push_back(MenuOption{ 37, "Emeryci/Renciści (37%)" });
		discountOptions.push_back(MenuOption{ 51, "Studenci/Doktoranci (51%)" });
		discountOptions.push_back(MenuOption{ 78, "Żołnierze (78%)" });
		discountOptions.push_back(MenuOption{ 100, "Dzieci do 4 roku życia (100%)" });
		std::string discountMenuTitle;
		if (numberOfPeople > 1 && discountChoice)
			discountMenuTitle = "Wybierz zniżke dla osoby nr " + std::to_string(i);
		else
			discountMenuTitle = "Wybierz zniżkę";
		int choice = showMenu(discountMenuTitle, discountOptions);
		if(choice == -2)
			return false; // User cancelled the menu
		if (!discountChoice) {
			for (int i = 1;i <= numberOfPeople;i++) {
				float value = static_cast<float>(choice) / 100;
				discounts.push_back(value);
			}
			break;
		}
		float value = static_cast<float>(choice) / 100;
		discounts.push_back(value);
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
	facingFront = getPreferenceValues("Czy chcesz siedzieć twarzą do kierunku jazdy?");

	// Position
	if (numberOfPeople == 1) {
		std::vector<MenuOption> positionOptions;
		positionOptions.push_back(MenuOption{ 1, "Przy oknie" });
		positionOptions.push_back(MenuOption{ 0, "Przy przejściu" });
		positionOptions.push_back(MenuOption{ 2, "Po środku" });
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
		case 2: // Po środku
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
	}
	if(findASeat())
		if (askIfUserAgrees())
			return true; // Reservation was made successfully
		else { // User cancelled the reservation
			// Removes all reservations from database inside of askIfUserAgrees method
			return false;
		}
	else {
		std::cout << "Nie znaleziono miejsc(a). Kliknij przycisk aby kontynouwać..." << std::endl;
		removeFromDatabaseMultiple();
		(void)_getch(); // Wait for user to press a key
		return false;
	}
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
	int numberOfPeopleLeft = numberOfPeople;
	// check if there is enough spots in the entire train, if not instantly returns false value.
	if (data.currentTrain.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople) {
		if (numberOfPeople > 8) {
			// Number of people exceeds all compartments sizes, tries to split users
			std::cout << "Uwaga: Za dużo osób aby zrobić rezerwacje w jednym przedziale. Program traktuje jako osobne rezerwacje! \nKliknij przycisk aby kontynuować...";
			(void)_getch(); // Wait for user to press a key
			return findASeatSplit();
		}
		if (!searchingMessageShown) {
			clearScreen();
			std::cout << "Szukanie miejsc..." << std::endl; // Just in case, if this will take a while, user will know what is happening.
			searchingMessageShown = true; // Show this message only once
		}
		data.loadCarsByTrainID(data.currentTrain.getTrainID());
		for (auto& carPair : data.currentCars) {
			// check if there is enough spots in a car
			if (carPair.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople) {
				data.loadCompartmentsByCarNumber(carPair.getCarNumber());
				for (auto& compartmentPair : data.currentCompartments) {
					// check if there is enough spots in a compartment
					if (compartmentPair.getFreeSeats(fromStationNumber, toStationNumber) >= numberOfPeople 
						&& compartmentPair.getIsFirstClass() == firstClass
						&& (!isCompartment.isChosen || isCompartment.value == compartmentPair.getIsAnActualCompartment())) {
						data.loadFreeSeatsByCompartmentNumber(compartmentPair.getCompartmentNumber(), carPair.getCarNumber(), fromStationNumber, toStationNumber);
						for (auto& seatPair : data.currentSeats) {
							// check if seat meets preferences
							if (meetsPreferences(seatPair)) {
								// Saves seat info to an object.
								carNumber = seatPair.getCarNumber();
								seatNumber = seatPair.getSeatNumber();
								tripID = seatPair.getTripID();
								ticketPrice = calculateTicketPrice();
								saveToDatabase();
								Reservation temp(*this);
								reservations.push_back(temp);
								if (numberOfPeopleLeft > 1) {
									numberOfPeopleLeft--;
									continue;
								}
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
				return false;
			}
		}
		else
			// If we are here, we are checking for a seat with changed preferences, but still no seat was found.
			return false; // That's info for findASeatWithConflicts method.
	}
	else {
		// If we are here, it means that there is not enough free seats in the entire train.
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
				removeFromDatabase();
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
			removeFromDatabase();
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
				removeFromDatabase();
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
	for (int i = numberOfPeople; i > 0; i--) {
		person = Reservation(*this);
		person.numberOfPeople = 1;
		if (person.findASeat()) {
			*this = person; // Ensure that the current object gotten the changes from the person object. For example removed preferences.
			numberOfPeople = i + reservations.size();
			reservations.push_back(person);
		}
		else {
			// We didn't find a seat for one of the people, we need to restore numberOfPeople and return false.
			numberOfPeople = i + reservations.size();
			removeFromDatabaseMultiple();
			return false;
		}
	}
}

void Reservation::removeFromDatabaseMultiple() {
	// Removes all reservations from database
	for (auto& reservation : reservations) {
		reservation.removeFromDatabase();
	}
}

bool Reservation::askIfUserAgrees() {
	int i = 1;
	int key = 0;

	auto& data = DataManager::getInstance();

	setConsoleCursorVisibility(false);
	for (auto& reservationPair : reservations) {
		clearScreen();
		if(reservations.size() == 1 || i == reservations.size())
			std::cout << "Znaleziono miejsca ("<< i<<'/' << reservations.size() << "): (ESC - anuluj rezerwacje, ENTER - zatwierdź rezerwacje)" << std::endl << std::endl;
		else
			std::cout << "Znaleziono miejsca (" << i << '/' << reservations.size() << "): (SPACE - następne, ESC - anuluj rezerwacje, ENTER - zatwierdź rezerwacje)" << std::endl << std::endl;

		std::cout << "   " << reservationPair.firstName << " " << reservationPair.lastName << std::endl;
		std::cout << "Numer biletu: " << reservationPair.reservationID << std::endl;
		std::cout << "Trasa: " << data.getTripByID(reservationPair.tripID).getMenuOptionTrip(reservationPair.fromStationNumber, reservationPair.toStationNumber).menuText << std::endl;
		std::cout << "Klasa: ";
		if (reservationPair.firstClass)
			std::cout << "Pierwsza" << std::endl;
		else
			std::cout << "Druga" << std::endl;
		std::cout << "Wagon: " << reservationPair.carNumber << ", Miejsce: " << reservationPair.seatNumber << std::endl;
		std::cout << "Cena biletu: " << reservationPair.ticketPrice << " zł" << std::endl;

		while (true) {
			key = _getch();
			if (key == 27) { // ESC pressed
				// Remove all reservations from database
				removeFromDatabaseMultiple();
				reservations.clear();
				setConsoleCursorVisibility(true);
				return false; // User cancelled the reservation
			}
			else if (key == 13) { // ENTER pressed
				// All reservations are already saved, so just return true
				reservations.clear();
				setConsoleCursorVisibility(true);
				return true; // User agreed to make a reservation
			}
			else if (key == ' ' && i < reservations.size()) { // SPACE pressed
				break; // Show next reservation
			}
		}
		i++;
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
	reservationID = obj.reservationID;
	discounts = obj.discounts;
}

// Checks if preferations declared in object meet seat real values.
bool Reservation::meetsPreferences(Seat& seat) {
	if ((!facingFront.isChosen || facingFront.value == seat.getIsFacingFront())
		&& (!byTable.isChosen || byTable.value == seat.getIsByTable())
		&& (!window.isChosen || window.value == seat.getIsWindow())
		&& (!middle.isChosen || middle.value == seat.getIsMiddle())
		&& (!corridor.isChosen || corridor.value == seat.getIsCorridor())) {
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
	reservationID = obj.reservationID;
	discounts = obj.discounts;
}

// Adds a new reservation to database and sets unique reservationID
void Reservation::saveToDatabase() {
	SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);
	SQLite::Statement query{ db, "INSERT INTO Passengers (TripID, CarNumber, SeatNumber, FromStation, ToStation, Name, Surname, Price)"
		"VALUES(?, ?, ?, ?, ?, ?, ?, ?)" };

	query.bind(1, tripID);
	query.bind(2, carNumber);
	query.bind(3, seatNumber);
	query.bind(4, fromStationNumber);
	query.bind(5, toStationNumber);
	query.bind(6, firstName);
	query.bind(7, lastName);
	query.bind(8, ticketPrice);
	query.exec();

	reservationID = static_cast<int>(db.getLastInsertRowid());
}

// Removes a reservation from database by reservationID
void Reservation::removeFromDatabase(){
	if( reservationID == 0) {
		return; // Reservation not saved yet, nothing to remove.
	}
	SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READWRITE);
	SQLite::Statement query{ db, "DELETE FROM Passengers "
		"WHERE ID=?" };

	query.bind(1, reservationID);
	query.exec();
}

float Reservation::calculateTicketPrice() {
	float price{}, distance{}, discount{};
	bool isInverted{};

	if(discounts.empty()) {
		// Should never happen, but just in case
		discount = 0;
	}
	else {
		discount = discounts.back();
		discounts.pop_back(); // Remove the last discount, because it was already used
	}

	if (fromStationNumber > toStationNumber) // This should not happen
		std::swap(fromStationNumber, toStationNumber);

	SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
	SQLite::Statement queryC(db, "SELECT Distance "
		"FROM Distances FULL JOIN (Routes FULL JOIN Trips ON Routes.ID = Trips.RouteID) ON Distances.StationA = Routes.StationID "
		"WHERE Routes.StationNumber = 1 "
		"AND Trips.ID = ?");
	queryC.bind(1, tripID);
	queryC.executeStep();
	if (!queryC.getColumn(0).getInt()) {
		isInverted = 1;
		fromStationNumber++;
		toStationNumber++;
	}

	// Calculates total distance between stations
	SQLite::Statement queryDistance(db, "SELECT SUM(Distance) "
		"FROM Distances FULL JOIN(Routes FULL JOIN Trips ON Routes.ID = Trips.RouteID) ON Distances.StationA = Routes.StationID "
		"WHERE Trips.ID = ? "
		"AND Routes.StationNumber >= ? "
		"AND Routes.StationNumber + 1 <= ? "
		"ORDER BY StationNumber");
	queryDistance.bind(1, tripID);
	queryDistance.bind(2, fromStationNumber);
	queryDistance.bind(3, toStationNumber);
	queryDistance.executeStep();
	distance = queryDistance.getColumn(0).getDouble();

	if (isInverted) {
		isInverted = 0;
		fromStationNumber--;
		toStationNumber--;
	}

	// Calculates price based on distance and seat class
	do {
		if (firstClass) {
			SQLite::Statement queryPrice(db, "SELECT FirstClassPrice "
				"FROM Prices "
				"WHERE Distance >= ? "
				"ORDER BY Distance "
				"LIMIT 1");
			queryPrice.bind(1, distance);
			queryPrice.executeStep();
			price += queryPrice.getColumn(0).getDouble();
		}
		else {
			SQLite::Statement queryPrice(db, "SELECT SecondClassPrice "
				"FROM Prices "
				"WHERE Distance >= ? "
				"ORDER BY Distance "
				"LIMIT 1");
			queryPrice.bind(1, distance);
			queryPrice.executeStep();
			price += queryPrice.getColumn(0).getDouble();
		}
		if (distance > 1000)
			distance -= 1000;
	} while (distance > 1000);
	
	return price - (price * discount);
}