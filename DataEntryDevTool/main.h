#pragma once
#include "curses.h"
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//for ncurses
const int LEFT(0);
const int CENTER(1);
const int RIGHT(2);
// forward declarations --------------------------------------------------------------------------------------------
void CleanupMenu(MENU* myMainMenu, ITEM** myMenuItems, int NumChoices);
// shared functions

//static char* TrimWhitespaces(char *str);
char* TrimWhitespaces(char* str);
bool isValidDate(char* s);
void debugMsg(char* string1, char* string2, int val);

void errMsg(WINDOW* win, int row, int col, char* string1);
std::string askQuestion(WINDOW* win, int startrow, int startcol, int width, char* prompt, chtype color);
bool  confirmAction(WINDOW* msgwin, WINDOW* fullwin, int startrow, int startcol, int width, std::string prompt, chtype color, int keyToPress);
void getNumericInput(WINDOW* msgwin, WINDOW* fullwin, int startrow, int startcol, int len, int width, char* prompt, char* input, chtype color);
void PrintInMiddle(WINDOW* win, int startrow, int startcol, int width, char* string, chtype color);
void FindMiddle(WINDOW* win, int startrow, int startcol, int& outrow, int& outcol, int width, char* msg);
// end of forward declarations --------------------------------------------------------------------------------------------
#endif // MAIN_HPP_INCLUDED







