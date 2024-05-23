// Version: 5.20.24.9.46
// File: DataEntry.h
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <assert.h>
#include <numeric>
#include "curses.h"
#include "ISAMWrapperLib.h"
#include <unordered_map>
#include <algorithm> // Include the header for std::max
#include <tuple>
#include <utility> // For std::pair
using DataTuple = std::tuple<std::string, std::string>;


class DataEntry {

private:
	WINDOW* win;
	std::string field_name;
	std::string fieldtype;
	std::string field_mask;
	int len;
	int row;
	int field_column;
	std::string field_value;
	int label_column;
	std::string label_text;
	

public:
	
	static const int STRING = 0;
	static const int NUMBER = 1;
	static const int MASKED = 2;
	static std::string inputResult;
	static std::string inputAction;	
	static std::string numtest;
	//static bool userTypedValue;
	
	
	DataEntry(WINDOW* win, std::string field_name, std::string fieldtype, std::string field_mask, int len, int row, int field_column, std::string field_value, int label_column, std::string label_text);
	static bool AcceptInput(DataEntry& dataEntry);
	//---------cut from stringInput.h-------------------------------------------
	static bool allowed(std::string type, char character, std::string EDIT$);
	static bool stringInput(DataEntry& DataEntry);
	static bool numericInput(DataEntry& dataEntry);
	static void displayInput(WINDOW* win, int row, int col, const std::string& mask, static std::vector<char>& input);
	static void shiftLeft(WINDOW* win, const std::string& mask, bool decimalEntered,std::vector<char>& input, int ch);
	static void shiftRight(WINDOW* win, const std::string& mask, std::vector<char>& input, int ch);
	static void doDecimal(WINDOW* win, const std::string& mask, bool decimalEntered, std::vector<char>& input, int ch);

	static bool validateXml(WINDOW* winFullScreen, WINDOW* winMsgArea, std::ifstream& xmlFile);
	static bool SetupFields(WINDOW* win, std::vector<DataEntry>& fields, std::ifstream& inputFile);
	void displayData(); //const;    
	void displayLabels();
	static void parseCSVWithQuotes(std::string& line, std::vector<std::string>& tokens);
	static std::string removeLeadingSpaces(const std::string& str);
	//static std::string removeTrailingSpaces(const std::string& str);
	static void removeTrailingSpaces(std::string& str);
	static bool isValidDate(const char* s);
	static std::vector<std::pair<std::string, std::string>> transformVector(const std::vector<DataTuple>& inputVector);

	static void debugMsg(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string string1, std::string string2,int val);

	static void errMsg(WINDOW* win, int row, int col, std::string& msg);

	static void FindMiddle(WINDOW* win, int startrow, int startcol, int& outrow, int& outcol, int width, std::string msg);
	
	static void PrintInMiddle(WINDOW* win, int startrow, int startcol, int width, std::string& msg, chtype color);
	
	static std::string askQuestion(WINDOW* win, int startrow, int startcol, int width, std::string prompt, chtype color);
	static bool confirmAction(WINDOW* msgwin, WINDOW* fullwin, int startrow, int startcol, int width, std::string prompt, chtype color, int keyToPress);	
	static std::string readRecord(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string tbl, bool AddingNew, bool prompt, ISAMWrapperLib& lib, const std::string condition, std::vector<DataEntry>& fields);
	                           
	static std::string doFunctionKeys(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string tbl, bool AddingNew,ISAMWrapperLib& lib, std::string& condition, std::vector<DataEntry>& fields);
	static int stringSwitchNumber(const std::string& key);
	static std::string displayLookupWindow(WINDOW* mainWindow, int lookupHeight, int lookupWidth, int lookupStartY, int lookupStartX, std::vector<std::pair<std::string, std::string>>& data, std::string condition);
	static bool exportTableToCSV(sqlite3* db, const std::string& tableName, const std::string& fileName);
	//static std::string tryreadRecord(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string tbl, bool AddingNew, ISAMWrapperLib& lib, const std::string condition, std::vector<DataEntry>& fields);
//Getters
	WINDOW* getWin() const {
		return win;
	}
	std::string getfieldName() const {
		return field_name;
	}
	std::string getFieldType() const {
		return fieldtype;
	}
	std::string getMask() const {
		return field_mask;
	}
	int getLength() const {
		return len;
	}

	int getRow() const {
		return row;
	}

	int getFieldColumn() const {
		return field_column;
	}

	std::string getFieldValue() const {
		return field_value;
	}

	int getLabelColumn() const {
		return label_column;
	}

	std::string getLabelText() const {
		return label_text;
	}
	


	// Setters
	void setWin(WINDOW * newWin) {
		win = newWin;
	}
	void setName(const std::string& newName) {
		field_name = newName;

	}
	std::string setFieldType(std::string newFieldType) {
		fieldtype = newFieldType;
	}
	void setMask(const std::string& newMask) {
		field_mask = newMask;
	}
	void setLength(int newLength) {
		len = newLength;
	}

	void setRow(int NewRow) {
		row = NewRow;
	}

	void setFieldColumn(int newFieldColumn) {
		field_column = newFieldColumn;
	}

	void setFieldValue(const std::string & newFieldValue)  {
		field_value = newFieldValue;
	}



	void setLabelColumn(int newLabelColumn) {
		label_column = newLabelColumn;
	}

	void setLabelText(const std::string & newLabelText) {
		label_text = newLabelText;
	}
		
	
		
};
