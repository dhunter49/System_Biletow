#include "Train.h"

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
