// Version: 5.20.24.9.46
// File: numericInput.cpp
#pragma once
#include "DataEntry.h" 
//#include <vector>
//#include <string>
//#include <iostream>
//#include <numeric>
const int INVALID_INDEX = -1;
std::string DataEntry::numtest;
bool DataEntry::numericInput(DataEntry& dataEntry)
{
    // this is code from the stringinput.cpp
    // I'm not sure if I'll use it in this program
   /* Note: parameter names are diffent then the declartions so they match
    the names of code i borrowed from a business BASIC program GENINPUT.PUB
    std::string X$ = "";
    std::string EDIT$ = "";
    std::string Action = "";
    std::string before = "";
    std::string after = "";*/
    std::string inputString = dataEntry.getFieldValue();

    inputString.erase(std::remove(inputString.begin(), inputString.end(), '\"'));
    // Convert string back to a vector of characters
    std::vector<char> input;
    input.assign(inputString.begin(), inputString.end());
    std::string mask = dataEntry.getMask();
    mask.erase(std::remove(mask.begin(), mask.end(), '\"'));
    WINDOW* saveWin = dataEntry.getWin();
    size_t saveLen = dataEntry.getLength();
    int saveRow = dataEntry.getRow();
    int saveColumn = dataEntry.getFieldColumn();


    if (inputString.empty()) {
        inputString = mask;
        inputString.erase(std::remove(inputString.begin(), inputString.end(), '\"'));
    }
    //mask = "000000";
Restart:
    dataEntry.setFieldValue(inputString);
    input.assign(inputString.begin(), inputString.end());

    dataEntry.displayData();

    int c = 0;
    int column = saveColumn;
    int charsEntered = 0;
    //int inputIndex = 0;
    bool isBackspace = false;
    bool decimalEntered = false;
    bool quit = false;
    bool GotoRestart = false;
    std::string Action = "";
    wattron(saveWin, COLOR_PAIR(2));

    noecho(); // Disable echoing of characters
    while (quit != true)
    {

        displayInput(saveWin, saveRow, saveColumn,mask,input);
        //mvwprintw(saveWin, saveRow, saveColumn , inputString.c_str());
        wrefresh(saveWin);


        c = wgetch(saveWin);
        switch (c)
        {
        case PADENTER:
        case 10:  // Enter key    
            Action = "KEY_ENTER";
            goto  exitField;
            break;
        case KEY_F(1):
            Action = "KEY_F(1)";
            quit = true;
            break;
        case KEY_F(2):
            Action = "KEY_F(2)";
            quit = true;
            break;
        case KEY_F(3):
            Action = "KEY_F(3)";
            quit = true;
            break;
        case KEY_F(4):
            Action = "KEY_F(4)";
            quit = true;
            break;
        case KEY_F(5):
            Action = "KEY_F(5)";
            quit = true;
            break;
        case KEY_F(6):
            Action = "KEY_F(6)";
            quit = true;
            break;
        case KEY_F(7):
            Action = "KEY_F(7)";
            wrefresh(saveWin);
            quit = true;
            break;
        case KEY_F(8):
            Action = "KEY_F(8)";
            wrefresh(saveWin);
            quit = true;
            break;
        case KEY_F(9):
            Action = "KEY_F(9)";
            wrefresh(saveWin);
            quit = true;
            break;
        case KEY_F(10):
            Action = "KEY_F(10)";
            wrefresh(saveWin);
            quit = true;
            break;
        case KEY_UP:
            Action = "KEY_UP";
            wrefresh(saveWin);
            quit = true;
            break;

        case KEY_DC:  // delete  
            Action = "KEY_DC";
            inputString = mask;
            quit = true;
            GotoRestart = true;
            break;
        default:
            if (c >= 0 && c <= 127)
            {
                /*if (POS > saveLen) {
                    beep();
                    break;
                }*/
            }
            else {
                Action = "KEY_NOT";// deal with keys i have planned on handling
                wrefresh(saveWin);
                quit = true;
                break;
            }

            // This is the end of the core logic that handles filtering funtion key or special key 
            // we have determined the key pressed  by user is a digit            
            // This is the  beginning of the core logic that handles building the numeric input field. 
            if ((isdigit(c) || c == '.') && charsEntered < mask.size()) {
                if (c == '.') {
                    // Check if decimal point is already entered
                    if (!decimalEntered) {
                        decimalEntered = true;
                        charsEntered++;    
                        doDecimal(saveWin, mask, decimalEntered, input, c);
                        shiftLeft(saveWin, mask, decimalEntered, input, c);
                    }
                }
                else {

                    charsEntered++;
                    shiftLeft(saveWin, mask, decimalEntered, input, c);
                   

                }
            }
            else if (c == 8 && !input.empty()) {
                // Handle backspace to delete the last entered character
                if (input.back() == '.') {
                    // Reset decimal flag if deleted
                    decimalEntered = false;
                }
                charsEntered--;
                shiftRight(saveWin, mask, input, c);
            }
        }

    }
    if (GotoRestart == true)
        goto Restart;

    wrefresh(saveWin);

    if (quit == true) {
        std::string result = std::accumulate(input.begin(), input.end(), std::string(""));
        dataEntry.field_value = result;
        inputResult = result;
        inputAction = Action;

        // indicates a funtion was  pressed and inputAction indicates which one
        return true;

    }

exitField:
    Action = "exitField_NOT_KEY_F";
    wrefresh(saveWin);
    std::string result = std::accumulate(input.begin(), input.end(), std::string(""));
    dataEntry.field_value = result;
    inputResult = result;
    inputAction = Action;
    return false;// indicates exit field and  a funtion was NOT  pressed
}
void  DataEntry::displayInput(WINDOW* win, int row, int col, const std::string& mask, static std::vector<char>& input) {

    // Convert vector of characters to a string
    std::string inputString(input.begin(), input.end());
    mvwprintw(win, row, col, inputString.c_str());
    // Move cursor to the end of input area
    wmove(win, row, col + mask.size());
    wrefresh(win);
}
void DataEntry::shiftLeft(WINDOW* win, const std::string& mask, bool decimalEntered,std::vector<char>& input, int c) {

    // Convert vector of characters to a string
    std::string inputString(input.begin(), input.end());
    std::string character(1, c);
    if (c != 8) {// exclude backspace
        //inputString.append(character);
        size_t decimalIndex = inputString.find('.');
        int numPlacesAfterDecimal = 0;
        int numPlacesBeforeDecimal = 0;
        if (decimalIndex != std::string::npos) {
            // Count the number of characters after the decimal point
            numPlacesAfterDecimal = inputString.size() - decimalIndex - 1;
            numPlacesBeforeDecimal = inputString.size() - numPlacesAfterDecimal - 1;
        }
        else {
            numPlacesAfterDecimal = 0;
            numPlacesBeforeDecimal = 0;
        }
        //inputString.append(character);


        // Move characters to the left by 1, excluding the decimal point
        if (decimalIndex != std::string::npos) {
            // get characters after the decimal point
            std::string after = inputString.substr(decimalIndex + 1, numPlacesAfterDecimal);
            inputString.erase(decimalIndex, 1); // Remove the decimal point
            if (numPlacesAfterDecimal > 0) {
                // Move characters to the left
                inputString = inputString.substr(1, decimalIndex-1) + character+ '.';
                inputString = inputString + after;
                
            }
            else
                inputString = inputString.substr(1, decimalIndex) + character;
         }
        else {
            inputString = inputString.substr(1, numPlacesBeforeDecimal) + character + ".00";
        }


        // Convert string back to a vector of characters
        input.assign(inputString.begin(), inputString.end());
    }
    }



void DataEntry::shiftRight(WINDOW* win, const std::string& mask, std::vector<char>& input, int c) {
    // Convert vector of characters to a string
    std::string inputString(input.begin(), input.end());
    std::string returnString = "";

    // Find the index of the decimal point (if exists)
    size_t decimalIndex = inputString.find('.');

    // Move characters to the right by 1, excluding the decimal point
    if (decimalIndex != std::string::npos && decimalIndex > 0 && decimalIndex < input.size() - 1) {
        // Extract characters before and after the decimal point
        std::string charactersBeforeDecimal = inputString.substr(0, decimalIndex);
        std::string charactersAfterDecimal = inputString.substr(decimalIndex + 1);

        // Move characters to the right
        //charactersAfterDecimal = inputString[decimalIndex - 1] + charactersAfterDecimal;
        // Remove the last character after decimal after moving right 
        //charactersAfterDecimal.pop_back();
         // Remove the original character before the decimal point
        charactersBeforeDecimal.pop_back();
       

       
      

        // Combine characters before and after the decimal point
        inputString = charactersBeforeDecimal + "." + charactersAfterDecimal;
        int x = inputString.length();
        // left pad with mask characters to proper length
        returnString = mask.substr(0, (mask.length() - inputString.length())) + inputString;
    }
    input.assign(returnString.begin(), returnString.end());
}

void DataEntry::doDecimal(WINDOW* win, const std::string& mask, bool decimalEntered, std::vector<char>& input, int ch) {
    // Convert vector of characters to a string
    std::string inputString(input.begin(), input.end());
    std::string returnString = "";

    // Find the index of the decimal point (if exists)
    size_t decimalIndex = inputString.find('.');
    if (decimalIndex != std::string::npos && decimalIndex > 0 && decimalIndex < input.size() - 1) {
        // Extract characters before and after the decimal point
        std::string charactersBeforeDecimal = inputString.substr(0, decimalIndex);
        std::string charactersAfterDecimal = inputString.substr(decimalIndex + 1);

        // Combine characters before and after the decimal point
        inputString = charactersBeforeDecimal + "." + charactersAfterDecimal;
        int x = inputString.length();
        // left pad with mask characters to proper length
        returnString = mask.substr(0, (mask.length() - inputString.length())) + inputString;
    }
    else {
        //TODO what should i do if anything?
    }
    
    input.assign(returnString.begin(), returnString.end());
}






