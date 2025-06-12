#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include "Car.h"
#include "GlobalConsts.h"

Car::Car():carNumber(0) {};
Car::Car(int carNumber) : carNumber(carNumber) {};
Car::Car(Train train) : Train(train) {};
 
int Car::getCarNumber() {
	return carNumber;
}

std::string Car::getCarModel() {
	return carModel;
}

void Car::setCarNumber(int newCarNumber) {
	carNumber = newCarNumber;
}

void Car::setCarModel(std::string newCarModel) {
	carModel = newCarModel;
}

// Returns how many seats are taken in this car
int Car::getTakenSeats(int stationStartNumber, int stationEndNumber) {
    try {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
        SQLite::Statement query(db, "SELECT COUNT(DISTINCT ID) FROM Passengers LEFT JOIN Seats ON Passengers.SeatNumber = Seats.Number WHERE "
            "Passengers.TripID = ? AND "
            "Passengers.CarNumber = ? AND "
            "Passengers.FromStation < ? AND "
            "Passengers.ToStation > ? "
            "AND Seats.Special IS NULL");
        query.bind(1, tripID);
        query.bind(2, carNumber);
        query.bind(3, stationEndNumber);
        query.bind(4, stationStartNumber);
        query.executeStep();
        return query.getColumn(0).getInt();
    }
    catch (SQLite::Exception& e) {
        std::cerr << "Problem z bazą danych: " << e.what() << "\nKliknij ESC aby kontynuowaæ...";
        waitForEsc();
        return 0;
    }
}

// Returns how many seats are there in this car
int Car::getSeatCount(int stationStartNumber, int stationEndNumber) {
    try {
        SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
        SQLite::Statement query(db, "SELECT COUNT(Seats.Number) "
            "FROM Seats FULL OUTER JOIN TrainSets ON Seats.CarModel = TrainSets.CarModel "
            "WHERE TrainSets.TrainID = ? "
            "AND TrainSets.CarNumber = ? "
            "AND Seats.Special IS NULL");
        query.bind(1, trainID);
        query.bind(2, carNumber);
        query.executeStep();
        return query.getColumn(0).getInt();
    }
    catch (SQLite::Exception& e) {
        std::cerr << "Problem z bazą danych: " << e.what() << "\nKliknij ESC aby kontynuowaæ...";
        waitForEsc();
        return 0;
    }
}

// Return amount of available seats in this car
int Car::getFreeSeats(int stationStartNumber, int stationEndNumber) {
    return getSeatCount(stationStartNumber, stationEndNumber) - getTakenSeats(stationStartNumber, stationEndNumber);
}