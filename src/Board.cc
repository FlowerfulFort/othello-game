#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include "GameManager.hpp"
#include "Board.hpp"
using std::make_pair;
static const char* piece = "   ";
static Pos differences[8] = {
    { -1, -1 }, { -1, 0 }, { 0, -1 }, { 1, 0 },
    { 0, 1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
};
constexpr int codeoffset = 2;
Board::Board(int boardsize, int y, int x): 
    boardsize_(boardsize), Pane(y, x) {
    //mvprintw(1,1,"boardsize: %d", boardsize_);
    gameboard_ = new int*[boardsize_];
    wboard_ = new WINDOW**[boardsize_];
    for (int i=0;i<boardsize_;i++) {
        gameboard_[i] = new int[boardsize_];
        std::fill_n(gameboard_[i], boardsize_, 1);
        wboard_[i] = new WINDOW*[boardsize_];

        for (int j=0;j<boardsize_;j++) {
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
    gameboard_[boardsize_/2-1][boardsize_/2-1] = 3;
    gameboard_[boardsize_/2][boardsize_/2] = 3;
    gameboard_[boardsize_/2][boardsize_/2-1] = 2;
    gameboard_[boardsize_/2-1][boardsize_/2] = 2;

    piece_aval_ = new bool**[2];    // 2 players.
    piece_aval_[0] = new bool*[boardsize_];
    piece_aval_[1] = new bool*[boardsize_];
    for (int i=0;i<boardsize_;i++) {
        piece_aval_[0][i] = new bool[boardsize_];
        std::fill_n(piece_aval_[0][i], boardsize_, false);
    }
    for (int i=0;i<boardsize_;i++) {
        piece_aval_[1][i] = new bool[boardsize_];
        std::fill_n(piece_aval_[1][i], boardsize_, false);
    }
    aval_[0] = true;
    aval_[1] = true;
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
Pos Board::getPointing() const {
    return std::make_pair(pointy_, pointx_);
}
/* 정우건님 개선코드 */
/* 최적화를 할수있는 여지가 있다면,
   뒤집을 수 있는 경우의 수를
   비트로 나타내어 8방향=1byte로 표현한다. 
   그 후 각각의 비트를 마스킹하여 뒤집으면 된다. */
Pos Board::canReverse(int code, const Pos& start, const Pos& diff) {
    auto [ dy, dx ] = diff;
    auto [ y, x ] = start;
    auto [ ny, nx ] = start;
    bool flag = false;
    ny += dy; nx += dx;
    while (ny >= 0 && nx >= 0 && ny < boardsize_ && nx < boardsize_) {
        if (gameboard_[ny][nx] == code) {
            if (flag) return make_pair(ny, nx);
            else return make_pair(-1, -1);
        } else if (gameboard_[ny][nx] == 1) {
            return make_pair(-1, -1);
        } else {
            flag = true;
            // break;
        }
        ny += dy; nx += dx;
    }
    return make_pair(-1, -1);   // cant put piece.
}
/* 놓음과 동시에 단방향 뒤집기 기능을 함. */
void Board::Reverse(int code, const Pos& start, const Pos& dest, const Pos& diff) {
    auto [ ny, nx ] = start;
    auto [ dy, dx ] = diff;
    while (ny != dest.first || nx != dest.second) {
        gameboard_[ny][nx] = code;
        ny += dy; nx += dx;
    }
}
void Board::UpdateRevMap(int code) {
    aval_[code-codeoffset] = false;
    for (int i=0;i<boardsize_;i++) {
        for (int j=0;j<boardsize_;j++) {
            bool flag = false;
            if (gameboard_[i][j] != 1)
                goto FLAGINPUT;
            for (int k=0;k<8;k++) {
                auto [ y, x ] = canReverse(code, make_pair(i, j), differences[k]);
                if (y != -1) {
                    flag = true;
                    break;
                }
            }
            FLAGINPUT:
            piece_aval_[code-codeoffset][i][j] = flag;
            aval_[code-codeoffset] = aval_[code-codeoffset] | flag;
        }
    }
}

bool Board::isTurnAval(int code) const {
    return aval_[code-codeoffset];
}
bool Board::pieceAval(int code, const Pos& p) const {
    return piece_aval_[code-codeoffset][p.first][p.second];
}

void Board::ReverseCaller(int code, const Pos& point) {
    for (int i=0;i<8;i++) {
        Pos dest = canReverse(code, point, differences[i]);
        if (dest.first == -1)
            continue;
        Reverse(code, point, dest, differences[i]);
    }
}
std::pair<int, int> Board::calcScore() const {
    int nums[4];
    /* 0으로 초기화 */
    std::fill_n(nums, 4, 0);
    for (int i=0;i<boardsize_;i++) {
        for (int j=0;j<boardsize_;j++) {
            nums[gameboard_[i][j]]++;
        }
    }
    return make_pair(nums[2], nums[3]);
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
