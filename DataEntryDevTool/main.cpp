// Version: 5.20.24.9.46
// File: main.cpp
#pragma once
#include "DataEntry.h"
#include "ISAMWrapperLib.h"
#include <vector>
#include <iostream>
#include <fstream>
int main(int argc, char* argv[]) {

    /* if (argc < 2) {
         std::cout << "No screen information provided." << std::endl;
         return 1;
     }
     std::string arg1 = argv[1];*/

    std::string arg1 = "EmployeeScreen";

    namespace fs = std::filesystem;
    // Open the text file
    fs::path currentPath = fs::current_path();
    std::string screenarg = currentPath.string();
    screenarg.append("\\" + arg1 + ".xml");
    //std::cout << "screenarg is " << screenarg << std::endl;
    std::ifstream xmlFile(screenarg);
    if (!xmlFile.is_open()) {
        std::cerr << "Failed to open " + screenarg + " file." << std::endl;
        return 1;
    }


    size_t pos = arg1.find("Screen");
    std::string screenTitle = arg1.substr(0, pos);
    int rows=24, cols=80;
    //std::cout << "screenTitle is " << screenTitle << std::endl;

    // Initialize curses    
    initscr();
    //cbreak();
    noecho();
    start_color();

    getmaxyx(stdscr, rows, cols);

    // Define color pairs for input field
    // white background, blue foreground
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    // Define color pairs for window
    //blue background, white forground
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    bkgd(COLOR_PAIR(3));

    WINDOW* winFullScreen = newwin(rows, cols, 0, 0);
    assert(winFullScreen != NULL);
    /*wattron(winFullScreen, COLOR_PAIR(3));
    wbkgd(winFullScreen, COLOR_PAIR(3));
    box(winFullScreen, ACS_VLINE, ACS_HLINE);*/
    //wrefresh(winFullScreen);

    WINDOW* winFormArea = derwin(winFullScreen, 20, 78, 1, 1);
    assert(winFormArea != NULL);
    wattron(winFormArea, COLOR_PAIR(3));
    wbkgd(winFormArea, COLOR_PAIR(3));
    //wrefresh(winFormArea);
    keypad(winFormArea, TRUE);

    WINDOW* winMsgArea = newwin(4, 78, 19, 1);
    assert(winMsgArea != NULL);
    wattron(winMsgArea, COLOR_PAIR(3));
    wbkgd(winMsgArea, COLOR_PAIR(3));
    box(winMsgArea, ACS_VLINE, ACS_HLINE);
    keypad(winMsgArea, TRUE);

    wrefresh(winMsgArea);
    wrefresh(winFormArea);
    wrefresh(winFullScreen);

    if (DataEntry::validateXml(winFullScreen, winMsgArea, xmlFile)) {

        //std::string err="XML file is correctly formatted.";

    }
    else {
        std::string err = "XML file validation failed.";
        DataEntry::errMsg(winMsgArea, 0, 0, err);
        DataEntry::confirmAction(winMsgArea, winFullScreen, 2, 2, 78, "Exit", COLOR_PAIR(1), KEY_F(7));

        return 1;
    }

    // Set the record pointer to the beginning of the file
    xmlFile.clear();  // Clear any error flags that may have been set during reading
    xmlFile.seekg(0, std::ios::beg);  // Set the position to the beginning of the file

    std::vector<DataEntry> fields;
    std::string condition = "";
    std::string temp = "";

    // Read screen data from the file and initialize DataEntry objects
    if (DataEntry::SetupFields(winFormArea, fields, xmlFile) == false) {
        delwin(winFormArea);
        endwin();
        //std::cout << "setup_fields failed " << std::endl;
        return 1;
    }

    std::vector<std::string> fieldNames;
    for (const auto& entry : fields) {
        std::string value = entry.getfieldName();
        std::replace(value.begin(), value.end(), ' ', '_');  // Replace spaces with underscores
        fieldNames.push_back(value);
    }
    sqlite3* db = nullptr;
    std::string dbname = currentPath.string() + "\\"+arg1 + ".db";
    
    ISAMWrapperLib lib(dbname);
    lib.createTable(arg1, fieldNames);


    int rc = sqlite3_open(dbname.c_str(), &db);
    if (rc) {

        // Handle error
        //std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
    }
    // Check if the table exists   
    bool tableExists = false;
    sqlite3_stmt* stmt;
    std::string tbl = arg1 + ".csv";
    std::string query = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + arg1 + "';";
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        int result = sqlite3_step(stmt);
        if (result == SQLITE_ROW || result == SQLITE_DONE) {
            tableExists = true;
        }
    }
    sqlite3_finalize(stmt);
    //if (tableExists) {
    //    std::string outname = arg1 + ".csv";
    //    if (!DataEntry::exportTableToCSV(db, arg1, outname)) {
    //        //std::cerr << "Error exporting table data to CSV file." << std::endl;
    //        sqlite3_close(db);
    //        return 1;
    //    }
    //}
    //--------------------------------------------
    query = "SELECT * FROM " + arg1 + ";";

    // Execute the query
    //sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    std::string emsg = sqlite3_errmsg(db);
    if (rc != SQLITE_OK) {
        //std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }





RecordEntryStart:
    std::ofstream debugFile("debug.txt");
    std::string Result = "";
    wclear(winFullScreen);
    wattron(winFullScreen, COLOR_PAIR(3));
    wbkgd(winFullScreen, COLOR_PAIR(3));
    box(winFullScreen, ACS_VLINE, ACS_HLINE);
    wattron(winFullScreen, A_REVERSE);
    mvwprintw(winFullScreen, 22, 2, "F2 Lookup");
    mvwprintw(winFullScreen, 22, 12, "F3 Delete");
    mvwprintw(winFullScreen, 22, 22, "F4 Save");
    mvwprintw(winFullScreen, 22, 32, "F5 Restart");
    mvwprintw(winFullScreen, 22, 45, "F7 Exit");
    wattroff(winFullScreen, A_REVERSE);
    wrefresh(winFullScreen);

    bool firstTime = true, quit = false;
    bool EditingRecord = false, AddingNew = false;
    int test = 0;
    std::string msg = "";
    // Display all fields Labels & defualt data
    for (auto& field : fields) {

        field.displayLabels();
        field.displayData();
    }
    wrefresh(winFormArea);

    size_t  index = 0;
    std::vector<std::string>  fieldValues;


    debugFile << "before  While " << std::endl;
    while (index < fields.size()) {
    BeginField:

        auto& field = fields[index];

        //**********ACCEPT INPUT ************
        field.AcceptInput(field);
        if (field.inputAction == "exitField_NOT_KEY_F")
            goto notFunctionKey;

        temp = field.getFieldValue();
        if (field.inputAction == "KEY_F(2)" || field.inputAction == "KEY_F(5)" || field.inputAction == "KEY_F(7)")
            goto functionKeys;
        // first field should always be the primary key and cant allow blank value
        if (index == 0 && DataEntry::removeLeadingSpaces(temp) == "") {
            goto BeginField;
        }
        field.setFieldValue(temp);
        fieldValues.push_back(temp);
        field.displayData();
        /* std::string tempnam = field.getfieldName();
         std::string tempval = field.getFieldValue();
         debugFile << "index = " << index << " tempnam = " << tempnam << " tempval " << tempval << std::endl;*/
        wrefresh(winFormArea);
    functionKeys:
        Result = DataEntry::doFunctionKeys(winFullScreen, winMsgArea, arg1, AddingNew, lib, condition, fields);
        if (Result == "Exit")
            goto Exit;

        if (Result == "Error")
            goto FunctionKeyError;
        if (Result == "TryReadRecord") {
            // condition is set in doFunctionKeys
            //condition = fieldNames[0] + " = \'" + fields[0].getFieldValue() + "\'";
            Result = "";
            AddingNew = false;
            EditingRecord = false;
            bool prompt = false;
            Result = DataEntry::readRecord(winFullScreen, winMsgArea, arg1, AddingNew, prompt, lib, condition, fields);
            if (Result == "Error")
                goto handleError;
            if (Result == "RecordNotFound" || Result == "RecordEntryStart") {
                index = 0;
                for (auto& field : fields) {
                    if (index > 0) {
                        field.setFieldValue("");
                        index++;
                    }


                }
                goto displayData;
            }
            if (Result == "EditingRecord") {
                AddingNew = false;
                EditingRecord = true;
                if (index == 0)
                    index = 1;
                goto displayData;


            }

            if (Result == "RecordEntryStart") {
                for (auto& field : fields) {
                    field.setFieldValue("");
                }
                goto RecordEntryStart;
            }
        }
        // yes this is needed again
        if (Result == "RecordEntryStart") {
            for (auto& field : fields) {
                field.setFieldValue("");
            }
            goto  RecordEntryStart; // displayData; //RecordEntryStart;
        }
    

        //goto RecordEntryStart;  //ideally it shouldn't reach this line but just in case
    
    notFunctionKey:

        test = DataEntry::stringSwitchNumber(field.inputAction);
        if (test != -99) {
            switch (test) {
            case -1:  //"KEY_UP"       
                    //debugFile << "KEY_UP" << std::endl;
                if (index > 1) {
                    --index;
                    auto& field = fields[index];
                    /*std::string tempnam = field.getfieldName();
                    std::string tempval = field.getFieldValue();
                    debugFile << "index = " << index << " tempnam = " << tempnam << " tempval " << tempval << std::endl;*/
                    goto BeginField;
                }
                else
                        index = 1;
                        goto BeginField;
            
#pragma region //start of code for 'KEY_ENTER' case   
            case 0: //KEY_ENTER
            case PADENTER:
            case 16://KEY_ENTER under special circumstance not sure why getch returned 16
                 // here we can check the value input by the user after they press enter
                 // 
                 //primary key is should always be the first field
                if (index == 0 && field.getfieldName() == fieldNames[index]) {

                    condition = fieldNames[0] + " = \'" + fields[0].getFieldValue() + "\'";
                    Result = "";
                    AddingNew = false;
                    EditingRecord = false;
                    bool prompt = true;
                    Result = DataEntry::readRecord(winFullScreen, winMsgArea, arg1, AddingNew,prompt, lib, condition, fields);
                    if (Result == "Error")
                        goto handleError;
                    if (Result == "RecordNotFound" || Result == "RecordEntryStart") {
                        index = 0;
                        for (auto& field : fields) {
                            if (index > 0) {
                                field.setFieldValue("");
                                index++;
                            }

                        }
                    }
                    if (Result == "EditingRecord") {
                        AddingNew = false;
                        EditingRecord = true;
                        if (index == 0)
                            index = 1;
                        goto displayData;


                    }
                    if (Result == "AddNewRecord") {
                        if (index == 0)
                            index = 1;
                        AddingNew = true;
                        EditingRecord = false;
                        wattron(winFormArea, COLOR_PAIR(3));
                        mvwprintw(winFormArea, field.getRow(), field.getFieldColumn(), field.getFieldValue().c_str());
                        wrefresh(winFormArea);
                        goto BeginField;
                    }
                    if (Result == "RecordEntryStart") {
                        index = 0;
                        for (auto& field : fields) {
                            if (index > 0) {
                                field.setFieldValue("");
                                index++;
                            }

                        }
                        goto RecordEntryStart;
                    }


                }

                debugFile << "KEY_ENTER index " << index << std::endl;
                //goto displayData
                wattron(winFormArea, COLOR_PAIR(3));
                mvwprintw(winFormArea, field.getRow(), field.getFieldColumn(), field.getFieldValue().c_str());
                wrefresh(winFormArea);
                index++;
#pragma endregion // end of code for 'KEY_ENTER' case
            }
        }

    }
    
    // if we reached this point that all field have been entered
    // loop back around to 2nd field on screen (without clearing fields) 
    // another option,instead of looping, would be to prompt the user to save the record
    index = 1; // index 1 is second field we don't want index 0  because it's the primary key field
    goto BeginField;

    displayData:
        for (auto& field : fields) {

            field.displayData();
        }
        wrefresh(winFormArea);
        wattroff(winFormArea, COLOR_PAIR(2));
        if (EditingRecord) {
            index = 1;
            goto BeginField;
           /* if (index < fields.size() - 1) {
                index++;
                goto BeginField;
            }*/
        }
        if (!AddingNew)
            goto RecordEntryStart;
        else {
            if (index < fields.size() - 1) {
                index++;
                goto BeginField;
            }
        }
        goto BeginField;
        

FunctionKeyError:
            msg = "FunctionKeyError occured";
            DataEntry::errMsg(winMsgArea, 2, 78, msg);
            if (DataEntry::confirmAction(winMsgArea, winFullScreen, 2, 2, 78, "Exit", COLOR_PAIR(1), KEY_F(7)) == true)
            {
                goto Exit;
            }
        
        
    
    
 handleError:
        msg = "handleError occured";
        DataEntry::errMsg(winMsgArea, 2, 78, msg);
        if (DataEntry::confirmAction(winMsgArea, winFullScreen, 2, 2, 78, "Exit", COLOR_PAIR(1), KEY_F(7)) == true)
        {
            goto Exit;
        }
Exit:

        mvwprintw(winFormArea, 7, 1, "Press Enter to exit...");
        debugFile.close();
        xmlFile.close();
        wrefresh(winFormArea);
        wgetch(winFormArea);

        // End curses mode
        endwin();

        //TODO: what memory clean up should be done?
        return 1;
}




