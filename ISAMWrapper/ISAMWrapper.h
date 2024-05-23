#pragma once
#ifndef ISAMWRAPPER_H
#define ISAMWRAPPER_H

// header is already in the sqllite3.c file as provided by www.sqlite.org
//#include "sqlite3.h"
#include <string>
#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <tuple>
#include <map>
class ISAMWrapper {
private:
    sqlite3* db;
    char* errMsg;
    std::string primaryKeyColumn;

    
    
    int executeQuery(const std::string& query);

public:
    ISAMWrapper(const std::string& dbPath, const std::string& pkColumn = "id") : primaryKeyColumn(pkColumn) {
        int rc = sqlite3_open(dbPath.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
        }
    }

    ~ISAMWrapper() {
        sqlite3_close(db);
    }

    // Method to get a specific student record by primary key
    
    std::vector<std::tuple<std::string, std::string>> selectRows(const std::string& tableName, const std::string& condition = "");

    bool getRecordByPrimaryKey(const std::string& primaryKeyValue, std::map<std::string, std::string>& record);


    int createTable(const std::string& tableName, const std::vector<std::string>& columns);
    int insert(const std::string& tableName, const std::vector<std::string>& values);
};

#endif // ISAMWRAPPER_H

