#include <ncurses.h>
#include "GameManager.hpp"
#include "PlayerPane.hpp"
#include "graphics.hpp"
#ifdef PRETESTING
#include "testplayer.hpp"
#endif

static const char* playerformat = "P L A Y E R %d";
// constexpr int height_ = 12;
// constexpr int width_ = 26;
#ifdef PRETESTING
PlayerPane::PlayerPane(__testplayer* p, int y, int x):
    player_(p), Pane(y, x) {
    player_ = p;
#else
PlayerPane::PlayerPane(Player* p, int y, int x):
    player_(p), Pane(y, x) {
#endif
    win_ = subwin(stdscr, height_, width_, starty_, startx_);
    box(win_, 0, 0);
}
#ifdef PRETESTING
void PlayerPane::UpdateWindow() {
#else
void PlayerPane::UpdateWindow() const {
#endif
    wclear(win_); box(win_, 0, 0);
#ifdef PRETESTING
    if (player_->turn_) {
        wattron(win_, COLOR_PAIR(color_pointer));
        mvwprintw(win_, 2, 7, playerformat, player_->playercode_);
        wattroff(win_, COLOR_PAIR(color_pointer));
    }
    else {
        wattron(win_, COLOR_PAIR(player_->playercode_));
        mvwprintw(win_, 2, 7, playerformat, player_->playercode_);
        wattroff(win_, COLOR_PAIR(player_->playercode_));
    }
#else
    if (player_->is_turn()) {
        wattron(win_, COLOR_PAIR(color_pointer));
        mvwprintw(win_, 2, 7, playerformat, player_->code());
        wattroff(win_, COLOR_PAIR(color_pointer));
    }
    else {
        wattron(win_, COLOR_PAIR(player_->code());
        mvwprintw(win_, 2, 7, playerformat, player_->code());
        wattroff(win_, COLOR_PAIR(color_pointer));
    }
#endif
#ifdef PRETESTING
    wdrawDigit(win_, 4, 2, player_->score_);
#else
    wdrawDigit(win_, 4, 2, player_->getScore());
#endif
    wrefresh(win_);
}

PlayerPane::~PlayerPane() {
    delwin(win_);
#ifndef PRETESTING
    delete player_;
#endif
}
