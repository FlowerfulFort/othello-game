#ifndef __PLAYERPANE_HPP__
#define __PLAYERPANE_HPP__
#include <ncurses.h>
#include "Pane.hpp"
#include "Player.hpp"

#ifdef PRETESTING
#include "testplayer.hpp"
#endif

constexpr int height_ = 12;
constexpr int width_ = 27;
class PlayerPane : public Pane {
 public:
#ifdef PRETESTING
    explicit PlayerPane(__testplayer* p, int y, int x);
    void UpdateWindow();
    __testplayer* player_;
#else
    explicit PlayerPane(Player* p, int y, int x);
    void UpdateWindow() const;
#endif  // PRETESTING
    /* 미구현됨. */
    ~PlayerPane();

 private:
    WINDOW* win_;
#ifndef PRETESTING
    Player* player_;
#endif  // if not defined PRETESTING
};
#endif  // headerguard.
