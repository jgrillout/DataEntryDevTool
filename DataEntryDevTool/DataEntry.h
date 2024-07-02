// Version: 7.2.24.10.06
// File: DataEntry.h
#pragma once
#include "curses.h"
#include "ISAMWrapperLib.h"
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
#include <unordered_map>
#include <algorithm> // Include the header for std::max
#include <tuple>
#include <utility> // For std::pair
using DataTuple = std::tuple<std::string, std::string>;


class DataEntry {

private:
	WINDOW* winFullScreen;
	WINDOW* winMsgArea;
	std::string field_name;
	std::string fieldtype;
	std::string field_mask;
	int len;
	int row;
	int field_column;
	std::string field_value;
	int label_column;
	std::string label_text;
	std::string InputKeyPressed;
	std::string allowedChoices;
	std::string choiceDescriptions;
	

public:
	
	static const int STRING = 0;
	static const int NUMBER = 1;
	static const int MASKED = 2;
	//static std::string inputResult;
	//static std::string InputKeyPressed;
	static std::string numtest;
	//static bool userTypedValue;
	//WINDOW* winFullScreen;
	//WINDOW* winMsgArea;
	
	DataEntry(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string field_name, std::string fieldtype, std::string field_mask, int len, int row, int field_column, std::string field_value, int label_column, std::string label_text, std::string allowedChoices, std::string choiceDescriptions);
	static bool AcceptInput(DataEntry& dataEntry, WINDOW* winFullScreen, WINDOW* winMsgArea,std::ofstream& debugFile);
	//---------cut from stringInput.h-------------------------------------------
	//static bool allowed(std::string type, char character, std::string EDIT$);
	static bool stringInput(DataEntry& DataEntry, std::ofstream& debugFile, int test);
	static bool NumericInput(DataEntry& dataEntry,std::ofstream& debugFile);
	static bool MaskedInput(DataEntry& dataEntry, std::ofstream& debugFile);
	static void displayRightToLeft(WINDOW* win, const std::string& input, int row, int col, int inputSize);
	static std::string removeMask(const std::string& maskedInput);
	static void inspectMask(const std::string& format, int& leftSize, int& rightSize);
	static void displayMask(DataEntry& dataEntry, std::string& input,std::ofstream& debugFile);
	static std::string combineInputWithMask(const std::string& mask, const std::string& input);
	static std::string generateDisplayMask(const std::string& mask);
	static bool validateXml(WINDOW* winFullScreen, WINDOW* winMsgArea, std::ifstream& xmlFile);
	static bool SetupFields(WINDOW* winFullScreen, WINDOW* winMsgArea,std::vector<DataEntry>& fields, std::ifstream& inputFile);
	void displayData(); //const;    
	void displayLabels();
	static void parseCSVWithQuotes(std::string& line, std::vector<std::string>& tokens);
	static std::string removeLeadingSpaces(const std::string& str);	
	static void removeTrailingSpaces(std::string& str);
	static std::string rightJustifyString(const std::string& str, int len);	
	static bool isValidDate(const std::string& date);
	static std::vector<std::pair<std::string, std::string>> transformVector(const std::vector<DataTuple>& inputVector);
	static void debugMsg(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string string1, std::string string2,int val);
	static bool errMsg(WINDOW* win, int row, int col, std::string& msg, std::ofstream& debugFile, std::string inputAction);

	static void FindMiddle(WINDOW* win, int startrow, int startcol, int& outrow, int& outcol, int width, std::string msg);
	
	static void PrintInMiddle(WINDOW* win, int startrow, int startcol, int width, std::string& msg, chtype color, std::ofstream& debugFile);
	
	static std::string askQuestion(WINDOW* win, int startrow, int startcol, int width, std::string prompt, chtype color);
	static bool confirmAction(WINDOW* msgwin, WINDOW* fullwin, int startrow, int startcol, int width, std::string prompt, chtype color, int keyToPress, std::ofstream& debugFile);
	static std::string readRecord(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string tbl, bool AddingNew, bool prompt, ISAMWrapperLib& lib, const std::string condition, std::vector<DataEntry>& fields);
	                           
	static std::string doFunctionKeys(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string tbl, bool AddingNew,ISAMWrapperLib& lib, std::string& condition, std::vector<DataEntry>& fields, std::ofstream& debugFile, std::string inputAction);
	static int stringSwitchNumber(const std::string& key);
	static std::string displayLookupWindow(WINDOW* mainWindow, int lookupHeight, int lookupWidth, int lookupStartY, int lookupStartX, std::vector<std::pair<std::string, std::string>>& data, std::string condition);
	static bool exportTableToCSV(sqlite3* db, const std::string& tableName, const std::string& fileName);
	static void hideWindow(WINDOW* winMsgArea, WINDOW* winfullScreen);
//Getters
	
	WINDOW* getwinFullScreen() const {
		return winFullScreen;
	}
	WINDOW* getwinMsgArea() const {
		return winMsgArea;
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

	int getLabelColumn() const {
		return label_column;
	}

	std::string getFieldValue() const {
		return field_value;
	}

	std::string getLabelText() const {
		return label_text;
	}
	std::string getInputKeyPressed() const {
		return InputKeyPressed;
	}
	const std::string& getAllowedChoices() const {
		return allowedChoices;
	}

	const std::string& getChoiceDescriptions() const {
		return choiceDescriptions;
	}

	
	// Setters
	void setwinFullScreen(WINDOW * newWin) {
		winFullScreen = newWin;
	}
	void setwinMsgArea(WINDOW* newwinMsgArea) {
		winMsgArea = newwinMsgArea;
	}
	void setName(const std::string& newName) {
		field_name = newName;

	}
	std::string setFieldType(std::string& newFieldType) {
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
	void setInputKeyPressed(const std::string & newInputKeyPressed) {
		InputKeyPressed = newInputKeyPressed;
	}
	void setallowedChoices(const std::string & newallowedChoices) {
		allowedChoices = newallowedChoices;
	}

	void setchoiceDescriptions(const std::string & newchoiceDescriptions) {
		choiceDescriptions = newchoiceDescriptions;
	}
};

