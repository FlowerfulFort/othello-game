#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__
#include <ncurses.h>
#include <string>
class Player{
 public:
    Player(WINDOW* ui, std::string name, int playercode, int color);
    void refreshUI() const;
    void turnSet(bool turn);
    void setScore(int score);
 private:
    WINDOW* ui_;
    bool ismyturn_;
    int score_;
    /* GameManager->board에 표시할 숫자. */
    int playercode_;
    std::string name_;
};
