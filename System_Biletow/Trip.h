#pragma once
#include "Route.h"

class Trip : protected Route {
protected:
	//variables
	int tripID;
	//array/list of arrival times on each stop
public:
	Trip();
	Trip(int);
};

