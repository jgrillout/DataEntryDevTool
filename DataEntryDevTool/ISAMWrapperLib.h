//  Version: 4.29.24.09.37
// File: ISAMWrapperLib.h
#pragma once
#ifndef ISAMWRAPPERLIB_H
#define ISAMWRAPPERLIB_H


#include "sqlite3.h"



#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <map>




class ISAMWrapperLib {
private:
    sqlite3* db;

    std::string primaryKeyColumn;

    // Executes a SQL query on the database

    int executeQuery(const std::string& query);

public:
    // Constructor to open the database connection
    ISAMWrapperLib(const std::string& dbPath, const std::string& pkColumn = "id");

    // Destructor to close the database connection
    ~ISAMWrapperLib();




    // Method to create a table with the given name and columns
    int createTable(const std::string& tableName, const std::vector<std::string>& columns);



     // Method to insert a row into the specified table
    int insert(const std::string& tableName, const std::vector<std::string> fieldNames,const std::vector<std::string>& fieldValues);

    int updateRow(const std::string& tableName, const std::vector<std::string>& columnNames, const std::vector<std::string>& newValues, const std::string& condition);
    int deleteRow(const std::string& tableName, const std::string& condition);
    // Method to select rows from a table optionally with a condition
    std::vector<std::tuple<std::string, std::string>> selectRows(const std::string& tableName, const std::string& condition = "");
    bool getRecordByCondition(const std::string& table, const std::string& condition, std::vector<std::string>& record);

};

#endif // ISAMWRAPPERLIB_H
