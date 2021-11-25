#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__
#include <ncurses.h>

void drawCircle(const int y, const int x, const int radius, const wchar_t ifill);
void wdrawCircle(WINDOW* window, const int y, const int x, const int radius, const wchar_t fill);
bool drawDigit(const int y, const int x, const int digit);
bool wdrawDigit(WINDOW* window, const int y, const int x, const int digit);
#endif  // headerguard.
