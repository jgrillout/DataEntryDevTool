// Version: 7.2.24.10.06
// File: NumericInput.cpp
#pragma once
#include "DataEntry.h" 

bool DataEntry::NumericInput(DataEntry& dataEntry, std::ofstream& debugFile)
{
    std::string input = dataEntry.getFieldValue();
    input.erase(std::remove(input.begin(), input.end(), '\"'));

    std::string mask = dataEntry.getMask();
    mask.erase(std::remove(mask.begin(), mask.end(), '\"'));
    WINDOW* savewinFullScreen = dataEntry.getwinFullScreen();
    size_t saveLen = dataEntry.getLength();
    int saveRow = dataEntry.getRow();
    int saveColumn = dataEntry.getFieldColumn();
    int rows = savewinFullScreen->_maxy;
    int cols = savewinFullScreen->_maxx;
    
    int leftSize = 0;
    int rightSize = 0;

    inspectMask(mask, leftSize, rightSize);
    
    
    displayRightToLeft(savewinFullScreen, input, saveRow, saveColumn, saveLen);

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
    wattron(savewinFullScreen, COLOR_PAIR(3));
    wrefresh(savewinFullScreen);
    noecho(); // Disable echoing of characters
    wrefresh(savewinFullScreen);
    while (quit != true)
    {
        //jrg
        wattroff(savewinFullScreen, COLOR_PAIR(3));
        wattron(savewinFullScreen, COLOR_PAIR(2));
        wrefresh(savewinFullScreen);

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



        
        ch = wgetch(savewinFullScreen);

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
            wrefresh(savewinFullScreen);
            quit = true;
            break;
        case KEY_F(8):
            Action = "KEY_F(8)";
            wrefresh(savewinFullScreen);
            quit = true;
            break;
        case KEY_F(9):
            Action = "KEY_F(9)";
            wrefresh(savewinFullScreen);
            quit = true;
            break;
        case KEY_F(10):
            Action = "KEY_F(10)";
            wrefresh(savewinFullScreen);
            quit = true;
            break;
        case KEY_UP:
            Action = "KEY_UP";
            wrefresh(savewinFullScreen);
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

        displayRightToLeft(savewinFullScreen, input, saveRow, saveColumn, saveLen);
    }
    if (quit == true) {
        //std::reverse(input.begin(), input.end());        
        dataEntry.setFieldValue(input);//??
        dataEntry.setInputKeyPressed(Action);
        wattroff(savewinFullScreen, COLOR_PAIR(2));
        wattron(savewinFullScreen, COLOR_PAIR(3));
        wrefresh(savewinFullScreen);
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
    //inputResult = input;
    //dataEntry.setFieldValue(inputResult);

    dataEntry.setFieldValue(input);
    dataEntry.setInputKeyPressed(Action);

    dataEntry.displayData();

    //displayNumeric(dataEntry,debugFile,0,false);


    return false;// indicates exit field and  a funtion was NOT  pressed
}

