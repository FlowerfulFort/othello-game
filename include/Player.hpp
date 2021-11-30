/* 있어야 할것.
   playercode_는 GameManagerm의 보드포인터에서 플레이어의 말을
   구분하는데 사용한다.

   플레이어가 판에 말을 주는 행위가 있어야 한다.
   매개변수로 int** board, 좌표 y, x를 받고
   해당 board에 playercode를 표시하는 방식으로 말을 놓는다.
   물론 그 곳에 말을 놓을수 있는지 판단해야 하므로
   리턴값으로 false/true로 놓을수 있는지 없는지 리턴한다.
   놓는데 성공했다면 true를 리턴하기로 한다.

   말을 놓았으면 말을 뒤집어야 한다.
   뒤집는 코드도 역시 Player의 함수로 표현한다.
   말을 놓음과 동시에 해도 상관없다.
*/

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
#endif  // headerguard
