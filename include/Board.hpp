#ifndef __BOARD_HPP__
#define __BOARD_HPP__
#include <ncurses.h>
#include <utility>
#include "GameManager.hpp"
#include "Pane.hpp"
class GameManager;
class Board : public Pane{
 public:
    explicit Board(int boardsize, int y, int x);
    int** returnBoard() const;
    void nowPointing(const int& y, const int& x);
    std::pair<int, int> getPointing() const;
    void pointUp();
    void pointDown();
    void pointLeft();
    void pointRight();
#ifdef PRETESTING
    void UpdateWindow();
#else
    void UpdateWindow() const;
#endif
    ~Board();
 private:
    /* calculate drawing pos. */
    WINDOW*** wboard_;
    int** gameboard_;
    //bool*** piece_aval_;
    int boardsize_;
    //int startx_;
    //int starty_;
    int pointx_;
    int pointy_;
    friend GameManager;
};
#endif  // headerguard
