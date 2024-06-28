//  Version: 6.28.24.09.24
// File: StringInput.cpp
#pragma once
#include "DataEntry.h"
bool DataEntry::stringInput(DataEntry& dataEntry, std::ofstream& debugFile) {
    /*Note: parameter names are diffent then the declartions so they match
    the names of code i borrowed from a business BASIC program GENINPUT.PUB*/
    std::string X$ = "";
    std::string EDIT$ = "";
    std::string Action = "";
    std::string before = "";
    std::string after = "";
    WINDOW* saveWin = dataEntry.getWin();
    size_t saveLen = dataEntry.getLength();
    int saveRow = dataEntry.getRow();
    int saveColumn = dataEntry.getFieldColumn();
    int rows = saveWin->_maxy; 
    int cols = saveWin->_maxx;
    int POS = 0;
    int SAVEPOS = 0;//size_t
    bool MASKED = false;
    bool END = false;
    bool INSERT = false;
    int inputSize = 0;//size_t
    int index = 0;//size_t
    // retrieve existing value (if any)
    EDIT$ = dataEntry.getFieldValue();

    inputSize = EDIT$.size(); // Size of the initial value
    if (inputSize < saveLen) {
        EDIT$.append(saveLen - inputSize, ' ');
    }
    POS = EDIT$.find_last_not_of(' ') + 1;
    SAVEPOS = POS;   
    // Enable keypad mode to capture function keys
    keypad(saveWin, TRUE);
    int c=0;
    bool quit = false;    
    //wattron(saveWin, COLOR_PAIR(2));

    mvwprintw(saveWin, saveRow, saveColumn, EDIT$.c_str());

    //wattroff(saveWin, COLOR_PAIR(2));
    //wrefresh(saveWin);
    // Capture user input until Enter key is pressed
    while (quit != true) {
        wattron(saveWin, COLOR_PAIR(2));
        //wrefresh(saveWin);
        mvwprintw(saveWin, saveRow, saveColumn, EDIT$.c_str());
        wrefresh(saveWin);
        //wattroff(saveWin, COLOR_PAIR(2));
        //wattron(saveWin, COLOR_PAIR(3));
        //wrefresh(saveWin);

//GETKEY:
        c = wgetch(saveWin);
        std::string action = keyname(c);
#pragma region //start of switch
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
// This is the core logic that handles the editing of the string input field 
        case KEY_LEFT:
            if (POS == 0) {
                beep();
            }
            else {
                --POS;
                
            }
            break;
        case KEY_RIGHT:            
            if (POS+1 >= saveLen) {
                beep();
                POS = saveLen;
            }
            else
                POS++;
            break;
        case KEY_END:            
            index = EDIT$.find_last_not_of(' ');
            POS = index + 1;
            break;
        case KEY_HOME:          
            POS = 0; 
            break;
        case KEY_DC:  // delete    
            before = EDIT$.substr(0, POS - 1);
            after = EDIT$.substr(POS + 1);
            EDIT$ = before + " " + after;
            break;       
        case PAD0: //INSERT  
        case 331: //INSERT
            INSERT = !INSERT;
            if (INSERT == true) {
                wattron(saveWin, COLOR_PAIR(2));
                mvwprintw(saveWin, rows - 2, cols - 8, "Insert");
            }
            else {
                wattron(saveWin, COLOR_PAIR(3));
                mvwprintw(saveWin, rows - 2, cols - 8, "       ");
                wattron(saveWin, COLOR_PAIR(2));
            }
            break;
        case 8:// Handle backspace
            if (POS == 0)
                beep();
            else {
                SAVEPOS = POS;
                --POS;
            }
            if (SAVEPOS != POS) {
                if (POS != saveLen) {

                    std::string before = EDIT$.substr(0, POS);
                    std::string after = EDIT$.substr(POS + 1) + " ";
                    EDIT$ = before + after;
                }
            }
            break;

        default:
            if (c >= 0 && c <= 127)
            {
                if (POS > saveLen) {
                    beep();
                    break;
                }
            }
            else {
                Action = "KEY_NOT";// deal with keys i haven't planned on handling
                wrefresh(saveWin);
                quit = true;
                break;
            }
// This is the end of the core logic that handles editing of the string input field  
// we have determined the key pressed is not a funtion key or special key 
// Rather the key pressed is by user was a printable character (a..z,A..Z,0..9 etc)  
//
// This is the  beginning of the core logic that handles building the string input field. 
           
            if (std::isprint(c)) {
                char character = static_cast<char>(c);
                /*if (allowed(dataEntry.getFieldType(), character, EDIT$) == false)
                    break;*/

                std::string chString(1, character);

                if (INSERT == true)
                {
                    if (POS != character) {
                        X$ = EDIT$.substr(POS, 1);
                        before = EDIT$.substr(0, POS);
                        after = EDIT$.substr(POS);
                        EDIT$ = before + chString + after;
                        EDIT$ = EDIT$.substr(0, character);
                    }
                    else
                        beep();
                }

                else
                    EDIT$.replace(POS, 1, chString);
             
                POS++;
                if (POS > saveLen) {
                    END = 1;
                    POS = saveLen;
                }

                wrefresh(saveWin);
            }
        }
#pragma endregion //end of switch
    }

    //traceoff();
    if (quit == true) {
        dataEntry.setFieldValue(EDIT$);    
        dataEntry.setInputKeyPressed(Action);             
        // indicates a funtion was  pressed and InputKeyPressed indicates which one
            return true;
        
    }

exitField:
    //Action = "exitField_NOT_KEY_F";
   // wrefresh(saveWin);    

    
    dataEntry.setFieldValue(EDIT$);
    dataEntry.setInputKeyPressed(Action);
    
    //wattroff(saveWin, COLOR_PAIR(2));
    //wattron(saveWin, COLOR_PAIR(3));
    //wrefresh(saveWin);
    return false;// indicates we have a string indicates a funtion was NOT  pressed
    
};

