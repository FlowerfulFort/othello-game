#include <ncurses.h>
#include <algorithm>
#include "Board.hpp"

Board::Board(int boardsize, int y, int x): 
    boardsize_(boardsize), starty_(y), startx_(x) {
    //mvprintw(1,1,"boardsize: %d", boardsize_);
    gameboard_ = new int*[boardsize_];
    wboard_ = new WINDOW**[boardsize_];
    for(int i=0;i<boardsize_;i++) {
        gameboard_[i] = new int[boardsize_];
        std::fill_n(gameboard_[i], boardsize_, -1);
        wboard_[i] = new WINDOW*[boardsize_];

        for(int j=0;j<boardsize_;j++) {
            wboard_[i][j] = subwin(stdscr, 4, 7, i*3+starty_, j*6+startx_);
            wborder(wboard_[i][j], '|', '|', '-', '-', '+', '+', '+', '+');
            //box(plate[i][j], 0, 0);
        }
    }
    touchwin(stdscr);
}
Board::~Board() {
    for (int i=0;i<boardsize_;i++) {
        delete[] gameboard_[i];
        for (int j=0;j<boardsize_;j++) {
            delwin(wboard_[i][j]);
        }
        delete[] wboard_[i];
    }
    delete[] wboard_;
    delete[] gameboard_;
}
