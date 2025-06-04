#include "Car.h"
#include <SQLiteCpp/SQLiteCpp.h>
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

int Car::getTakenSeats(int stationStartNumber, int stationEndNumber) {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT COUNT(*) FROM Passengers WHERE "
        "TripID = ? AND "
        "CarNumber = ? AND "
        "FromStation < ? AND "
        "ToStation >= ?");
    query.bind(1, tripID);
    query.bind(2, carNumber);
    query.bind(3, stationEndNumber);
    query.bind(4, stationStartNumber);
    query.executeStep();
    return query.getColumn(0).getInt();
}