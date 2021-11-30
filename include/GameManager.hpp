/* 추가해야 할 것.
   보드판에 말을 놓는것은 Player들의 몫이니,
   GameManager는 보드판을 검사하여
   게임이 끝났는지 검사해야 합니다.
   게임이 끝나는 조건은 크게 두가지인데,
   1. 보드판이 꽉차거나,
   2. 현재 턴의 플레이어가 말을 놓을 수 없는 상황일때
   를 매 턴마다 검사해야 합니다.

   1의 경우에는 턴수를 검사하여 간단히 조사할 수 있지만,
   2의 경우가 어렵겠네요.
*/
#ifndef __GAMEMANAGER_HPP__
#define __GAMEMANAGER_HPP__
#include "Player.hpp"
#include "Board.hpp"
class Board;
class GameManager {
 public:
    /* this class is Singleton Class. */
    static GameManager* GetManager();

    /* before "Program" start, this function must be called. */
    void GameInitialize() const;

    /* before "Game" start, this function must be called. */
    /* It sets UI for players. */
    void WindowInitialize();

    void registerPlayer(Player* p1, Player* p2);
    /* draw Player UserInterface. */
    void drawUI();

    /* draw Memubar. */
    void drawMemubar() const;
    
    void setBoardSize(int size);
    int boardsize() const;

    void askExit();
    /* Free Memory, end ncurses. */
    void ExitGame(int mode=0);
 private:
    GameManager();  // private constructor.
    Player* p1_;
    Player* p2_;
    void resetTerm();
    static GameManager* instance_;
    Board* gboard_;
    int boardsize_;
    int termX_;     // terminal X size.
    int termY_;     // terminal Y size.
};
#endif  // headerguard
