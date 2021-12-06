#ifndef INCLUDE_PLAYERPANE_HPP_
#define INCLUDE_PLAYERPANE_HPP_
#include <ncurses.h>
#include "Pane.hpp"
#include "Player.hpp"

#ifdef PRETESTING
#include "testplayer.hpp"
#endif

/* UI크기를 고정하기 위한 컴파일타임 상수입니다. */
constexpr int height_ = 12;
constexpr int width_ = 27;
class PlayerPane : public Pane {    // Pane을 상속합니다.
 public:
#ifdef PRETESTING
    explicit PlayerPane(__testplayer* p, int y, int x);
    void UpdateWindow();
    __testplayer* player_;
#else
    /* Player 객체 주소값과 UI가 개설될 좌표를 입력받습니다. */
    explicit PlayerPane(Player* p, int y, int x);

    /* UI를 업데이트 하기위한 virtual 함수입니다. */
    void UpdateWindow() const;

    /* 승리자를 나타내기 위한 함수입니다. */
    void setWin();
#endif  // PRETESTING
    /* 미구현됨. */
    ~PlayerPane();

 private:
    WINDOW* win_;       // UI를 나타내는 Ncurses의 WINDOW 입니다.
#ifndef PRETESTING
    Player* player_;    // UI에 나타낼 Player 포인터 입니다.
#endif  // if not defined PRETESTING
    bool isWinner_;     // 이겼는지 나타내는 변수입니다.
};
#endif  // INCLUDE_PLAYERPANE_HPP_
