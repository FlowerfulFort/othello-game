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
/* GameManager의 운용.
   GameManager는 인스턴스가 하나만 존재하는 싱글톤 클래스입니다.
   게임에 관한 모든 움직임을 GameManager가 담당합니다.
   또, Pane 인스턴스들을 모두 관리하며, 
   UpdateWindows()를 일괄적으로 실행하는 RefreshWindow()로
   화면을 업데이트 합니다. */
#ifndef INCLUDE_GAMEMANAGER_HPP_
#define INCLUDE_GAMEMANAGER_HPP_
#include <vector>
#include "Player.hpp"
#include "Pane.hpp"
#ifdef PRETESTING
#include "testplayer.hpp"
#endif
/* COLOR_PAIR(x)에 의해 표현될 색깔의 상수들입니다. */
constexpr int color_board = 1;      // 빈칸
constexpr int color_p1 = 2;         // 흑돌
constexpr int color_p2 = 3;         // 백돌
constexpr int color_pointer = 4;    // 주황색 포인터
constexpr int color_alert = 7;      // 선택창

class Board;
class Player;
class GameManager {
 public:
    /* this class is Singleton Class. */
    static GameManager* GetManager();

    /* before "Program" start, this function must be called. */
    void GameInitialize() const;

    /* before "Game" start, this function must be called. */
    /* It sets UI for players. */
    void WindowInitialize();

    /* Initialize Color Pairs */
    void InitializeColorSet() const;

    /* Execute All of Defined Refresh Function. */
    void RefreshWindow() const;

    /* 실제로 쓰지않는 더미함수입니다. */
    void registerPlayer(Player* p1, Player* p2);
    /* draw Player UserInterface. */
    void drawUI();

    /* Game Process */
    void GameProcess();

    /* boardsize의 getter와 setter입니다. */
    void setBoardSize(int size);
    int boardsize() const;

    /* Check if Game was ended. */
    bool isGameEnded(int** b_);

    /* SIGINT가 입력되었을때 게임을 끝낼지 묻는 함수입니다. */
    void askExit();
    /* Free Memory, end ncurses. */
    void ExitGame(int mode = 0);

 private:
    GameManager();  // private constructor.
    bool is_init;
#ifndef PRETESTING
    Player* p1_;
    Player* p2_;
#else
    __testplayer* p1_;
    __testplayer* p2_;
#endif
    void resetTerm();
    static GameManager* instance_;
    /* for updating windows. */
    std::vector<Pane*>* windows;
    int boardsize_;
    int termX_;     // terminal X size.
    int termY_;     // terminal Y size.
};
#endif  // INCLUDE_GAMEMANAGER_HPP_
