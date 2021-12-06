#ifndef INCLUDE_GRAPHICS_HPP_
#define INCLUDE_GRAPHICS_HPP_
#include <ncurses.h>

// bool GraphicsStart();
void drawCircle(const int y, const int x,
    const int radius, const wchar_t ifill);
void wdrawCircle(WINDOW* window, const int y,
    const int x, const int radius, const wchar_t fill);
bool drawDigit(const int y, const int x, const int digit);
bool wdrawDigit(WINDOW* window, const int y, const int x, const int digit);
#endif  // INCLUDE_GRAPHICS_HPP_
