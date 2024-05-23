#include <sqlite3.h>
#include <iostream>

int main() {
    // Open the SQLite database
    sqlite3* db;
    int rc = sqlite3_open("your_database.db", &db);
    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Check if the table exists
    bool tableExists = false;
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='your_table';";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            tableExists = true;
        }
    }
    sqlite3_finalize(stmt);
	if (tableExists) {
		 std string outname = arg1+".csv";
    if (!exportTableToCSV(db, arg1, outname)) {
        std::cerr << "Error exporting table data to CSV file." << std::endl;
        sqlite3_close(db);
        return 1;
    }
	}
    // Create the new table if it does not exist
    if (!tableExists) {
        const char* createTableQuery = "CREATE TABLE your_table (id INTEGER PRIMARY KEY, name TEXT);";
        rc = sqlite3_exec(db, createTableQuery, nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return 1;
        }
    }

    // Retrieve data from the existing table and insert it into the new table
    // (Code for data retrieval and insertion goes here)

    // Close the database connection
    sqlite3_close(db);

    return 0;
}
