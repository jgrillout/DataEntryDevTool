#include <iostream>
//#include <sqlite3.h>
#include "sqlite3.h"

// Callback function to handle query results
int queryCallback(void* data, int argc, char** argv, char** azColName) {
    int i;
    std::cout << "--------------------" << std::endl;
    for (i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

int ListData() {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

    // Open the SQLite database
    rc = sqlite3_open("EmployeeScreen.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    else {
        std::cout << "Opened database successfully" << std::endl;
    }

    // SQL query to select data for the report
    const char* sql = "SELECT * FROM EmployeeScreen";

    // Execute the SQL query
    rc = sqlite3_exec(db, sql, queryCallback, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "Operation done successfully" << std::endl;
    }

    // Close the SQLite database
    sqlite3_close(db);

    return 0;
}
