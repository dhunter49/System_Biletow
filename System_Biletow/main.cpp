#include "headers.h"
#include <SQLiteCpp/SQLiteCpp.h>

int main() {
	try {
		SQLite::Database db("test_db.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	}
	catch (const std::exception e) {
		std::cerr << "Blad: " << e.what() << std::endl;
	}
	
	return 0;
}