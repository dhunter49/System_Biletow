#include <iostream>
#include "sqlite3.h"

int main() {
    sqlite3* db;
    int rc = sqlite3_open("example.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // Use the database...

    sqlite3_close(db);
	return 0;
}