#include "Train.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "GlobalConsts.h"

std::string Train::getTrainID() {
	return trainID;
}
std::string Train::getTrainName() {
	return trainName;
}

void Train::setTrainID(std::string newTrainID) {
	trainID = newTrainID;
}
void Train::setTrainName(std::string newTrainName) {
	trainName = newTrainName;
}

Train::Train() {};
Train::Train(std::string trainID) :trainID(trainID) {};
Train::Train(std::string trainID, int trainIDNumber, std::string trainName) :trainID(trainID), trainIDNumber(trainIDNumber), trainName(trainName) {};
Train::Train(Trip trip) {
	routeID = trip.getRouteID();
	stationList = trip.getStationList();
	tripID = trip.getTripID();
	schedules = trip.getSchedules();
	date = trip.getDate();
}

MenuOption Train::getMenuOptionTrain() {
    std::string optionName{};
    optionName += trainID;
    optionName += ' ';
    optionName += trainName;
    
    return MenuOption{ trainIDNumber, optionName };
}

int Train::getTakenSeats(int stationStartNumber, int stationEndNumber) {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT COUNT(*) FROM Passengers LEFT JOIN Seats ON Passengers.SeatNumber = Seats.Number WHERE "
        "Passengers.TripID = ? AND "
        "Passengers.FromStation < ? AND "
        "Passengers.ToStation > ? "
        "AND Seats.Special IS NULL");

    query.bind(1, tripID);
    query.bind(2, stationEndNumber);
    query.bind(3, stationStartNumber);
    query.executeStep();
    return query.getColumn(0).getInt();
}

int Train::getSeatCount(int stationStartNumber, int stationEndNumber) {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT COUNT(Seats.Number) "
        "FROM Seats FULL OUTER JOIN TrainSets ON Seats.CarModel = TrainSets.CarModel "
        "WHERE TrainSets.TrainID = ? "
        "AND Special IS NULL");
    query.bind(1, trainID);
    query.executeStep();
    return query.getColumn(0).getInt();
}

int Train::getFreeSeats(int stationStartNumber, int stationEndNumber) {
    return getSeatCount(stationStartNumber, stationEndNumber) - getTakenSeats(stationStartNumber, stationEndNumber);
}

void Train::showInfo() {
    clearScreen();
    std::cout << "Wagony w poci¹gu " << trainID << " - " << trainName << ":" << std::endl;
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT CarNumber, CarModel "
        "FROM TrainSets "
        "WHERE TrainSets.TrainID = ? "
        "ORDER BY CarNumber");
    query.bind(1, trainID);
    while(query.executeStep()) {
        std::cout << query.getColumn(0).getString() << " - " << query.getColumn(1).getString() << std::endl;
    }
}