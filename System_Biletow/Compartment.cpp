#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include "Compartment.h"
#include "GlobalConsts.h"

Compartment::Compartment() : compartmentNumber(0) {};
Compartment::Compartment(int compartmentNumber) :compartmentNumber(compartmentNumber) {};
Compartment::Compartment(Car car) : Car(car) {};

void Compartment::setCompartmentNumber(int newCompartmentNumber) {
	compartmentNumber = newCompartmentNumber;
}

void Compartment::setIsAnActualCompartment(bool is) {
	isAnActualCompartment = is;
}

void Compartment::setIsFirstClass(bool is) {
    isFirstClass = is;
}

int Compartment::getCompartmentNumber() {
	return compartmentNumber;
}

bool Compartment::getIsAnActualCompartment() {
	return isAnActualCompartment;
}

bool Compartment::getIsFirstClass() {
    return isFirstClass;
}

// Returns how many seats are taken in this compartment
int Compartment::getTakenSeats(int stationStartNumber, int stationEndNumber) {
    try {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
        SQLite::Statement query(db, "SELECT COUNT(DISTINCT ID) FROM Passengers LEFT JOIN Seats ON Passengers.SeatNumber = Seats.Number WHERE "
            "Passengers.TripID = ? AND "
            "Passengers.CarNumber = ? AND "
            "Passengers.SeatNumber >= ? AND Passengers.SeatNumber <= ? AND "
            "Passengers.FromStation < ? AND "
            "Passengers.ToStation > ? "
            "AND Seats.Special IS NULL");

        query.bind(1, tripID);
        query.bind(2, carNumber);
        query.bind(3, compartmentNumber * 10);
        query.bind(4, compartmentNumber * 10 + 9);
        query.bind(5, stationEndNumber);
        query.bind(6, stationStartNumber);
        query.executeStep();
        return query.getColumn(0).getInt();
    }
    catch (SQLite::Exception& e) {
        std::cerr << "Problem z baz¹ danych: " << e.what() << "\nKliknij ESC aby kontynuowaæ...";
        waitForEsc();
        return 0;
    }
}

// Returns how many seats are there in this compartment
int Compartment::getSeatCount(int stationStartNumber, int stationEndNumber) {
    try {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
        SQLite::Statement query(db, "SELECT COUNT(Seats.Number) "
            "FROM Seats FULL OUTER JOIN TrainSets ON Seats.CarModel = TrainSets.CarModel "
            "WHERE TrainSets.TrainID = ? "
            "AND TrainSets.CarNumber = ? "
            "AND Seats.Number/10 = ? "
            "AND Special IS NULL");
        query.bind(1, trainID);
        query.bind(2, carNumber);
        query.bind(3, compartmentNumber);
        query.executeStep();
        return query.getColumn(0).getInt();
    }
    catch (SQLite::Exception& e) {
        std::cerr << "Problem z bazą danych: " << e.what() << "\nKliknij ESC aby kontynuowaæ...";
        waitForEsc();
        return 0;
    }
}

// Return amount of available seats in this compartment
int Compartment::getFreeSeats(int stationStartNumber, int stationEndNumber) {
    return getSeatCount(stationStartNumber, stationEndNumber) - getTakenSeats(stationStartNumber, stationEndNumber);
}
