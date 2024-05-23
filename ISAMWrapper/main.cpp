#include "curses.h"
#include <vector>
#include <string>
#include <iostream>
#include <numeric>

void displayInput(WINDOW* win, const std::string& mask, const std::vector<char>& input) {
    // Clear the window
    werase(win);
    std::string result = std::accumulate(input.begin(), input.end(), std::string(""));
    mvwprintw(win, 0, 0, "%s", result.c_str());
    

    // Move cursor to the end of input area
    wmove(win, 0, mask.size());   
    wrefresh(win);
}

bool validateInput(WINDOW* win, const std::string& mask, std::vector<char>& input) {
    // Validate input against the mask
    for (size_t i = 0; i < mask.size(); ++i) {
        if (isdigit(mask[i])) {
            if (i < input.size() && mask[i] != input[i]) {
                return false;
            }
        }
        else if (mask[i] == '.') {
            if (i < input.size() && input[i] != '.') {
                return false;
            }
        }
    }
    return true;
}
void shiftLeft(WINDOW* win, const std::string& mask, std::vector<char>& input, int ch) {
    // Convert vector of characters to a string
    std::string inputString(input.begin(), input.end());
    
    if (ch != 8)// exclude backspace
        inputString += ch;
    // Find the index of the decimal point (if exists)

    size_t decimalIndex = inputString.find('.');
    int numPlaces = 0;
    if (decimalIndex != std::string::npos) {
        // Count the number of characters after the decimal point
        numPlaces = inputString.size() - decimalIndex - 1;        
    }
    else {
        numPlaces = 0;
    }
    // Move characters to the left by 1, excluding the decimal point
    if (decimalIndex != std::string::npos) {
        inputString.erase(decimalIndex, 1); // Remove the decimal point
        if (numPlaces > 0) {
            // Move characters to the left
            inputString = inputString.substr(1, decimalIndex) + '.' + inputString.substr(decimalIndex + 1, numPlaces); 
        }
        else
            inputString = inputString.substr(1, decimalIndex);

    }

    // Convert string back to a vector of characters
    input.assign(inputString.begin(), inputString.end());

    
}
void shiftRight(WINDOW* win, const std::string& mask, std::vector<char>& input, int ch) {
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
        charactersAfterDecimal = inputString[decimalIndex - 1] + charactersAfterDecimal;
        // Remove the last character after decimal after moving right 
        charactersAfterDecimal.pop_back();
        // Remove the original character before the decimal point

        charactersBeforeDecimal.pop_back();

        // Combine characters before and after the decimal point
        inputString = charactersBeforeDecimal + "." + charactersAfterDecimal;
        int x = inputString.length();
        // left pad with mask characters to proper length
        returnString = mask.substr(0, (mask.length() - inputString.length()))+inputString;
    }
    input.assign(returnString.begin(), returnString.end());
}


//int main() {
//   
//    std::vector<char> input = { '1', '2', '3', '4', '5','.','6','7'};
//    // Convert vector of characters to a string
//    std::string inputString(input.begin(), input.end());
//
//    // Find the index of the decimal point (if exists)
//    size_t decimalIndex = inputString.find('.');
//
//    // Move characters to the right by 1, excluding the decimal point
//    if (decimalIndex != std::string::npos && decimalIndex > 0 && decimalIndex < input.size() - 1) {
//        // Extract characters before and after the decimal point
//        std::string charactersBeforeDecimal = inputString.substr(0, decimalIndex);
//        std::string charactersAfterDecimal = inputString.substr(decimalIndex + 1);
//
//        // Move characters to the right
//        charactersAfterDecimal = inputString[decimalIndex - 1] + charactersAfterDecimal;
//        // Remove the last character after decimal after moving right 
//        charactersAfterDecimal.pop_back();
//        // Remove the original character before the decimal point
//        charactersBeforeDecimal.pop_back();
//
//        // Combine characters before and after the decimal point
//        inputString = charactersBeforeDecimal + "." + charactersAfterDecimal;
//    }
//
//    // Convert string back to a vector of characters
//    input.assign(inputString.begin(), inputString.end());
//    
//
//    // Output the modified vector
//    for (char c : input) {
//        std::cout << c;
//    }
//    std::cout << std::endl;
//
//    return 0;
//}



int main() {
    WINDOW* win; // Declare a pointer to a window

    initscr(); // Initialize curses
    cbreak(); // Line buffering disabled, pass on everything to me
    noecho(); // Don't echo while we do wgetch

    // Create a new window with specified dimensions
    win = newwin(1, COLS, 0, 0);

    // Mask specifying the format of input
    std::string mask = "0000.00"; // Example mask    
    // Vector to store entered characters, initialized with mask contents
    std::vector<char> input(mask.begin(), mask.end());

    // Track if decimal point has been entered
    bool decimalEntered = false;

    // Display the initial input
    displayInput(win, mask, input);
    
    int charsEntered = 0;
   
    int inputIndex = input.size()-1;

    // Loop to get user input
    while (true) {
        int ch = wgetch(win);     
        
            // // Check if input is a digit, decimal point, backspace, or 'q' to quit
            if ((isdigit(ch) || ch == '.') && charsEntered < mask.size()) {
                if (ch == '.') {
                    // Check if decimal point is already entered
                    if (!decimalEntered) {
                        decimalEntered = true;
                        charsEntered++;
                        shiftLeft(win,mask,input, ch);
                    }
                }
                else {
                    
                    charsEntered++;
                    shiftLeft(win, mask, input, ch);
                    inputIndex--;

                }    
            displayInput(win, mask, input);  
            
        }
        else if (ch == 8 && !input.empty()) {
            // Handle backspace to delete the last entered character
            if (input.back() == '.') {
                // Reset decimal flag if deleted
                decimalEntered = false;
            }           
            charsEntered--;
            shiftRight(win, mask, input,ch);
            // Display the updated input
            displayInput(win, mask, input);
        }
        else if (ch == 'q') {
            break; // Quit if 'q' is pressed
        }
    }

    // Clean up curses
    delwin(win);
    endwin();

    // Validate the entered characters against the mask
    if (validateInput(win,mask,input)) {
        std::cout << "Input is valid: ";
        for (char c : input) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Input is invalid!" << std::endl;
    }

    return 0;
}
