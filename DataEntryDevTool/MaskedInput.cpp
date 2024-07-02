// Version: 6.22.24.12.00
// File: MaskedInput.cpp
#pragma once
#include "DataEntry.h" 
bool DataEntry::MaskedInput(DataEntry& dataEntry, std::ofstream& debugFile)
{
    //std::string input = dataEntry.getFieldValue();
    std::string input = removeMask(dataEntry.getFieldValue());
    input.erase(std::remove(input.begin(), input.end(), '\"'));

    std::string mask = dataEntry.getMask();
    mask.erase(std::remove(mask.begin(), mask.end(), '\"'));
    WINDOW* savewinFullScreen = dataEntry.getwinFullScreen();
    size_t saveLen = dataEntry.getLength();
    int saveRow = dataEntry.getRow();
    int saveColumn = dataEntry.getFieldColumn();
    int rows = savewinFullScreen->_maxy;
    int cols = savewinFullScreen->_maxx;
    keypad(savewinFullScreen, TRUE); // Enable arrow keys for win
    //std::string mask = "(999) 999-9999";
    //std::string mask = "99/99/9999";
    std::string displayMaskStr = generateDisplayMask(mask);
    wattroff(savewinFullScreen, COLOR_PAIR(3));
    wattron(savewinFullScreen, COLOR_PAIR(2));
    displayMask(dataEntry, input, debugFile);

    // Set cursor to the first '9' position in the mask
    int pos = 0;
    while (pos < mask.size() && mask[pos] != '9') {
        ++pos;
    }
    wmove(savewinFullScreen, saveRow, saveColumn+pos);
    wrefresh(savewinFullScreen);

    
    int ch = 0;
    int column = saveColumn;
    int charsEntered = 0;
    int inputPos = 0;
    bool isBackspace = false;   
    bool quit = false;
    bool GotoRestart = false;
    bool firstpass = true;
    std::string Action = "";
    wattron(savewinFullScreen, COLOR_PAIR(3));
    wrefresh(savewinFullScreen);
    noecho(); // Disable echoing of characters
    wrefresh(savewinFullScreen);
    while (quit != true)
    {

        wattroff(savewinFullScreen, COLOR_PAIR(3));
        wattron(savewinFullScreen, COLOR_PAIR(2));
        wrefresh(savewinFullScreen);

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
            isBackspace = true;
            if (isBackspace == true && pos>0) {
                while (pos > 0 && mask[pos - 1] != '9') {
                    --pos; // Move to the previous '9' position in the mask
                }
                if (pos > 0) {
                    --pos;
                    input.erase(std::count(mask.begin(), mask.begin() + pos, '9'), 1); // Erase character from input
                }
            }
            displayMask(dataEntry, input, debugFile);
            break;
#pragma endregion // end of logic to deal with delete key or known values representing backspace
                    // This is the end of the core logic that handles filtering funtion key or special key 
                    // we have determined the key pressed  by user is a digit
                    //---------------------------------------------------------
#pragma region //beginning of the core logic that handles building the masked input field.

        default:
            
            bool digit = false;
            if (ch >= 0 && ch <= 127)
            {
                if (isdigit(ch))
                    digit = true;
            }
            if (digit == true) {
                if (pos < mask.size()) {
                    while (pos < mask.size() && mask[pos] != '9') {
                        ++pos; // Move to the next '9' position in the mask
                    }

                    if (pos < mask.size() && mask[pos] == '9') {
                        int inputPos = std::count(mask.begin(), mask.begin() + pos, '9');
                        if (inputPos < input.size()) {
                            input[inputPos] = ch; // Replace digit at correct position in input
                        }
                        else {
                            input.insert(inputPos, 1, ch); // Insert digit at correct position in input
                        }
                        ++pos; // Move to the next position
                    }
                }
            }
               
                else if (ch == KEY_LEFT && pos > 0) {
                    do {
                        --pos; // Move left to the previous position
                    } while (pos > 0 && mask[pos] != '9');
                }
                else if (ch == KEY_RIGHT && pos < mask.size() - 1) {
                    do {
                        ++pos; // Move right to the next position
                    } while (pos < mask.size() && mask[pos] != '9');
                }

                displayMask(dataEntry, input, debugFile);
                if (pos <= mask.size()) {
                    wmove(savewinFullScreen, saveRow, saveColumn + pos ); // Move cursor to the next input position
                }
                wrefresh(savewinFullScreen);
        }
    }
        
    
    

    if (quit == true) {
       
        std::string inputResult = combineInputWithMask(mask, input);
        dataEntry.setFieldValue(inputResult);
        dataEntry.setFieldValue(input);
        dataEntry.setInputKeyPressed(Action);
        wattroff(savewinFullScreen, COLOR_PAIR(2));
        wattron(savewinFullScreen, COLOR_PAIR(3));
        wrefresh(savewinFullScreen);
        
        dataEntry.displayData(); //jg 6/27
        // true indicates a funtion was  pressed and InputKeyPressed indicates which one
        return true;

    }


exitField:    
    std::string inputResult = combineInputWithMask(mask, input);
    dataEntry.setFieldValue(inputResult);    
    dataEntry.setInputKeyPressed(Action);
    dataEntry.displayData();
    // false indicates exit field and  a funtion was NOT  pressed
    // most likely enter or up arrow got us here
    return false;
}