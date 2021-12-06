#include <ncurses.h>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "graphics.hpp"

// static std::ifstream* digitres[10];
/* fstream* 을 전역변수로 지정하고
   drawdigit로 스트림을 하나만 쓰도록 하려고 했던 코드.
bool GraphicsStart() {
    const char* fileformat = "resource/n%d.txt";
    char buf[24];
    bool flag = true;
    for (int i=0;i<10;i++) {
        std::sprintf(buf, fileformat, i);
        digitres[i] = new std::ifstream(buf, std::fstream::in);
        flag = flag & digitres[i]->is_open();
    }
    return flag;
}*/
int** circleRasterize(const int radius) {
    /* ploting 1/4 circle. */
    int** render = new int*[radius+1];
    for (int i=0; i < radius+1; i++) {
        render[i] = new int[radius+1];
        std::fill_n(render[i], radius+1, 0);
    }
    // render[radius][0] = fill;
    // render[0][radius] = fill;

    int x = 0;
    int y = radius;
    int p = 1-radius;

    for (int i=0; i <= y; i++) {
        render[i][0] = 1;
        render[0][i] = 1;
    }
    while (x < y) {  // rend 1/4 circle
        if (p < 0) {
            p += 2*x + 3;
        } else {
            p += 2*x -2*y + 1;
            y--;
        }
        x++;
        for (int i=0; i <= y; i++) {
            render[x][i] = 1;
            render[i][x] = 1;
        }
    }
    return render;
}
void drawCircle(const int y, const int x,
    const int radius, const wchar_t fill) {
    wdrawCircle(stdscr, y, x, radius, fill);    // wrapper function.
}
void wdrawCircle(WINDOW* window, const int y, const int x,
    const int radius, const wchar_t fill) {
    int** render = circleRasterize(radius);
    int yMax, xMax;
    getmaxyx(window, yMax, xMax);
    for (int i=0; i <= radius; i++) {       // dx
        for (int j=0; j <= radius; j++) {    // dy
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
    for (int i=0; i <= radius; i++) {
        delete[] render[i];
    }
    delete[] render;
}
bool drawDigit(int y, int x, const int digit) {
    return wdrawDigit(stdscr, y, x, digit);    // wrapper function.
}
bool wdrawDigit(WINDOW* window, int y, int x, const int digit) {
    /* draw digit 15x7 ~ 21x7 size */
    /* fstream* 이 전역변수일때 수정하고 사용해야 함.
       111 처럼 같은 숫자일때 여러번 읽으면 안됨.
    int d0 = (digit >= 100);
    int d1 = (digit%100)/10;
    int d2 = digit%10;
    std::string buf1;
    std::string buf2;

    while (std::getline(*digitres[d1], buf1)) {
        std::string buffer;
        if (d0) {
            std::getline(*digitres[1], buf2);
            buffer += buf2;
        }
        buffer += buf1;
        std::getline(*digitres[d2], buf2);
        buffer += buf2;
        mvwprintw(window, y++, x, buffer.c_str());
    }
    if (d0)
        digitres[1]->seekg(0, std::ios::beg);
    digitres[d1]->seekg(0, std::ios::beg);
    digitres[d2]->seekg(0, std::ios::beg);
    return true;
    */
    int d0 = (digit >= 100);
    int d1 = (digit%100)/10;
    int d2 = digit%10;

    const char* fileformat = "resource/n%d.txt";
    char d0filename[24];
    char d1filename[24];
    char d2filename[24];

    std::snprintf(d0filename, sizeof(d0filename), fileformat, 1);
    std::snprintf(d1filename, sizeof(d1filename), fileformat, d1);
    std::snprintf(d2filename, sizeof(d2filename), fileformat, d2);

    std::fstream d0file(d0filename, std::fstream::in);
    std::fstream d1file(d1filename, std::fstream::in);
    std::fstream d2file(d2filename, std::fstream::in);

    if (!(d1file.is_open() && d2file.is_open())) {
        return false;   // opening resource failed.
    }
    std::string buf1;
    std::string buf2;
    while (std::getline(d1file, buf1)) {
        std::ostringstream oss;
        std::string buf0;
        if (d0) {
            std::getline(d0file, buf0);
            buf0 += ' ';
        }
        oss << buf0 << buf1 << ' ';
        std::getline(d2file, buf2);
        oss << buf2;
        mvwprintw(window, y++, x, oss.str().c_str());
    }
    return true;
}
