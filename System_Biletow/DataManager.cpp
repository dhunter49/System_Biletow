#include "DataManager.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "GlobalConsts.h"

DataManager* DataManager::instance = nullptr;

DataManager& DataManager::getInstance() {
    if (!instance) {
        instance = new DataManager();
    }
    return *instance;
}

Route DataManager::getRouteByID(int routeID) {
    if (routes.find(routeID) != routes.end()) {
        return routes[routeID];
    }
    throw std::runtime_error("Route not found");
}

// Loades all routes to a map. Station List in only filled with station's that are supposed to show in menu - skipped unimportant stations.
void DataManager::loadAllRoutesFromDatabase() {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT ID, StationNumber, StationID FROM Routes WHERE IsShowing = 1 ORDER BY ID, StationNumber");

    if (!query.executeStep()) {
        throw std::runtime_error("No routes found in database!");
    }

    int currentRouteID = query.getColumn(0).getInt();
    Route currentRoute(currentRouteID);

    do {
        int routeID = query.getColumn(0).getInt();

        // If we encounter a new route, store the previous one and start a new route
        if (routeID != currentRouteID) {
            routes[currentRouteID] = currentRoute;
            currentRouteID = routeID;
            currentRoute = Route(currentRouteID);
        }

        currentRoute.loadStations(false);
    } while (query.executeStep());

    // Store the last route after the loop ends
    routes[currentRouteID] = currentRoute;
}

// Loades all trains to a vector
void DataManager::loadAllTrainsFromDatabase() {
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT ID, IDNumber, Name FROM Trains");

    if (!query.executeStep()) {
        throw std::runtime_error("No trains found in database!");
    }

    do {
        std::string trainID = query.getColumn(0).getString();
        int trainIDNumber = query.getColumn(1).getInt();
        std::string trainName = query.getColumn(2).getString();
        Train train(trainID, trainIDNumber, trainName);
        trains[trainIDNumber]=train;
    } while (query.executeStep());
}

// Generates vector of menu options for function showMenu, it uses all loaded routes to give user possiblity to choose a route.
std::vector<MenuOption> DataManager::generateMenuListRoutes() {
    std::vector<MenuOption> out;
    MenuOption oneOption;
    for (auto& routePair : routes) {
    	oneOption = routePair.second.getMenuOptionRoute();
    	out.push_back(oneOption);
    }
    return out;
}

// Generates vector of menu options for function showMenu, it uses all loaded trains to give user possiblity to choose a train.
std::vector<MenuOption> DataManager::generateMenuListTrains() {
    std::vector<MenuOption> out;
    MenuOption oneOption;
    for (auto& train : trains) {
        oneOption = train.second.getMenuOptionTrain();
    	out.push_back(oneOption);
    }
    return out;
}

// Gets all trips in a vector that are matching routeID and matching inputted date. Supposedly should find one Trip, although not always.
// Returns vector with Trips with matching criteria.
void DataManager::getTripsByDateAndRouteID(Date date, int routeID) {
    currentTrips = std::vector<Trip>(); // Empty vector
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db,
        "SELECT ID, "
        "CAST(strftime('%H', Time) AS INTEGER) AS hour, "
        "CAST(strftime('%M', Time) AS INTEGER) AS minute "
        "FROM Trips "
        "WHERE RouteID = ? "
        "AND strftime('%d', Date) = ? "
        "AND strftime('%m', Date) = ? "
        "AND strftime('%Y', Date) = ?");

    // Bind parameters
    query.bind(1, routeID);
    query.bind(2, date.day < 10 ? "0" + std::to_string(date.day) : std::to_string(date.day)); // Ensure 2-digit format
    query.bind(3, date.month < 10 ? "0" + std::to_string(date.month) : std::to_string(date.month)); // Ensure 2-digit format
    query.bind(4, std::to_string(date.year));
    if (!query.executeStep())
        throw std::runtime_error("Brak przejazdów danego dnia tej relacji!");
    int currentTripID = query.getColumn(0).getInt();
    Trip currentTrip;
    do {
        currentTrip = Trip(currentTripID); // Clear an object
        // Set all vars in new obj
        currentTrip.setRouteID(routeID);
        currentTrip.setDate(date);
        currentTrip.loadStations(true);
        currentTrip.setSchedule(1,
            Schedule{
            currentTrip.getStation(1).id,
            Time{query.getColumn(1).getUInt(), query.getColumn(2).getUInt()}, // arrival
            Time{query.getColumn(1).getUInt(), query.getColumn(2).getUInt()}  // departure
            }
        );
        // Adds Trip to vector
        currentTrips.push_back(currentTrip);
    } while (query.executeStep());

    for (auto& itTrip : currentTrips) {
        itTrip.loadAllOtherSchedules();
    }
}

void DataManager::loadTripByID(int tripID) {
    currentTrips = std::vector<Trip>(); // Empty vector
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db,
        "SELECT ID, RouteID "
        "CAST(strftime('%H', Time) AS INTEGER) AS hour, "
        "CAST(strftime('%M', Time) AS INTEGER) AS minute, "
        "CAST(strftime('%d', Date) AS INTEGER) AS day, "
        "CAST(strftime('%m', Date) AS INTEGER) AS month, "
        "CAST(strftime('%y', Date) AS INTEGER) AS year "
        "FROM Trips "
        "WHERE ID = ?");

    // Bind parameters
    query.bind(1, tripID);

    if (!query.executeStep())
        throw std::runtime_error("Brak przejazdów o danym numerze!");
    Trip trip(tripID);
    // Set all vars in new obj
    trip.setRouteID(query.getColumn(1).getInt());
    trip.setDate(
        Date{
			query.getColumn(4).getUInt(), // day
			query.getColumn(5).getUInt(), // month
			query.getColumn(6).getUInt() // year
        });
    trip.loadStations(true);
    trip.setSchedule(1,
        Schedule{
        trip.getStation(1).id,
        Time{query.getColumn(2).getUInt(), query.getColumn(3).getUInt()}, // arrival
        Time{query.getColumn(2).getUInt(), query.getColumn(3).getUInt()}  // departure
        }
    );
    trip.loadAllOtherSchedules();
    // Adds Trip to vector
    currentTrips.push_back(trip);
}

Trip DataManager::getTripByID(int tripID) {
    auto it = std::find_if(currentTrips.begin(), currentTrips.end(),
        [tripID](Trip t) {return t.getTripID() == tripID;});

    if (it == currentTrips.end()) {
        throw std::runtime_error("Trip not found");
    }

    return *it;
}

std::vector<MenuOption> DataManager::generateMenuListTrips(int stationNumberStart, int stationNumberEnd) {
	std::vector<MenuOption> out;
	MenuOption oneOption;
	for (auto& trip : currentTrips) {
		oneOption = trip.getMenuOptionTrip(stationNumberStart, stationNumberEnd);
		out.push_back(oneOption);
	}
	return out;
}

void DataManager::getTrainByTripID(int tripID) {
    int routeID = getTripByID(tripID).getRouteID();

    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT ID, Name FROM Trains WHERE RouteID = ?");
    query.bind(1, routeID);

    if (!query.executeStep()) {
        throw std::runtime_error("Nie znaleziono pociągu dla przejazdu: " + std::to_string(tripID));
    }

   std::string trainID = query.getColumn(0).getString();
   std::string trainName = query.getColumn(1).getString();

    // Trying to get another row, as there should be only one
    if (query.executeStep()) { 
        throw std::runtime_error("Więcej niż jeden pociąg dla przejazdu: " + std::to_string(tripID));
    }

    currentTrain = Train(getTripByID(tripID));
    currentTrain.setTrainID(trainID);
    currentTrain.setTrainName(trainName);
}

Train DataManager::getTrain() {
    return currentTrain;
}

void DataManager::getCarsByTrainID(std::string trainID) {
    currentCars = std::vector<Car>();
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT CarNumber, CarModel FROM TrainSets WHERE TrainID = ?");
    query.bind(1, trainID);

    Car currentCar;
    while (query.executeStep()) {
        currentCar = Car(currentTrain);
        currentCar.setCarNumber(query.getColumn(0).getInt());
        currentCar.setCarModel(query.getColumn(1).getString());

        currentCars.push_back(currentCar);
    }
}

Car DataManager::getCarByNumber(int carNumber) {
    auto it = std::find_if(currentCars.begin(), currentCars.end(),
        [carNumber](Car t) {return t.getCarNumber() == carNumber;});

    if (it == currentCars.end()) {
        throw std::runtime_error("Car not found");
    }

    return *it;
}

void DataManager::getCompartmentsByCarNumber(int carNumber) {
    currentCompartments = std::vector<Compartment>();
    std::string carModel = getCarByNumber(carNumber).getCarModel();
    Compartment currentCompartment;
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT Number, IsCompartment, IsFirstClass FROM Compartments WHERE CarModel = ?");
    query.bind(1, carModel);

    while (query.executeStep()) {
        currentCompartment = Compartment(getCarByNumber(carNumber));
        currentCompartment.setCompartmentNumber(query.getColumn(0).getInt());
        currentCompartment.setIsAnActualCompartment(query.getColumn(1).getInt());
        currentCompartment.setIsFirstClass(query.getColumn(2).getInt());

        currentCompartments.push_back(currentCompartment);
    }
}

Compartment DataManager::getCompartmentByNumber(int compartmentNumber) {
    auto it = std::find_if(currentCompartments.begin(), currentCompartments.end(),
        [compartmentNumber](Compartment t) {return t.getCompartmentNumber() == compartmentNumber;});

    if (it == currentCompartments.end()) {
        throw std::runtime_error("Compartment not found");
    }

    return *it;
}

void DataManager::getSeatsByCompartmentNumber(int compartmentNumber, int carNumber) {
    currentSeats = std::vector<Seat>();
    std::string carModel = getCarByNumber(carNumber).getCarModel();
    Seat currentSeat;
    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    SQLite::Statement query(db, 
        "SELECT Number, IsWindow, IsMiddle, IsCorridor, IsTable, Special "
        "FROM Seats "
        "WHERE Number >= ? AND Number <= ? AND CarModel = ?");
    query.bind(1, compartmentNumber * 10);
    query.bind(2, compartmentNumber * 10 + 9);
    query.bind(3, carModel);

    while (query.executeStep()) {
        currentSeat = Seat(getCompartmentByNumber(compartmentNumber));

        currentSeat.setSeatNumber(query.getColumn(0).getInt());
        currentSeat.setIsWindow(query.getColumn(1).getInt());
        currentSeat.setIsMiddle(query.getColumn(2).getInt());
        currentSeat.setIsCorridor(query.getColumn(3).getInt());
        currentSeat.setIsByTable(query.getColumn(4).getInt());
        currentSeat.setSpecial(query.getColumn(5).getInt());

        currentSeats.push_back(currentSeat);
    }
}

void DataManager::getFreeSeatsByCompartmentNumber(int compartmentNumber, int carNumber, int stationNumberStart, int stationNumberEnd) {
    currentSeats = std::vector<Seat>();
    std::string carModel = getCarByNumber(carNumber).getCarModel();
    int tripID = currentTrain.getTripID();

    SQLite::Database db(DATABASE_PATH, SQLite::OPEN_READONLY);
    // Select all seats in the compartment where special IS NULL
    SQLite::Statement seatQuery(db, 
        "SELECT Number, IsWindow, IsMiddle, IsCorridor, IsTable "
        "FROM Seats "
        "WHERE Number >= ? AND Number <= ? AND CarModel = ? AND Special IS NULL");

    seatQuery.bind(1, compartmentNumber * 10);
    seatQuery.bind(2, compartmentNumber * 10 + 9);
    seatQuery.bind(3, carModel);

    while (seatQuery.executeStep()) {
        int seatNumber = seatQuery.getColumn(0).getInt();

        // Check if seat is reserved for this trip and for any overlapping segment
        SQLite::Statement reservationQuery(db,
            "SELECT COUNT(*) FROM Passengers "
            "WHERE TripID = ? AND SeatNumber = ? AND CarNumber = ? "
            "AND ((FromStation < ? AND ToStation > ?) "
            "  OR (FromStation < ? AND ToStation > ?) "
            "  OR (FromStation >= ? AND ToStation <= ?))");

        reservationQuery.bind(1, tripID);
        reservationQuery.bind(2, seatNumber);
        reservationQuery.bind(3, carNumber);
        reservationQuery.bind(4, stationNumberEnd);
        reservationQuery.bind(5, stationNumberStart);
        reservationQuery.bind(6, stationNumberEnd);
        reservationQuery.bind(7, stationNumberStart);
        reservationQuery.bind(8, stationNumberStart);
        reservationQuery.bind(9, stationNumberEnd);

        reservationQuery.executeStep();
        int reservedCount = reservationQuery.getColumn(0).getInt();

        if (reservedCount == 0) {
            Seat currentSeat(getCompartmentByNumber(compartmentNumber));
            currentSeat.setSeatNumber(seatNumber);
            currentSeat.setIsWindow(seatQuery.getColumn(1).getInt());
            currentSeat.setIsMiddle(seatQuery.getColumn(2).getInt());
            currentSeat.setIsCorridor(seatQuery.getColumn(3).getInt());
            currentSeat.setIsByTable(seatQuery.getColumn(4).getInt());
            currentSeats.push_back(currentSeat);
        }
    }
}

Seat DataManager::getSeatByNumber(int seatNumber) {
    auto it = std::find_if(currentSeats.begin(), currentSeats.end(),
        [seatNumber](Seat t) {return t.getSeatNumber() == seatNumber;});

    if (it == currentSeats.end()) {
        throw std::runtime_error("Seat not found");
    }

    return *it;
}