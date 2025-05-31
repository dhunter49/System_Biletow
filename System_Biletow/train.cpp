#include "Train.h"

Train::Train() {};
Train::Train(int trainID) :trainID(trainID) {};
Train::Train(Trip trip) {
	routeID = trip.getRouteID();
	stationList = trip.getStationList();
	tripID = trip.getTripID();
	schedules = trip.getSchedules();
	date = trip.getDate();
}
