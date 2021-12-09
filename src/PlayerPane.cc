#include <ncurses.h>
#include "GameManager.hpp"
#include "PlayerPane.hpp"
#include "graphics.hpp"
#ifdef PRETESTING
#include "testplayer.hpp"
#endif
/* Player를 나타내는 문자열포맷 입니다. */
static const char* playerformat = "P L A Y E R %d";
// constexpr int height_ = 12;
// constexpr int width_ = 26;
#ifdef PRETESTING
PlayerPane::PlayerPane(__testplayer* p, int y, int x):
    player_(p), Pane(y, x) {
    player_ = p;
#else

/* UI를 생성하고, 각 변수를 할당하고 상위 클래스의
   생성자를 호출합니다. */
PlayerPane::PlayerPane(Player* p, int y, int x):
    player_(p), Pane(y, x), isWinner_(false) {
#endif
    win_ = subwin(stdscr, height_, width_, starty_, startx_);
    box(win_, 0, 0);
}
#ifdef PRETESTING
void PlayerPane::UpdateWindow() {
#else

/* UI를 업데이트하는 virutal 함수입니다. */
void PlayerPane::UpdateWindow() const {
#endif
    /* UI를 지우고, 경계선을 다시 만듭니다. */
    werase(win_);
    box(win_, 0, 0);
#ifdef PRETESTING
    if (player_->turn_) {
        wattron(win_, COLOR_PAIR(color_pointer));
        mvwprintw(win_, 2, 7, playerformat, player_->playercode_);
        wattroff(win_, COLOR_PAIR(color_pointer));
    } else {
        wattron(win_, COLOR_PAIR(player_->playercode_));
        mvwprintw(win_, 2, 7, playerformat, player_->playercode_);
        wattroff(win_, COLOR_PAIR(player_->playercode_));
    }
#else
    /* 현재 턴인 플레이어는 노랑색으로 하이라이트 됩니다. 
       COLOR_PAIR(color_pointer)가 노랑색으로 칠해줍니다. */
    if (player_->is_turn()) {
        wattron(win_, COLOR_PAIR(color_pointer));
        mvwprintw(win_, 2, 7, playerformat, player_->getCode()-1);
        wattroff(win_, COLOR_PAIR(color_pointer));
    } else {    // 만약 턴이 아니라면 플레이어 색깔에 맞춰 칠합니다.
        wattron(win_, COLOR_PAIR(player_->getCode()));
        mvwprintw(win_, 2, 7, playerformat, player_->getCode()-1);
        wattroff(win_, COLOR_PAIR(color_pointer));
    }
    if (isWinner_) {    // 만약 승리자가 확정되었다면,
        // 좌표 UI기준 좌표 y=1, x=6에 해당 문자열을 표시합니다.
        mvwprintw(win_, 1, 6, "! W I N N E R !");
    }
#endif
#ifdef PRETESTING
    wdrawDigit(win_, 4, 2, player_->score_);
#else
    /* 해당 플레이어의 점수를 그립니다.
       graphics.cc에 정의되어 있습니다.  */
    wdrawDigit(win_, 4, 2, player_->getScore());
#endif
    wrefresh(win_);     // UI를 그립니다.
}
void PlayerPane::setWin() {     // 이겼다고 표시하는 함수입니다.
    isWinner_ = true;
}
PlayerPane::~PlayerPane() {     // 미구현 destructor 입니다.
    delwin(win_);
#ifndef PRETESTING
    delete player_;
#endif
}
