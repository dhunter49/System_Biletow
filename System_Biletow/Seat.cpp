#include "Seat.h"

Seat::Seat() : seatNumber(0), isFacingFront(0), isByTable(0) {};
Seat::Seat(int seatNumber) :seatNumber(seatNumber) {};
Seat::Seat(Compartment compartment) : Compartment(compartment) {};

bool Seat::getIsFacingFront() {
	return isFacingFront;
}

bool Seat::getIsByTable() {
	return isByTable;
}

bool Seat::getIsWindow() {
	return isWindow;
}

bool Seat::getIsMiddle() {
    return isMiddle;
}

bool Seat::getIsCorridor() {
    return isCorridor;
}

char Seat::getSpecial() {
    return special;
}

int Seat::getSeatNumber() {
    return seatNumber;
}

std::map<int, int> Seat::getTakenBy() {
    return takenBy;
}

void Seat::setIsFacingFront(bool newIsFacingFront) {
    isFacingFront = newIsFacingFront;
}

void Seat::setIsByTable(bool newIsByTable) {
    isByTable = newIsByTable;
}

void Seat::setIsWindow(bool newIsWindow) {
    isWindow = newIsWindow;
}

void Seat::setIsMiddle(bool newIsMiddle) {
    isMiddle = newIsMiddle;
}

void Seat::setIsCorridor(bool newIsCorridor) {
    isCorridor = newIsCorridor;
}

void Seat::setSpecial(char newSpecial) {
    special = newSpecial;
}

void Seat::setSeatNumber(int newSeatNumber) {
    seatNumber = newSeatNumber;
}

void Seat::setTakenBy(std::map<int, int> newTakenBy) {
    takenBy = newTakenBy;
}