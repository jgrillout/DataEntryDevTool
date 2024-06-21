// Version: 6.20.24.20.03
// File: NumericInput.cpp
#pragma once
#include "DataEntry.h" 

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
    //std::string reverseInput = "";
    //bool edit = false;
    int leftSize = 0;
    int rightSize = 0;

    inspectMask(mask, leftSize, rightSize);
    
    
    displayRightToLeft(saveWin, input, saveRow, saveColumn, saveLen);

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
    wrefresh(saveWin);
    noecho(); // Disable echoing of characters
    wrefresh(saveWin);
    while (quit != true)
    {
        //jrg
        wattroff(saveWin, COLOR_PAIR(3));
        wattron(saveWin, COLOR_PAIR(2));
        wrefresh(saveWin);

        //if (firstpass == true) {
        //    int inputSize = input.size(); // Size of the initial value
        //    if (inputSize < saveLen) {
        //        input.append(saveLen - inputSize, ' ');
        //    }
        //    mvwprintw(saveWin, saveRow, saveColumn, input.c_str());
            //wattroff(saveWin, COLOR_PAIR(2));
            //wattron(saveWin, COLOR_PAIR(3));
            //wrefresh(saveWin);
        //    firstpass = false;
        //}



        
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
            /*if (!input.empty()) {
                if (input[0] == '.') {
                    hasDecimalPoint = false;
                }
                input.erase(input.begin());
            }*/
            if (!input.empty()) {
                input.pop_back();
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
                //DataEntry::removeTrailingSpaces(input);


            }

            if (digit == true) {
                size_t decimalPos = input.find('.');
                if (decimalPos == std::string::npos) {
                    // No decimal point yet
                    if (input.size() < leftSize) {
                        input.push_back(ch);
                    }
                    else if (input.size() == leftSize) {
                        // Automatically add the decimal point
                        input.push_back('.');
                        input.push_back(ch);
                    }
                }
                else {
                    // Decimal point already present
                    if (input.size() - decimalPos - 1 < rightSize) {
                        input.push_back(ch);
                    }
                }
            }
            //---------------------------------------------
            else if (ch == '.' && input.find('.') == std::string::npos && input.size() <= leftSize) {
                // Allow manually entering the decimal point if it's not already present and it's valid to add it
                input.push_back(ch);
            }

#pragma endregion //end of switch
        }

        //if (isBackspace) {
        //    //std::string blanks(saveLen + 1, ' ');
        //    mvwprintw(saveWin, saveRow, saveColumn, blanks.c_str());
        //    wrefresh(saveWin);
        //}
        ////std::string blanks(saveLen + 1, ' ');
        ////std::string blanks(saveLen, ' ');
        //mvwprintw(saveWin, saveRow, saveColumn, blanks.c_str());
        //wrefresh(saveWin);

        displayRightToLeft(saveWin, input, saveRow, saveColumn, saveLen);
    }
    if (quit == true) {
        //std::reverse(input.begin(), input.end());
        inputResult = input;
        dataEntry.setFieldValue(inputResult);//??
        inputAction = Action;
        wattroff(saveWin, COLOR_PAIR(2));
        wattron(saveWin, COLOR_PAIR(3));
        wrefresh(saveWin);
        // indicates a funtion was  pressed and inputAction indicates which one
        return true;

    }


exitField:

    ////wrefresh(saveWin);
    //if (!reverseInput.empty()){
    //    std::reverse(reverseInput.begin(), reverseInput.end()); //??
    //    inputResult = reverseInput;
    //}
    //else {
    //    std::reverse(input.begin(), input.end()); //??
    //    inputResult = input;
    //}
    inputResult = input;
    dataEntry.setFieldValue(inputResult);
    inputAction = Action;

    dataEntry.displayData();

    //displayNumeric(dataEntry,debugFile,0,false);


    return false;// indicates exit field and  a funtion was NOT  pressed
}

