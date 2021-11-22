#include <cstdio>
#include <ncurses.h>
#include <algorithm>
#include <fstream>
#include "graphics.hpp"

int** circleRasterize(const int radius) {
    /* ploting 1/4 circle. */
    int** render = new int*[radius+1];
    for (int i=0;i<radius+1;i++) {
        render[i] = new int[radius+1];
        std::fill_n(render[i], radius+1, 0);
    }
    // render[radius][0] = fill;
    // render[0][radius] = fill;

    int x = 0;
    int y = radius;
    int p = 1-radius;

    for (int i=0;i<=y;i++) {
        render[i][0] = 1;
        render[0][i] = 1;
    }
    while (x < y) { // rend 1/4 circle
        if (p < 0) {
            p += 2*x + 3;
        }
        else {
            p += 2*x -2*y + 1;
            y--;
        }
        
        x++;
        for (int i=0;i<=y;i++) {
            render[x][i] = 1;
            render[i][x] = 1;
        }
    }
    return render;
}
void drawCircle(const int y, const int x, const int radius, const wchar_t fill) {
    wdrawCircle(stdscr, y, x, radius, fill);    // wrapper function.
}
void wdrawCircle(WINDOW* window, const int y, const int x, const int radius, const wchar_t fill) {
    int** render = circleRasterize(radius);
    int yMax, xMax;
    getmaxyx(window, yMax, xMax);
    for (int i=0;i<=radius;i++) {       // dx
        for (int j=0;j<=radius;j++) {    // dy
            if (render[i][j]) {
                if ((y+j < yMax) && (x+i < xMax))
                    mvwprintw(window, y+j, x+i, "%lc", fill);
                if ((y-j >= 0) && (x+i < xMax))
                    mvwprintw(window, y-j, x+i, "%lc", fill);
                if ((y+j < yMax) && (x-i >= 0))
                    mvwprintw(window, y+j, x-i, "%lc", fill);
                if ((y-j >= 0) && (x-i >= 0))
                    mvwprintw(window, y-j, x-i, "%lc", fill);
            }
        }
    }
    for (int i=0;i<=radius;i++) {
        delete[] render[i];
    }
    delete[] render;
}
bool drawDigit(int y, int x, const int digit) {
    return wdrawDigit(stdscr, y, x, digit);    // wrapper function.
}
bool wdrawDigit(WINDOW* window, int y, int x, const int digit) {
    /* draw digit 7x7 / 14x7 size */
    int d1 = digit/10;
    int d2 = digit%10;

    const char* fileformat = "resource/n%d.txt";
    char d1filename[24];
    char d2filename[24];
    std::sprintf(d1filename, fileformat, d1);
    std::sprintf(d2filename, fileformat, d2);

    std::fstream d1file(d1filename, std::fstream::in);
    std::fstream d2file(d2filename, std::fstream::in);

    if (!(d1file.is_open() && d2file.is_open())) {
        return false;   // opening resource failed.
    }
    std::string buf1;
    std::string buf2;
    while (std::getline(d1file, buf1)) {
        std::getline(d2file, buf2);
        buf1 += buf2;
        mvwprintw(window, y++, x, buf1.c_str());
    }
    return true;
}
