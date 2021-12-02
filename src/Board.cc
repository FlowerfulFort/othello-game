#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include "GameManager.hpp"
#include "Board.hpp"
static const char* piece = "   ";
Board::Board(int boardsize, int y, int x): 
    boardsize_(boardsize), Pane(y, x) {
    //mvprintw(1,1,"boardsize: %d", boardsize_);
    gameboard_ = new int*[boardsize_];
    wboard_ = new WINDOW**[boardsize_];
    for(int i=0;i<boardsize_;i++) {
        gameboard_[i] = new int[boardsize_];
        std::fill_n(gameboard_[i], boardsize_, 1);
        wboard_[i] = new WINDOW*[boardsize_];

        for(int j=0;j<boardsize_;j++) {
            wboard_[i][j] = subwin(stdscr, 4, 7, i*3+starty_, j*6+startx_);
            wattron(wboard_[i][j], COLOR_PAIR(color_board));
            wborder(wboard_[i][j], '|', '|', '-', '-', '+', '+', '+', '+');
            mvwprintw(wboard_[i][j], 1, 1, "     ");
            mvwprintw(wboard_[i][j], 2, 1, "     ");
            wattroff(wboard_[i][j], COLOR_PAIR(color_board));
            //box(plate[i][j], 0, 0);
        }
    }
    /* 2==black , 3==white */
    gameboard_[boardsize_/2-1][boardsize_/2-1] = 2;
    gameboard_[boardsize_/2][boardsize_/2] = 2;
    gameboard_[boardsize_/2][boardsize_/2-1] = 3;
    gameboard_[boardsize_/2-1][boardsize_/2] = 3;
    /* !!!!!TESTING START!!!!! */
    pointx_ = 2;
    pointy_ = 3;
    /* !!!!!TESTING END!!!!! */
    touchwin(stdscr);
}
int** Board::returnBoard() const { return gameboard_; }
void Board::nowPointing(const int& y, const int& x) {
    pointx_ = x;
    pointy_ = y;
}
#ifdef PRETESTING
void Board::UpdateWindow() {
#else
void Board::UpdateWindow() const {
#endif
    for (int i=0;i<boardsize_;i++) {
        for (int j=0;j<boardsize_;j++) {
            WINDOW* pos = wboard_[i][j];
            const int& n = gameboard_[i][j];
            wattron(pos, COLOR_PAIR(n));
            mvwprintw(pos, 1, 2, piece);
            mvwprintw(pos, 2, 2, piece);
            wattroff(pos, COLOR_PAIR(n));
        }
    }
    if(pointy_ < 0 || pointx_ < 0 || pointy_ >= boardsize_ || pointx_ >= boardsize_) {
        endwin();
        std::cerr << "pointx/y_ error occured" << std::endl;
        exit(0);
    }
        
    WINDOW* pos = wboard_[pointy_][pointx_];
    wattron(pos, COLOR_PAIR(color_pointer));
    mvwprintw(pos, 1, 2, piece);
    mvwprintw(pos, 2, 2, piece);
    wattroff(pos, COLOR_PAIR(color_pointer));
    touchwin(stdscr);
    refresh();
}
void Board::pointUp() {
    if (pointy_ <= 0) return;
    pointy_--;
}
void Board::pointDown() {
    if (pointy_ >= boardsize_-1) return;
    pointy_++;
}
void Board::pointLeft() {
    if (pointx_ <= 0) return;
    pointx_--;
}
void Board::pointRight() {
    if (pointx_ >= boardsize_-1) return;
    pointx_++;
}
std::pair<int, int> Board::getPointing() const {
    return std::make_pair(pointy_, pointx_);
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
