#ifndef __PLAYERPANE_HPP__
#define __PLAYERPANE_HPP__
#include <ncurses.h>
#include "Pane.hpp"
#include "Player.hpp"
constexpr int height_ = 12;
constexpr int width_ = 27;
class PlayerPane : public Pane {
 public:
#ifdef PRETESTING
    explicit PlayerPane(int y, int x);
#else
    explicit PlayerPane(Player* p, int y, int x);
#endif
#ifdef PRETESTING
    void UpdateWindow();
#else
    void UpdateWindow() const;
#endif
    ~PlayerPane();

#ifdef PRETESTING
    bool turn_;
    int playercode_;
    int score_;
#endif  // end #ifdef PRETESTING
 private:
    WINDOW* win_;
    Player* player_;
};
#endif  // headerguard.
