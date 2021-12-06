/* 특수한 그래픽 그리기를 담당하는 graphics.cc 입니다.
   여기에 정의되어 있는 circleRasterize 및 wdrawCircle은
   계획이 변경되어 사용하지 않게 되었습니다.
   이 파일에 정의되어 있는 함수 중
   wdrawDigit와 drawDigit만 사용합니다. */
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
    /* 각각의 자릿수를 추출합니다. */
    int d0 = (digit >= 100);
    int d1 = (digit%100)/10;
    int d2 = digit%10;

    /* 파일경로 포맷입니다. */
    const char* fileformat = "resource/n%d.txt";

    /* 각각의 자릿수의 파일네임을 저장할 변수입니다. */
    char d0filename[24];
    char d1filename[24];
    char d2filename[24];

    /* snprintf()로 파일경로를 만듭니다. */
    std::snprintf(d0filename, sizeof(d0filename), fileformat, 1);
    std::snprintf(d1filename, sizeof(d1filename), fileformat, d1);
    std::snprintf(d2filename, sizeof(d2filename), fileformat, d2);

    /* 생성된 파일경로 문자열로 파일스트림을 엽니다. */
    std::fstream d0file(d0filename, std::fstream::in);
    std::fstream d1file(d1filename, std::fstream::in);
    std::fstream d2file(d2filename, std::fstream::in);

    /* 만약 오류가 생겨 열리지 않으면 종료합니다. */
    if (!(d1file.is_open() && d2file.is_open())) {
        return false;   // opening resource failed.
    }

    /* 스트림에서 문자열을 담을 string 입니다. */
    std::string buf1;
    std::string buf2;
    /* d1file에서 줄단위로 입력을 받습니다. */
    while (std::getline(d1file, buf1)) {
        /* stringbuild를 위한 ostringstream 입니다. */
        std::ostringstream oss;
        std::string buf0;
        if (d0) {   // 만약 세번째 자릿수가 존재한다면,
            std::getline(d0file, buf0);  // 그 자리도 입력받아서
            buf0 += ' ';        // 공백을 추가합니다.
        }
        oss << buf0 << buf1 << ' ';      // buf0, buf1을 넣고
        std::getline(d2file, buf2);      // 세번째 자릿수도 입력받습니다.
        oss << buf2;                     // buf2도 넣습니다.
        /* 완성된 문자열을 해당 좌표에 출력합니다. */
        mvwprintw(window, y++, x, oss.str().c_str());
    }
    return true;
}
