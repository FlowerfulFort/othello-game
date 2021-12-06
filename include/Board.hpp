#ifndef __BOARD_HPP__
#define __BOARD_HPP__
#include <ncurses.h>
#include <utility>
#include "GameManager.hpp"
#include "Pane.hpp"

typedef std::pair<int, int> Pos;    // <y, x>
class GameManager;
class Board : public Pane{
 public:
    explicit Board(int boardsize, int y, int x);
    int** returnBoard() const;
    void nowPointing(const int& y, const int& x);
    Pos getPointing() const;

    void ReverseCaller(int code, const Pos& point);
    /* start 지점에 놓을수 있는지 체크하는 함수. */
    Pos canReverse(int code, const Pos& start, const Pos& diff);
    /* start부터 dest까지 code로 뒤집는 함수. */
    void Reverse(int code, const Pos& start, const Pos& dest, const Pos& diff);
    /* 리버스맵을 업데이트하는 함수 */
    void UpdateRevMap(int code);

    bool pieceAval(int code, const Pos& p) const;
    /* 자신이 턴을 가질수 있는지 반환하는 함수 */
    bool isTurnAval(int code) const;

    std::pair<int, int> calcScore() const;
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
    bool*** piece_aval_;
    bool aval_[2];
    int boardsize_;
    //int startx_;
    //int starty_;
    int pointx_;
    int pointy_;
    friend GameManager;
};
#endif  // headerguard
