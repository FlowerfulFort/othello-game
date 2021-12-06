#ifndef __BOARD_HPP__       // headerguard.
#define __BOARD_HPP__
#include <ncurses.h>        // Ncurses
#include <utility>          // pair<T, V>
#include "GameManager.hpp"  // Class GameManager.
#include "Pane.hpp"         // Class Pane

/* 정우건님의 코드에서 Position을
   std::pair<int, int>로 대체함. */
typedef std::pair<int, int> Pos;    // <y, x>
class GameManager;

class Board : public Pane{      // Class Pane 상속.
 public:
    /* 생성자. 보드사이즈와 UI의 위치 기준점을 입력받습니다. */
    explicit Board(int boardsize, int y, int x);

    /* gameboard_를 리턴합니다. */
    int** returnBoard() const;

    /* 포인터가 현재 가리키는 위치값을 갱신합니다. */
    void nowPointing(const int& y, const int& x);

    /* 현재 포인터가 가리키는 좌표를 반환합니다. */
    Pos getPointing() const;

    /* 유저가 돌을 놓는 함수입니다. 
       어떤 유저인지는 code로 구분합니다.
       입력받은 code로 돌을 뒤집습니다.
       예를 들면, 흑돌의 코드가 2인데, 돌을 뒤집을때
       gameboard_의 해당 셀의 값을 2로 바꾸는 것입니다.
       point로 말을 놓는 위치를 입력받습니다. 
       이름이 ReverseCaller인 이유는 아래의 Reverse함수를
       여러번 호출하는 Caller function이기 때문입니다. */
    void ReverseCaller(int code, const Pos& point);

    /* start 지점에 놓을수 있는지 체크하는 함수. 
       리턴 값으로 뒤집는 목표지점의 좌표를 리턴합니다. */
    Pos canReverse(int code, const Pos& start, const Pos& diff);

    /* start부터 dest까지 code로 뒤집는 함수. */
    void Reverse(int code, const Pos& start, const Pos& dest, const Pos& diff);
    /* 각 유저의 piece_aval_ 맵을 업데이트하는 함수 
       돌이 놓여서 gameboard_ 배열이 수정될때 호출됩니다. */
    void UpdateRevMap(int code);

    /* 각 유저의 aval_값을 갱신합니다.
       즉, code로 넘겨진 유저가 지금 판에 돌을 놓을수 있는지
       리턴합니다. */
    bool pieceAval(int code, const Pos& p) const;

    /* 자신이 턴을 가질수 있는지 반환하는 함수. 
       aval_이 private 멤버이기 때문에 이를 리턴하는
       getter 함수입니다. */
    bool isTurnAval(int code) const;

    /* 스코어를 계산하여 pair<int, int>형으로 반환합니다.
       흑돌의 스코어가 pair<>.first이고, 
       백돌의 스코어가 pair<>.second 입니다. */
    std::pair<int, int> calcScore() const;
    
    /* 주황색 사용자 돌 포인터를 각 방향으로 움직이는 함수입니다. */
    void pointUp();
    void pointDown();
    void pointLeft();
    void pointRight();
#ifdef PRETESTING
    void UpdateWindow();    // 테스팅용 코드.
#else
    /* 클래스 Pane에서 상속받은 virtual 함수입니다.
       Ncurses Window에 보드UI를 업데이트합니다. */
    void UpdateWindow() const;
#endif
    /* 소멸자. 할당된 wboard_, gameboard_ 등
       각종 동적할당 자원들을 해제합니다. */
    ~Board();
 private:
    /* calculate drawing pos. */
    WINDOW*** wboard_;      // 보드판을 그리기 위한 WINDOW.
    /* 실제 보드의 정보를 기록하는 gameboard_
       1=아무것도 없음, 2=흑돌, 3=백돌 */
    int** gameboard_;
    /* 유저가 해당 칸에 돌을 올릴수 있는지 나타내는 3차원배열.
       piece_aval[0] = 흑돌의 보드.
       piece_aval[1] = 백돌의 보드.
       예를 들어, piece_aval[0][7][4]는 흑돌이 y=7, x=4칸에 놓을수 있는지
       나타냄. */
    bool*** piece_aval_;
    /* 유저가 해당 턴에 돌을 놓을 수 있는지 나타낸다.
       aval_[0] = 흑돌.
       aval_[1] = 백돌.
       예를 들어, 백돌이 존재하지 않아 놓을 수 없을 때,
       aval_[1]은 false의 값이 된다. */
    bool aval_[2];
    int boardsize_;     // 보드사이즈.
    int pointx_;        // 사용자 포인터의 x좌표
    int pointy_;        // 사용자 포인터의 y좌표
    /* GameManager는 Board의 private 멤버에 접근 가능 */
    friend GameManager;
};
#endif  // headerguard
