//#include "curses.h"
//#include <vector>
//#include <string>
//
//void displayNames(WINDOW* window, const std::vector<std::string>& names, int startIdx, int visibleRows) {
//    wclear(window);
//    box(window, 0, 0);
//
//    // Display a subset of names based on the start index and visible rows
//    for (int i = 0; i < visibleRows && (startIdx + i) < names.size(); ++i) {
//        mvwprintw(window, i, 0, "%s", names[startIdx + i].c_str());
//    }
//
//    wrefresh(window);
//}
//
//int main() {
//    initscr();
//    cbreak();
//    keypad(stdscr, TRUE);
//    noecho();
//
//    int windowHeight = 10;
//    int windowWidth = 40;
//    WINDOW* mainWin = newwin(windowHeight + 2, windowWidth, 0, 0);
//    box(mainWin, 0, 0);
//    wrefresh(mainWin);
//
//    std::vector<std::string> names;
//    for (int i = 1; i <= 100; ++i) {
//        names.push_back("Name " + std::to_string(i));
//    }
//
//    int startIdx = 0;
//    int visibleRows = windowHeight;
//
//    displayNames(mainWin, names, startIdx, visibleRows);
//    wrefresh(mainWin);
//    /// <summary>
//    /// 
//    /// </summary>
//    /// <returns></returns>
//    int ch;
//    while ((ch = wgetch(mainWin)) != KEY_F(1)) {
//        switch (ch) {
//        case KEY_UP:
//            if (startIdx > 0) {
//                --startIdx;
//                displayNames(mainWin, names, startIdx, visibleRows);
//            }
//            break;
//        case KEY_DOWN:
//            if (startIdx + visibleRows < names.size()) {
//                ++startIdx;
//                displayNames(mainWin, names, startIdx, visibleRows);
//            }
//            break;
//        case KEY_PPAGE:  // Page Up
//            if (startIdx >= visibleRows) {
//                startIdx -= visibleRows;
//            }
//            else {
//                startIdx = 0;
//            }
//            displayNames(mainWin, names, startIdx, visibleRows);
//            break;
//        case KEY_NPAGE:  // Page Down
//            if (startIdx + 2 * visibleRows < names.size()) {
//                startIdx += visibleRows;
//            }
//            else {
//                startIdx = names.size() - visibleRows;
//            }
//            displayNames(mainWin, names, startIdx, visibleRows);
//            break;
//        }
//    }
//
//    delwin(mainWin);
//    endwin();
//    refresh();
//
//    return 0;
//}
