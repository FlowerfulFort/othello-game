#ifndef __BOARD_HPP__
#define __BOARD_HPP__
#include <ncurses.h>
#include "GameManager.hpp"
class GameManager;
class Board {
 public:
    explicit Board(int boardsize);
    ~Board();
 private:
    WINDOW*** wboard_;
    int** gameboard_;
    int boardsize_;
    int startx_;
    int starty_;
    friend GameManager;
};
#endif  // headerguard
