//  Version: 5.20.24.9.46
// File: DataEntry.cpp
#include "DataEntry.h"
std::string DataEntry::inputAction;
std::string DataEntry::inputResult;

// Constructor implementation
DataEntry::DataEntry(WINDOW* win, std::string field_name, std::string fieldtype, std::string field_mask, int len, int row, int field_column, std::string field_value, int label_column, std::string label_text)
    : win(win), field_name(field_name),fieldtype(fieldtype), field_mask(field_mask),len(len), row(row), field_column(field_column), field_value(field_value), label_column(label_column), label_text(label_text) {}


void DataEntry::displayLabels() {
    //std::string myString = label_text.c_str();
    //// remove quotes in labels if any
    //myString.erase(std::remove(myString.begin(), myString.end(), '\"'), myString.end());
    //setLabelText(myString);
    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, row, label_column, label_text.c_str());
}

void DataEntry::displayData() {
    std::string myString = field_value.c_str();
    // remove quotes in data if any
    myString.erase(std::remove(myString.begin(), myString.end(), '\"'), myString.end());    
    setFieldValue(removeLeadingSpaces(myString));
    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, row, field_column, field_value.c_str());
}

 bool DataEntry::AcceptInput(DataEntry& dataEntry) {
        
    
    bool result = FALSE;
    std::string type = dataEntry.getFieldType();
    int test = DataEntry::stringSwitchNumber(dataEntry.getFieldType());
    switch (test)    
    {
    case 17: //STRING:
        result = stringInput(dataEntry);
        break;

    case 18: //NUMERIC:
        result = numericInput(dataEntry);
        break;
    }
    std::string temp = dataEntry.field_value.c_str();
    dataEntry.setFieldValue(temp);
    

    return result;
}

 bool DataEntry::allowed(std::string type, char character, std::string EDIT$) {
     //TODO: Add code to verify more types

     bool result = true;
     size_t found = false;

     int test = stringSwitchNumber(type);
     switch (test)
     {

     case   18: //NUMERIC:
         found = numtest.find(character);
         
        if (found == std::string::npos)
            result = false;
        else
        {
            if (character == '.' && EDIT$.find(".") != std::string::npos)
                result = false;//reject . if string already contains .
            else
                if (character == '-' && EDIT$.find("-") != std::string::npos)
                    result = false;//reject - if string already contains -
        }

     }
     return result;
 }
 bool DataEntry::SetupFields(WINDOW* win, std::vector<DataEntry>& fields, std::ifstream& xmlFile) {

     std::string field_name = "";
     std::string fieldtype = "";
     std::string field_mask = "";
     int len = 0;
     int row = 0;
     int col = 0;
     std::string defaultValue = "";
     int label_column = 0;
     std::string label_text = "";
     if (xmlFile.is_open()) {
         std::string line;
         std::getline(xmlFile, line);
         int x = 0;
     }
     
     // Read data from the file and initialize DataEntry objects
     std::string line;
     while (std::getline(xmlFile, line)) {// && line.find("</field>") == std::string::npos) {

         if (line.find("</field>") != std::string::npos) {
             //were at the end of the field XML element.so add the field to the vector
             if (!field_name.empty()) // just in case
                 fields.emplace_back(win, field_name, fieldtype, field_mask, len, row, col, defaultValue, label_column, label_text);
         }

         std::string tag, value;
         // Find the positions of the opening and closing angle brackets
         size_t start_pos = line.find("<");
         size_t end_pos = line.find(">");
         if (start_pos != std::string::npos && end_pos != std::string::npos) {
             // Extract the tag name
             tag = line.substr(start_pos + 1, end_pos - start_pos - 1);
             // Trim leading and trailing whitespace characters from the tag
             tag.erase(0, tag.find_first_not_of(" \n\r\t"));
             tag.erase(tag.find_last_not_of(" \n\r\t") + 1);
             // Find the position of the opening angle bracket after the closing tag
             size_t value_start_pos = line.find(">", end_pos);
             if (value_start_pos != std::string::npos) {
                 // Extract the value between the angle brackets
                 value = line.substr(value_start_pos + 1);
                 // Find the position of the closing tag
                 size_t value_end_pos = value.find("</" + tag + ">");
                 if (value_end_pos != std::string::npos) {
                     // Trim leading and trailing whitespace characters from the value
                     value.erase(0, value.find_first_not_of(" \n\r\t"));
                     value.erase(value.find_last_not_of(" \n\r\t") + 1);
                     // Extract the value between the opening and closing tags
                     value = value.substr(0, value_end_pos);
                 }
             }
             if (tag == "name") {
                 field_name = DataEntry::removeLeadingSpaces(value);
                 // remove double quote mark if any
                 field_name.erase(std::remove(field_name.begin(), field_name.end(), '\"'), field_name.end());
             }

             else if (tag == "type") fieldtype = value;
             else if (tag == "mask") field_mask = value;
             else if (tag == "length")  len = std::stoi(value);
             else if (tag == "row")  row = std::stoi(value);
             else if (tag == "col")  col = std::stoi(value);
             else if (tag == "defaultValue") {
                 defaultValue = DataEntry::removeLeadingSpaces(value);
                 // remove double quote mark if any
                 defaultValue.erase(std::remove(defaultValue.begin(), defaultValue.end(), '\"'), defaultValue.end());
             }
             else if (tag == "label_column")  label_column = std::stoi(value);

             else if (tag == "label_text") {

                 label_text = DataEntry::removeLeadingSpaces(value);
                 // remove double quote mark if any
                 label_text.erase(std::remove(label_text.begin(), label_text.end(), '\"'), label_text.end());

             }

         }
     }
     return true;
}

         
 
 void DataEntry::parseCSVWithQuotes(std::string& line, std::vector<std::string>& tokens) {
     std::string token;
     std::istringstream stream(line);
     char c;
     bool inQuotes = false;
     while (stream.get(c)) {
         if (c == '"' && (stream.peek() != '"' || !inQuotes)) { // Handle quote toggle
             inQuotes = !inQuotes;
         }
         else if (c == ',' && !inQuotes) { // End of a token
             tokens.push_back(token);
             token.clear();
         }
         else if (c == '"' && inQuotes && stream.peek() == '"') { // Handle escaped quote
             token += c; // Add one quote of the escaped pair
             stream.get(c); // Skip the next quote
         }
         else {
             token += c;
         }
     }
     if (!token.empty()) tokens.push_back(token); // Add the last token
 } 
 std::string DataEntry::removeLeadingSpaces(const std::string& str) {
     // Find the position of the first non-space character
     size_t start = str.find_first_not_of(" \t");

     // If all characters are spaces, return an empty string
     if (start == std::string::npos) {
         return "";
     }

     // Extract the substring starting from the first non-space character
     return str.substr(start);
 }
 void DataEntry::removeTrailingSpaces(std::string& str) {
     // Find the position of the last non-space character
     size_t pos = str.find_last_not_of(' ');

     // Erase all characters from pos+1 to the end of the string
     if (pos != std::string::npos) {
         str.erase(pos + 1);
     }
     else {
         // If the entire string is spaces, clear it
         str.clear();
     }
 }
 //std::string DataEntry::removeTrailingSpaces(const std::string& str) {
 //    // Find the position of the first non-space character
 //    size_t start = str.find_last_not_of(" \t");

 //    // If all characters are spaces, return an empty string
 //    if (start == std::string::npos) {
 //        return "";
 //    }

 //    
 //    return str.substr(start);
 //}

 bool DataEntry::isValidDate(const char* s) {
     if (strlen(s) != 10) return false; // Quick check for date length

     char cmonth[3] = { 0 }, cday[3] = { 0 }, cyear[5] = { 0 };
     bool result = true;
     int year, month, day;

     if (isdigit(s[0]) && isdigit(s[1]) && s[2] == '/' &&
         isdigit(s[3]) && isdigit(s[4]) && s[5] == '/' &&
         isdigit(s[6]) && isdigit(s[7]) && isdigit(s[8]) && isdigit(s[9])) {
         strncpy_s(cmonth, sizeof(cmonth), s, 2);
         strncpy_s(cday, sizeof(cday), s + 3, 2);
         strncpy_s(cyear, sizeof(cyear), s + 6, 4);

         month = atoi(cmonth);
         day = atoi(cday);
         year = atoi(cyear);

         // Leap year check
         bool leapyear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));

         if (month < 1 || month > 12) {
             result = false;
         }
         else {
             if (day < 1 || day > 31) {
                 result = false;
             }
             else {
                 if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
                     result = false;
                 }
                 else if (month == 2) {
                     if (leapyear && day > 29) {
                         result = false;
                     }
                     else if (!leapyear && day > 28) {
                         result = false;
                     }
                 }
             }
         }
     }
     else {
         result = false;
     }

     return result;
 }
 
 void DataEntry::debugMsg(WINDOW* msgwin, WINDOW* fullwin,std::string string1, std::string string2, int val)
 {
     //---------------------------------------------------
     std::string result;
     // Create a stringstream
     std::stringstream ss;

     // Insert the string and integer into the stringstream
     ss << "string1 = "<<string1 << "val " << val;

     // Extract the concatenated string from the stringstream
     result = ss.str();

     // Print the result
     //std::cout << result << std::endl;
     //---------------------------------------------------
     wmove(msgwin, 22, 2);
     ///clrtoeol();
     const char* myCString = result.c_str();
     wmove(msgwin, 22, 2);
     wclrtoeol(msgwin);
     mvwprintw(msgwin, 22, 2, myCString);
     wrefresh(msgwin);
     wrefresh(fullwin);
     wgetch(msgwin);
 }
 void DataEntry::errMsg(WINDOW* win, int row, int col, std::string& msg)
 {
     //wprintw(win,"Debug message: %s\n", msg.c_str());
     

     // Refresh the screen to make the changes visible
     //wrefresh(win);
     //return;
     //endwin();
     

     int outrow, outcol, width;
     width = 79;
     beep();


     //if (win == NULL)
     //{
     //    //debugMsg("win == NULL", "", 0);
     //    win = stdscr;
     //}

     wmove(win, row, col - 1);
     wclrtoeol(win);
     FindMiddle(win, row, col, outrow, outcol, width, msg);

     //wattron(win, color);
     wattron(win,A_BLINK);
     mvwprintw(win, outrow, outcol, "%s", msg.c_str());
     PrintInMiddle(win,row,col,79,msg, COLOR_PAIR(1));
     //box(win,0,0);
     wattroff(win,A_BLINK);
     wrefresh(win);
     wgetch(win);
     wmove(win, col, row);
     wclrtoeol(win);
     //box(win,0,0);
     wrefresh(win);
     refresh();

 }
 void DataEntry::PrintInMiddle(WINDOW* win, int startrow, int startcol, int width, std::string& msg, chtype color)
 {
     int outrow, outcol;

     FindMiddle(win, startrow, startcol, outrow, outcol, width, msg);
     mvwprintw(win, outrow, outcol, "%s", msg.c_str());
     refresh();

 }


 void DataEntry::FindMiddle(WINDOW* win, int startrow, int startcol, int& outrow, int& outcol, int width, std::string msg)
 {
     int length, col, row;
     float temp;
     //if (win == NULL)
         //debugMsg("FindMiddle", "NULL window passed", 0);

     if (win == NULL)
         win = stdscr;
     getyx(win, row, col);
     if (startcol != 0)
         col = startcol;
     if (startrow != 0)
         row = startrow;
     if (width == 0)
         width = 80;
     
     temp = ((width - int(msg.length())) / 2);
     outcol = startcol + (int)temp;
     outrow = row;


 }
 std::string DataEntry::askQuestion(WINDOW* win, int startrow, int startcol, int width, std::string prompt, chtype color)
 {

 restart:
     echo();


     char xstr[] = { ' ' };
     int input;

     wmove(win, startrow, startcol);
     wclrtoeol(win);
     int outrow, outcol;

     FindMiddle(win, startrow, startcol, outrow, outcol, width, prompt);

     //wattron(win, color);
     mvwprintw(win, outrow, outcol, "%s", prompt.c_str());
     //wattroff(win, color);
     box(win, 0, 0);
     wrefresh(win);

     input = wgetch(win);

     wmove(win, outrow, outcol);
     //wclrtoeol(win);
     werase(win);
     wrefresh(win);
     if (input == 'y' || input == 'Y')
     {
         return "Yes";
     }
     else
     {

         return "No";
     }
 }
 bool  DataEntry::confirmAction(WINDOW* msgwin, WINDOW* fullwin, int startrow, int startcol, int width, std::string prompt, chtype color, int keyToPress)
 {


     std::string keyMessage, message;
     int c, outrow, outcol;
     bool result = false;

     wattron(msgwin, COLOR_PAIR(3));
     wmove(msgwin, startrow, startcol);
     wclrtoeol(msgwin);
     //refresh();
     switch (keyToPress)
     {
     case KEY_F(3):
         keyMessage = "Press F3 to ";
         break;
     case KEY_F(4):
         keyMessage = "Press F4 to ";
         break;
     case KEY_F(5):
         keyMessage = "Press F5 to ";
         break;
     case KEY_F(7):
         keyMessage = "Press F7 to ";
         break;
     case KEY_NPAGE:
         keyMessage = "Press Page Down to ";
         break;

     }
     message = keyMessage;
     message.append(prompt);

     FindMiddle(msgwin, startrow, startcol, outrow, outcol, width, message);


     mvwprintw(msgwin, outrow, outcol, "%s", message.c_str());

     box(msgwin, 0, 0);
     keypad(msgwin, TRUE);
     wbkgd(msgwin, COLOR_PAIR(3));
     wrefresh(msgwin);
     //wrefresh(winFormArea);
     wrefresh(fullwin);
     c = wgetch(msgwin);
     wmove(msgwin, startrow, startcol);
     wclrtoeol(msgwin);
     //refresh();

     if (c == keyToPress)
     {


         result = true;
     }
     else
     {
         if (msgwin != stdscr)
         {

             result = false;
         }

     }

     return result;
 }
 
 std::string DataEntry::doFunctionKeys(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string tbl, bool AddingNew,  ISAMWrapperLib& lib,  std::string& condition, std::vector<DataEntry>& fields) {
     std::string result = "";
     int index = 0;
    /* int mainHeight = 24;
     int mainWidth = 80;*/
     int lookupHeight = 10;
     int lookupWidth = 50;
     int lookupStartY = 5;
     int lookupStartX = 5;
     std::vector<DataTuple> tupleVectordata;
     std::vector<std::pair<std::string, std::string>> pairVectordata;
     std::vector<std::string> fieldNames;
     std::vector<std::string> fieldValues;
     std::string selectedElement = "";

     for (const auto& entry : fields) {        
         std::string fieldName = entry.getfieldName();
         std::replace(fieldName.begin(), fieldName.end(), ' ', '_');  // Replace spaces with underscores
         fieldNames.push_back(fieldName);
         std::string fieldValue = entry.getFieldValue();
         std::replace(fieldValue.begin(), fieldValue.end(), '"', ' ');  // Replace quote with space
         fieldValues.push_back(fieldValue);
     }
     int test = DataEntry::stringSwitchNumber(inputAction);
     switch (test) {
     case 1: break;//KEY_F(1)                
     
     case 2: //KEY_F2
        
         tupleVectordata = lib.selectRows(tbl, "");
         // Transform vector of tuples into vector of pairs
         pairVectordata = DataEntry::transformVector(tupleVectordata);
         selectedElement = DataEntry::displayLookupWindow(winFullScreen, lookupHeight, lookupWidth, lookupStartY, lookupStartX, pairVectordata, condition);
         result = "TryReadRecord";
         condition = fieldNames[0] + " = \'" + selectedElement + "\'";
         return result;
         break;
     case 3: //KEY_F(3)
         if (DataEntry::confirmAction(winMsgArea, winFullScreen, 2, 2, 78, "Delete Record", COLOR_PAIR(1), KEY_F(3)) == true) {
             lib.deleteRow(tbl, condition);
             result = "RecordEntryStart";
         }
         break;
     case 4: //KEY_F(4)
         if (DataEntry::confirmAction(winMsgArea, winFullScreen, 2, 2, 78, "Save Record", COLOR_PAIR(1), KEY_F(4)) == true) {
             if (AddingNew == true) {
                 lib.insert(tbl, fieldNames, fieldValues);
             }
             else {
                 lib.updateRow(tbl, fieldNames, fieldValues, condition);
             }
             // Clear field values after update or insert
             for (auto& field : fields) {
                 field.setFieldValue("");
             }
             result = "RecordEntryStart";
         }
         break;
     case 5: //KEY_F(5)
         if (DataEntry::confirmAction(winMsgArea, winFullScreen, 2, 2, 78, "Restart", COLOR_PAIR(1), KEY_F(5)) == true) {
            
             result = "RecordEntryStart";
         }
         break;
     case 6: break;//KEY_F(6)
     case 7: //KEY_F(7)     
         if (DataEntry::confirmAction(winMsgArea, winFullScreen, 2, 2, 78, "Exit", COLOR_PAIR(1), KEY_F(7)) == true) {
             result = "Exit";
         }
         break;
     default:
         //TODO  add other cases?
         result = "";
     }
 return result;
 }
 

 std::string DataEntry::readRecord(WINDOW* winFullScreen, WINDOW* winMsgArea, std::string tbl, bool AddingNew, bool prompt, ISAMWrapperLib& lib, const std::string condition, std::vector<DataEntry>& fields) {
     std::string result = "";
     std::vector<std::string> fieldNames;
     std::vector<std::string> fieldValues;
     int index = 0;

     //for (const auto& entry : fields) {
     //    std::string fieldName = entry.getfieldName();
     //    std::replace(fieldName.begin(), fieldName.end(), ' ', '_');  // Replace spaces with underscores
     //    fieldNames.push_back(fieldName);
     //    std::string fieldValue = entry.getFieldValue();
     //    std::replace(fieldValue.begin(), fieldValue.end(), '"', ' ');  // Replace quote with space
     //    fieldValues.push_back(fieldValue);
     //}

     bool exists = lib.getRecordByCondition(tbl, condition, fieldValues);
     if (exists == true) {
         index = 0;
         for (auto& entry : fields) {
             entry.setFieldValue(fieldValues[index]);
             index++;
         }
         result = "EditingRecord";
     }
     else {
         if (prompt == true) {
             std::string message = "Record not found. Do you want to add it?";
             std::string answer = askQuestion(winMsgArea, 1, 2, 0, message, COLOR_PAIR(1));
             if (answer == "Yes")
                 result = "AddNewRecord";
             else
                 result = "RecordNotFound";
         }
     }
     return result;
 } 
 int DataEntry::stringSwitchNumber(const std::string& key) {
    
    // because the switch statement doesn't support alpha values
    // this function is helpful for mapping an alpha value to a number
    // originally i used this for funtion key values but 
    // found this tecnique will be useful for any alpha test
    static const std::unordered_map<std::string, int> keyMap = {

        {"KEY_UP", -1},
        {"KEY_ENTER", 0},             
        {"KEY_F(1)", 1},
        {"KEY_F(2)", 2},
        {"KEY_F(3)", 3},
        {"KEY_F(4)", 4},
        {"KEY_F(5)", 5},
        {"KEY_F(7)", 7},
        {"BeginField", 13},
        {"AddNewRecord", 14},
        {"RecordEntryStart", 15},
        {"exitField_NOT_KEY_F",16},
        {"STRING", 17},
        {"NUMERIC", 18},
        {"MASKED", 19},
        {"UPPERCASE",20},
        {"DIGITS", 21},
        {"QUESTION",22},
        {"DATE",23}   
    };

    // Check if the key exists in the map
    auto it = keyMap.find(key);
    if (it != keyMap.end()) {
        // Return the corresponding numeric value
        return it->second;
    }
    else {
        // Return a default value or handle the case when the key is not found
        return -99; // Default value indicating key not found
    }
 }
 bool DataEntry::validateXml(WINDOW* winFullScreen, WINDOW* winMsgArea,std::ifstream& xmlFile) {
     
     if (!xmlFile.is_open()) {
         return false;
     }

     std::string line;
     bool dataDictionaryFound = false;
     bool fieldOpenTagFound = false;
     std::vector<std::string> fieldTags = { "name", "type", "mask", "length", "row", "col", "defaultValue", "label_column", "label_text" };
     //int x = 0;
     while (std::getline(xmlFile, line)) {
         std::string msg = "line = "+line;
         //x++;
         //debugMsg(winFullScreen, winMsgArea,msg, "",x);
         // Check if line contains the opening tag for dataDictionary
         if (line.find("<dataDictionary>") != std::string::npos) {
             dataDictionaryFound = true;
         }

         // Check if line contains the opening tag for field
         if (line.find("<field>") != std::string::npos) {
             fieldOpenTagFound = true;
         }

         // Check if line contains the closing tag for field
         if (line.find("</field>") != std::string::npos) {
             fieldOpenTagFound = false;
         }

         // Check if line contains any field tags
         for (const auto& tag : fieldTags) {
             if (line.find("<" + tag + ">") != std::string::npos && !fieldOpenTagFound) {
                 //std::cerr << "Error: Found tag <" << tag << "> outside <field> element" << std::endl;
                 return false;
             }
         }

         // Check if line contains the closing tag for dataDictionary
         if (line.find("</dataDictionary>") != std::string::npos) {
             if (!dataDictionaryFound) {
                 //std::cerr << "Error: Found </dataDictionary> without opening <dataDictionary> tag" << std::endl;
                 return false;
             }
             else {
                 return true; // XML validation successful
             }
         }
     }

     //std::cerr << "Error: Missing </dataDictionary> tag" << std::endl;
     return false;
 }

 bool DataEntry::exportTableToCSV(sqlite3* db, const std::string& tableName, const std::string& fileName) {
     std::ofstream outputFile(fileName);
     if (!outputFile.is_open()) {
         //std::cerr << "Error opening output file: " << fileName << std::endl;
         return false;
     }

     // Prepare the SQL query to fetch data from the table
     //std::stringstream query;
     std::string query ="SELECT * FROM "+ tableName+";";

     // Execute the query
     sqlite3_stmt* stmt;
     int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
     if (rc != SQLITE_OK) {
         //std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
         return false;
     }

     // Write column names as the header row in the CSV file
     int numColumns = sqlite3_column_count(stmt);
     for (int i = 0; i < numColumns; ++i) {
         if (i > 0) outputFile << ",";
         outputFile << sqlite3_column_name(stmt, i);
     }
     outputFile << std::endl;

     // Fetch and write data rows to the CSV file
     while (sqlite3_step(stmt) == SQLITE_ROW) {
         for (int i = 0; i < numColumns; ++i) {
             if (i > 0) outputFile << ",";
             const unsigned char* columnText = sqlite3_column_text(stmt, i);
             if (columnText) {
                 outputFile << columnText;
             }
         }
         outputFile << std::endl;
     }

     // Finalize the statement and close the output file
     sqlite3_finalize(stmt);
     outputFile.close();

     return true;
 }
 // Function to transform vector of tuples into vector of pairs
 std::vector<std::pair<std::string, std::string>> DataEntry::transformVector(const std::vector<DataTuple>& inputVector) {
     std::vector<std::pair<std::string, std::string>> outputVector;

     for (const auto& tuple : inputVector) {
         std::string key = std::get<0>(tuple);
         std::string value = std::get<1>(tuple);
         outputVector.push_back(std::make_pair(key, value));
     }

     return outputVector;
 }
 std::string DataEntry::displayLookupWindow(WINDOW* mainWindow, int lookupHeight, int lookupWidth, int lookupStartY, int lookupStartX, std::vector<std::pair<std::string, std::string>>& data, std::string condition) {

     int maxLookupWidth = 0;
     // get the value of the widest values about to be displayed
     for (int i = 0; i < static_cast<int>(data.size()); ++i) {
         std::string first = data[i].first;
         std::string second = data[i].second;
         removeTrailingSpaces(first);
         removeTrailingSpaces(second);
         int x = first.length() + second.length();
         maxLookupWidth = std::max(maxLookupWidth, x);
     }
     maxLookupWidth += 10;
     WINDOW* lookupWin = newwin(lookupHeight, maxLookupWidth, lookupStartY, lookupStartX);
     keypad(lookupWin, TRUE);
     box(lookupWin, 0, 0);
     wrefresh(lookupWin); 
     wrefresh(mainWindow);
     int currentRow = 1;
     int selectedRow = 0;
     int topRow = 0;     
     std::string firstValue = "";
     std::string blankLine(maxLookupWidth - 5, ' ');

     while (true) {
         // Clear previous content
         werase(lookupWin);
         box(lookupWin, 0, 0);

         // Display data
         for (int i = topRow; i < std::min(topRow + lookupHeight - 2, static_cast<int>(data.size())); ++i) {
             if (i >= data.size()) {
                 break; // Prevent out-of-bounds access
             }
             mvwprintw(lookupWin, i - topRow + 1, 2, "%s", blankLine.c_str());
             if (i == selectedRow) {
                 wattron(lookupWin, A_REVERSE);
             }
             std::string first = data[i].first;
             std::string second = data[i].second;
             removeTrailingSpaces(first);
             removeTrailingSpaces(second);
           
             mvwprintw(lookupWin, i - topRow + 1, 2, "%s %s", first.c_str(), second.c_str());
             wattroff(lookupWin, A_REVERSE);
         }

         wmove(lookupWin, currentRow - topRow + 1, 2); // Move cursor to currentRow
         wrefresh(lookupWin);

         int ch = wgetch(lookupWin);
         switch (ch) {
         case KEY_UP:
             if (selectedRow > 0) {
                 selectedRow--;
                 if (currentRow > 1) {
                     currentRow--;
                 }
                 else if (topRow > 0) {
                     topRow--;
                 }
             }
             break;
         case KEY_DOWN:
             if (selectedRow < static_cast<int>(data.size()) - 1) {
                 selectedRow++;
                 if (currentRow < lookupHeight - 2 && currentRow < static_cast<int>(data.size()) - topRow) {
                     currentRow++;
                 }
                 else if (topRow < static_cast<int>(data.size()) - lookupHeight + 2) {
                     topRow++;
                 }
             }
             break;
         case '\n': // Enter key pressed
             delwin(lookupWin);
             touchwin(mainWindow);
             wrefresh(mainWindow);
             firstValue = data[selectedRow].first;
             return firstValue;
         case 27: // Escape key pressed
             delwin(lookupWin);
             touchwin(mainWindow);
             wrefresh(mainWindow);
             return "";
         }
     }
     return 0;
 }
 


 