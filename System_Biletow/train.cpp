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
Train::Train(Trip trip) {
	routeID = trip.getRouteID();
	stationList = trip.getStationList();
	tripID = trip.getTripID();
	schedules = trip.getSchedules();
	date = trip.getDate();
}

int Train::getTakenSeats(int stationStartNumber, int stationEndNumber) {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT COUNT(*) FROM Passengers WHERE "
        "TripID = ? AND "
        "FromStation < ? AND "
        "ToStation >= ?");
    query.bind(1, tripID);
    query.bind(2, stationEndNumber);
    query.bind(3, stationStartNumber);
    query.executeStep();
    return query.getColumn(0).getInt();
}
