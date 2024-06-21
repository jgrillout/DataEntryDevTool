// Version: 6.5.24.16.32
// File: NumericInputController.cpp
#pragma once
#include "DataEntry.h" 
//#include <vector>
//#include <string>
//#include <iostream>
//#include <numeric>
const int INVALID_INDEX = -1;
std::string DataEntry::numtest;
bool DataEntry::NumericInput(DataEntry& dataEntry, std::ofstream& debugFile)
{
    std::string input = dataEntry.getFieldValue();
    input.erase(std::remove(input.begin(), input.end(), '\"'));

    std::string mask = dataEntry.getMask();
    mask.erase(std::remove(mask.begin(), mask.end(), '\"'));
    WINDOW* saveWin = dataEntry.getWin();
    size_t saveLen = dataEntry.getLength();
    int saveRow = dataEntry.getRow();
    int saveColumn = dataEntry.getFieldColumn();
    int rows = saveWin->_maxy;
    int cols = saveWin->_maxx;
    std::string reverseInput = "";
    bool edit = false;
    // jrg
    //std::string blanks(saveLen + 1, ' ');
    std::string blanks(saveLen, ' ');
    if (input.empty()) {
        std::string reverseInput = "";
        edit = false;
    }
    else {
        std::string reverseInput = input;
        std::reverse(reverseInput.begin(), reverseInput.end());
        edit = true;
    }


    int inputSize = input.size(); // Size of the initial value
    if (inputSize < saveLen) {
        input.append(saveLen - inputSize, ' ');
    }
    wattron(saveWin, COLOR_PAIR(2));
    wrefresh(saveWin);

    //jrg
    //mvwprintw(saveWin, saveRow, saveColumn, blanks.c_str());
    mvwprintw(saveWin, saveRow, saveColumn, input.c_str());
    //wrefresh(saveWin);    
    //wattroff(saveWin, COLOR_PAIR(2));
    wattron(saveWin, COLOR_PAIR(3));
    wrefresh(saveWin);
    //jrg
//Restart:
//    dataEntry.setFieldValue(input);
//    //input.assign(input.begin(), input.end());
//
//    dataEntry.displayData();
    //jrg
    int ch = 0;
    int column = saveColumn;
    int charsEntered = 0;
    //int inputIndex = 0;
    bool isBackspace = false;
    bool decimalEntered = false;
    bool quit = false;
    bool GotoRestart = false;
    bool hasDecimalPoint = false;
    bool firstpass = true;
    std::string Action = "";
    wattron(saveWin, COLOR_PAIR(3));
    //wrefresh(saveWin);
    noecho(); // Disable echoing of characters
    wrefresh(saveWin);
    while (quit != true)
    {
        //jrg
        //wattroff(saveWin, COLOR_PAIR(3));
        wattron(saveWin, COLOR_PAIR(2));
        //wrefresh(saveWin);

        if (firstpass == true) {
            int inputSize = input.size(); // Size of the initial value
            if (inputSize < saveLen) {
                input.append(saveLen - inputSize, ' ');
            }
            mvwprintw(saveWin, saveRow, saveColumn, input.c_str());
            wattroff(saveWin, COLOR_PAIR(2));
            //wattron(saveWin, COLOR_PAIR(3));
            wrefresh(saveWin);
            firstpass = false;
        }



        //jrg
        ch = wgetch(saveWin);

#pragma region //start of switch
        switch (ch)
        {
        case PADENTER:
        case 10:  // Enter key    
            Action = "KEY_ENTER";
            quit = true;
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
            goto  exitField; //jrg
            break;
#pragma region// deal with delete key or known values representing backspace
        case KEY_DC:  // delete key dec 330 hex 14
            Action = "KEY_DC";
        case 8: // backspace
            Action = "CASE_8";
        case 127:// backspace
            Action = "CASE_127";
        case 462:// backspace
            Action = "CASE_462";
            if (!input.empty()) {
                if (input[0] == '.') {
                    hasDecimalPoint = false;
                }
                input.erase(input.begin());
            }
            isBackspace = true;


#pragma endregion // end of logic to deal with delete key or known values representing backspace
            // This is the end of the core logic that handles filtering funtion key or special key 
            // we have determined the key pressed  by user is a digit
            //---------------------------------------------------------
#pragma region //beginning of the core logic that handles building the numeric input field.
            break;
        default:
            bool digit = false;
            // prevents error caused by testing isdigit 
            // when funtion or control keys are pressed
            if (ch >= 0 && ch <= 127)
            {
                if (isdigit(ch))
                    digit = true;
                DataEntry::removeTrailingSpaces(input);


            }

            if (digit == true) {
                if (input.size() < saveLen) {
                    input.insert(input.begin(), ch);
                    reverseInput.insert(reverseInput.end(), ch);
                }
            }
            else if (ch == '.' && !hasDecimalPoint) {
                if (input.size() < saveLen - 2) {
                    input.insert(input.begin(), ch);
                    reverseInput.insert(reverseInput.end(), ch);
                    hasDecimalPoint = true;
                }
            }

#pragma endregion //end of switch
        }

        if (isBackspace) {
            //std::string blanks(saveLen + 1, ' ');
            mvwprintw(saveWin, saveRow, saveColumn, blanks.c_str());
            wrefresh(saveWin);
        }
        //std::string blanks(saveLen + 1, ' ');
        //std::string blanks(saveLen, ' ');
        mvwprintw(saveWin, saveRow, saveColumn, blanks.c_str());
        wrefresh(saveWin);

        //displayNumeric(dataEntry, debugFile, 1, true);
        int adjustColumn = 1;
        int col2 = saveColumn + saveLen - adjustColumn;
        wattron(saveWin, COLOR_PAIR(2));

        // Display the buffer content right-to-left
        inputResult = input;
        //std::reverse(inputResult.begin(), inputResult.end());
        for (int i = 0; i < inputResult.size(); ++i) {
        //for (int i = 0; i < reverseInput.size(); ++i) {
            //mvwaddch(saveWin, saveRow, col2 - i, reverseInput[i]);
            mvwaddch(saveWin, saveRow, col2 - i, inputResult[i]);
            wmove(saveWin, saveRow, col2);
            wrefresh(saveWin);
        }

        //wrefresh(saveWin);
//jrg
    }
    if (quit == true) {
        //std::reverse(input.begin(), input.end());
        inputResult = input;
        dataEntry.setFieldValue(inputResult);//??
        inputAction = Action;
        //wattroff(saveWin, COLOR_PAIR(2));
        //wattron(saveWin, COLOR_PAIR(3));
        //wrefresh(saveWin);
        // indicates a funtion was  pressed and inputAction indicates which one
        return true;

    }


exitField:

    //wrefresh(saveWin);
    if (!reverseInput.empty()){
        std::reverse(reverseInput.begin(), reverseInput.end()); //??
        inputResult = reverseInput;
    }
    else {
        std::reverse(input.begin(), input.end()); //??
        inputResult = input;
    }
    dataEntry.setFieldValue(inputResult);
    inputAction = Action;

    dataEntry.displayData();

    //displayNumeric(dataEntry,debugFile,0,false);


    return false;// indicates exit field and  a funtion was NOT  pressed
}

//void  DataEntry::displayNumeric(DataEntry& dataEntry, std::ofstream& debugFile, int adjustColumn, bool highlight) {
//    WINDOW* saveWin = dataEntry.getWin();
//    size_t saveLen = dataEntry.getLength();
//    int saveRow = dataEntry.getRow();
//    int saveColumn = dataEntry.getFieldColumn();
//    std::string mask = dataEntry.getMask();
//    mask.erase(std::remove(mask.begin(), mask.end(), '\"'));
//    //std::string blanks(saveLen + 1, ' ');
//    std::string blanks(saveLen, ' ');
//
//    std::string displayNum = dataEntry.getFieldValue();
//    //if (highlight == true)
//        std::reverse(displayNum.begin(), displayNum.end());
//    debugFile << "displayNumeric " << "saveColumn = " << saveColumn << std::endl;
//    debugFile << "displayNum = " << displayNum << std::endl;
//    mvwprintw(saveWin, saveRow, saveColumn, blanks.c_str());
//    wrefresh(saveWin);
//    int col2 = saveColumn + saveLen - adjustColumn;
//
//    if (highlight == true)
//        wattron(saveWin, COLOR_PAIR(2));
//    else
//        wattron(saveWin, COLOR_PAIR(3));
//
//    mvwprintw(saveWin, saveRow, saveColumn, blanks.c_str());
//    wrefresh(saveWin);
//    // Display the buffer content right-to-left
//    for (int i = 0; i < displayNum.size(); ++i) {
//        mvwaddch(saveWin, saveRow, col2 - i, displayNum[i]);
//        wmove(saveWin, saveRow, col2 - i);
//        wrefresh(saveWin);
//    }
//    if (highlight == true)
//        wattron(saveWin, COLOR_PAIR(3));// turn off highlight
//
//    wmove(saveWin, saveRow, col2);
//    wrefresh(saveWin);
//}
#include <curses.h>
#include <string>
#include <iostream>

void displayRightToLeft(WINDOW* win, const std::string& input, int row, int col, int inputSize) {
    werase(win);
    box(win, 0, 0);
    int startPos = col - inputSize + input.size(); // Calculate the starting position for display
    for (int i = 0; i < input.size(); ++i) {
        mvwaddch(win, row, startPos + i, input[i]);
    }
    wrefresh(win);
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int winHeight = 10;
    int winWidth = 30;
    int startY = 5;
    int startX = 5;
    int inputSize = 10; // Fixed size for input display
    int displayCol = 20; // Column where the input display should start

    WINDOW* win = newwin(winHeight, winWidth, startY, startX);
    box(win, 0, 0);
    wrefresh(win);

    std::string input;
    int ch;

    while ((ch = wgetch(win)) != '\n') { // Enter key to stop input
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (!input.empty()) {
                input.pop_back();
            }
        }
        else if (std::isdigit(ch)) {
            input.push_back(ch);
        }
        displayRightToLeft(win, input, 1, displayCol, inputSize);
    }

    endwin();

    // Print the captured input to the console
    std::cout << "Input: " << input << std::endl;

    return 0;
}

