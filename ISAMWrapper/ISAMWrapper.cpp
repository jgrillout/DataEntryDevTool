#pragma once
#include <iostream>
#include <string>
#include "ISAMWrapperLib.h"
//#include "sqlite3.h"

using namespace std;
const std::string& tableName="";
char* errMsg;

// Execute SQL query
int ISAMWrapperLib::executeQuery(const std::string& query) {
    return sqlite3_exec(db, query.c_str(), NULL, 0, &errMsg);
}

    // Create a table
    int ISAMWrapperLib::createTable(const std::string& tableName, const std::vector<std::string>& columns) {
        std::string query = "CREATE TABLE IF NOT EXISTS " + tableName + " (" + columns[0] + "  PRIMARY KEY, ";
        for (size_t i = 1; i < columns.size(); ++i) {
            query += columns[i];
            if (i != columns.size() - 1) query += ", ";
        }
        query += ")";
        return executeQuery(query);
    }

    // Insert a row
    int ISAMWrapperLib::insert(const std::string& tableName, const std::vector<std::string>& values) {
        std::string query = "INSERT INTO " + tableName + " VALUES (" + values[0];
        for (size_t i = 1; i < values.size(); ++i) {
            query += ", " + values[i];
        }
        query += ")";
        return executeQuery(query);
    }

    // Select rows
    std::vector<std::tuple<std::string, std::string>> ISAMWrapperLib::selectRows(const std::string& tableName, const std::string& condition) {
        std::string query = "SELECT * FROM " + tableName;
        if (!condition.empty()) {
            query += " WHERE " + condition;
        }

        sqlite3_stmt* stmt;
        std::vector<std::tuple<std::string, std::string>> results;
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string id = std::to_string(sqlite3_column_int(stmt, 0));
                std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                results.emplace_back(id, name);
            }
            sqlite3_finalize(stmt);
        }
        return results;
    }
    bool ISAMWrapperLib::getRecordByPrimaryKey(const std::string& tableName, const std::string& primaryKeyValue, std::map<std::string, std::string>& record){
   
        std::string query = "SELECT * FROM " + tableName + " WHERE " + primaryKeyColumn + " = " + primaryKeyValue;


        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
            return false; // Query preparation failure
        }

        sqlite3_bind_text(stmt, 1, primaryKeyValue.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int columns = sqlite3_column_count(stmt);
            for (int i = 0; i < columns; ++i) {
                const char* colName = sqlite3_column_name(stmt, i);
                const unsigned char* colValue = sqlite3_column_text(stmt, i);
                record[colName] = colValue ? reinterpret_cast<const char*>(colValue) : "";
            }
            sqlite3_finalize(stmt);
            return true; // Record found and populated
        }

        sqlite3_finalize(stmt);
        record.clear(); // Clear any existing data in the record map
        return false; // No record found
    }



int main() {
    ISAMWrapperLib isam("example.db");

    isam.createTable("students", { "id", "name", "age" });
    isam.insert("students", { "1", "'Alice'", "25" });
    isam.insert("students", { "2", "'Bob'", "30" });

    auto rows = isam.selectRows("students");
    for (const auto& row : rows) {
        std::cout << "ID: " << std::get<0>(row) << ", Name: " << std::get<1>(row) << std::endl;
    }

    std::map<std::string, std::string> record;
    std::string primaryKeyValue = "1"; // Assuming you are looking for a record with the primary key value "1"

    // Attempt to retrieve the record
    if (isam.getRecordByPrimaryKey(tableName, primaryKeyValue, record)) {
        std::cout << "Record found:" << std::endl;
        for (const auto& field : record) {
            std::cout << field.first << ": " << field.second << std::endl;
        }
    }
    else {
        std::cout << "No record found with primary key value " << primaryKeyValue << std::endl;
    }

}
