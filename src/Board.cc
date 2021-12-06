#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include "GameManager.hpp"
#include "Board.hpp"
using std::make_pair;   // 두 변수를 받아 pair<T, V>로 만들어줌.
/* 비어있는 칸에 출력할 문자열. 3칸의 공백입니다. */
static const char* piece = "   ";

/* 정우건님의 코드에 있었던 dy, dx 배열을
   전역변수화 시킨것입니다. */
static Pos differences[8] = {
    { -1, -1 }, { -1, 0 }, { 0, -1 }, { 1, 0 },
    { 0, 1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
};

constexpr int codeoffset = 2;       // 컴파일타임 상수.

Board::Board(int boardsize, int y, int x):
    boardsize_(boardsize), Pane(y, x) {
    // mvprintw(1,1,"boardsize: %d", boardsize_);
    gameboard_ = new int*[boardsize_];      // boardsize_만큼 할당.
    wboard_ = new WINDOW**[boardsize_];     // boardsize_만큼 할당.
    for (int i=0; i < boardsize_; i++) {
        gameboard_[i] = new int[boardsize_];
        /* gameboard_[i] 주소값부터 boardsize_만큼의 데이터들을
           1로 초기화시키는 함수. */
        std::fill_n(gameboard_[i], boardsize_, 1);
        wboard_[i] = new WINDOW*[boardsize_];

        for (int j=0; j < boardsize_; j++) {
            /* 기본적인 UI 세팅. */
            /* 각 칸에 맞는 WINDOW를 생성합니다. */
            wboard_[i][j] = subwin(stdscr, 4, 7, i*3+starty_, j*6+startx_);
            wattron(wboard_[i][j], COLOR_PAIR(color_board));    // 색상변경.
            /* 해당 칸의 경계선을 만듭니다. */
            wborder(wboard_[i][j], '|', '|', '-', '-', '+', '+', '+', '+');
            /* 처음엔 비어있으니 공백으로 채웁니다. */
            mvwprintw(wboard_[i][j], 1, 1, "     ");
            mvwprintw(wboard_[i][j], 2, 1, "     ");
            wattroff(wboard_[i][j], COLOR_PAIR(color_board));
            // 색상변경원위치.
        }
    }
    /* 2==black , 3==white */
    /* 보드 기본세팅, 중앙에 흑돌과 백돌 두개를 엇갈리게 놓습니다. */
    gameboard_[boardsize_/2-1][boardsize_/2-1] = 3;
    gameboard_[boardsize_/2][boardsize_/2] = 3;
    gameboard_[boardsize_/2][boardsize_/2-1] = 2;
    gameboard_[boardsize_/2-1][boardsize_/2] = 2;

    piece_aval_ = new bool**[2];    // 2 players.
    /* 보드크기만큼 각 플레이어에게도 할당해 줍니다. */
    piece_aval_[0] = new bool*[boardsize_];
    piece_aval_[1] = new bool*[boardsize_];
    for (int i=0; i < boardsize_; i++) {
        piece_aval_[0][i] = new bool[boardsize_];
        std::fill_n(piece_aval_[0][i], boardsize_, false);
    }
    for (int i=0; i < boardsize_; i++) {
        piece_aval_[1][i] = new bool[boardsize_];
        std::fill_n(piece_aval_[1][i], boardsize_, false);
    }
    /* 초기에는 둘다 돌을 놓을 수 있습니다. 고로 true. */
    aval_[0] = true;
    aval_[1] = true;
    /* 유저 포인터의 초기좌표입니다. */
    pointx_ = 0;
    pointy_ = 0;
    /* Ncurses 스크린을 리프레시하는 함수입니다. */
    touchwin(stdscr);
}
int** Board::returnBoard() const { return gameboard_; }
void Board::nowPointing(const int& y, const int& x) {
    pointx_ = x;
    pointy_ = y;
}
#ifdef PRETESTING
void Board::UpdateWindow() {
#else
void Board::UpdateWindow() const {
#endif
    for (int i=0; i < boardsize_; i++) {
        for (int j=0; j < boardsize_; j++) {
            /* wboard_의 모든 요소를 순회합니다. */
            WINDOW* pos = wboard_[i][j];
            /* 편한 코딩을 위한 참조자입니다. */
            const int& n = gameboard_[i][j];
            /* UpdateWindow()의 핵심코드.
               COLOR_PAIR(x) 매크로는 입력된 x값의 색상쌍을 반환합니다.
               이를 wattron() 함수로 해당 WINDOW에 색깔을 표현할 수 있습니다.
               매크로에 넘겨지는 x값은 보드의 해당 셀의 값을 넘겨줍니다.
               예를들면, 지금 gameboard_[i][j] 셀이 비어있어 1의 값이 저장되어 있다면, 
               COLOR_PAIR(1)의 색깔로 공간을 채웁니다.
               또, 흑돌이 놓아져 있다면 x값이 2가 됩니다.
               이렇게 표현된 색깔로 비어있음, 흑돌, 백돌로 표현합니다.
               색상에 대한것은 GameManager::InitializeColor() 함수를 참조하세요.
            */
            wattron(pos, COLOR_PAIR(n));
            /* WINDOW pos의 y=1, x=2 좌표에 "   " 출력 */
            mvwprintw(pos, 1, 2, piece);        // 공백출력
            mvwprintw(pos, 2, 2, piece);        // 공백출력
            wattroff(pos, COLOR_PAIR(n));       // 색상해제
        }
    }
    /* 만약 포인터가 배열의 좌표를 벗어나 SIGSEGV의 위험이 있다면, */
    if (pointy_ < 0 || pointx_ < 0 ||
        pointy_ >= boardsize_ || pointx_ >= boardsize_) {
        endwin();   // Ncurses 종료
        /* 표준에러스트림에 에러문구 출력 */
        std::cerr << "pointx/y_ error occured" << std::endl;
        exit(0);    // 프로그램 종료.
    }
    /* 해당 포인터가 가리킬 WINDOW를 뽑음. */
    WINDOW* pos = wboard_[pointy_][pointx_];
    /* 위와 마찬가지로, pointer의 색상을 불러내 공백을 추력 */
    wattron(pos, COLOR_PAIR(color_pointer));
    mvwprintw(pos, 1, 2, piece);
    mvwprintw(pos, 2, 2, piece);
    wattroff(pos, COLOR_PAIR(color_pointer));
    /* Ncurses의 스크린을 리프레쉬 */
    touchwin(stdscr);
    /* 리프레쉬 */
    refresh();
}
/* 포인터를 이동시키는 함수 */
void Board::pointUp() {
    if (pointy_ <= 0) return;
    pointy_--;
}
void Board::pointDown() {
    if (pointy_ >= boardsize_-1) return;
    pointy_++;
}
void Board::pointLeft() {
    if (pointx_ <= 0) return;
    pointx_--;
}
void Board::pointRight() {
    if (pointx_ >= boardsize_-1) return;
    pointx_++;
}
/* 포인터를 Pos형(std::pair<int, int>형)으로 반환 */
Pos Board::getPointing() const {
    return std::make_pair(pointy_, pointx_);
}
/* 정우건님 개선코드 */
/* 최적화를 할수있는 여지가 있다면,
   뒤집을 수 있는 경우의 수를
   비트로 나타내어 8방향=1byte로 표현한다. 
   그 후 각각의 비트를 마스킹하여 뒤집으면 된다. */
Pos Board::canReverse(int code, const Pos& start, const Pos& diff) {
    /* 정우건님의 Possible() 함수를 개량하였습니다.
       start지점부터 <dy, dx> 방향으로 탐색합니다. */
    auto [ dy, dx ] = diff;
    auto [ ny, nx ] = start;
    bool flag = false;
    ny += dy; nx += dx;
    /* 보드의 dy dx 방향 끝까지 탐색 */
    while (ny >= 0 && nx >= 0 && ny < boardsize_ && nx < boardsize_) {
        if (gameboard_[ny][nx] == code) {
            if (flag) {     // 조건이 충족되어있다면,
                return make_pair(ny, nx);   // 해당 좌표 반환
            } else {        // 조건이 충족되어있지 않다면,
                return make_pair(-1, -1);   // <-1, -1>반환
            }
        } else if (gameboard_[ny][nx] == 1) {
            return make_pair(-1, -1);
        } else {    // 다른 돌을 만났다면,
            flag = true;    // 조건충족.
            // break;
        }
        ny += dy; nx += dx;
    }
    return make_pair(-1, -1);   // cant put piece.
}
/* 놓음과 동시에 단방향 뒤집기 기능을 함. */
void Board::Reverse(int code, const Pos& start,
    const Pos& dest, const Pos& diff) {
    auto [ ny, nx ] = start;
    auto [ dy, dx ] = diff;
    /* 목표지점까지 code로 채움 */
    while (ny != dest.first || nx != dest.second) {
        gameboard_[ny][nx] = code;
        ny += dy; nx += dx;
    }
}
/* 두 플레이어가 돌을 놓을 수 있는 맵을 업데이트함. */
void Board::UpdateRevMap(int code) {
    aval_[code-codeoffset] = false;
    for (int i=0; i < boardsize_; i++) {
        for (int j=0; j < boardsize_; j++) {
            bool flag = false;
            /* 해당 좌표에 돌이 있다면 */
            if (gameboard_[i][j] != 1)
                goto FLAGINPUT;     // 루프 건너뛰기
            for (int k=0; k < 8; k++) {
                /* 특정 지점(y=i, x=j)에서 
                   differences방향으로 돌을 놓을수 있는지 검사. */
                auto [ y, x ] =
                    canReverse(code, make_pair(i, j), differences[k]);
                if (y != -1) {      // 돌을 놓을수 있다면,
                    flag = true;    // flag 설정.
                    break;
                }
            }
            FLAGINPUT:
            /* 돌을 놓을 수 있는 맵을 구성. */
            piece_aval_[code-codeoffset][i][j] = flag;
            /* 플레이어가 돌을 놓을 수 있는지.
               어느곳중 하나라도 놓을 수 있다면 aval_[]의 값은 true. */
            aval_[code-codeoffset] = aval_[code-codeoffset] | flag;
        }
    }
}

bool Board::isTurnAval(int code) const {
    return aval_[code-codeoffset];  // 돌을 놓을수 있는지 반환.
}
/* 특정 플레이어가 해당 지점에 놓을 수 있는지를 반환 */
bool Board::pieceAval(int code, const Pos& p) const {
    return piece_aval_[code-codeoffset][p.first][p.second];
}
/* 특정 플레이어가 돌을 놓는 함수. */
void Board::ReverseCaller(int code, const Pos& point) {
    for (int i=0; i < 8; i++) {
        Pos dest = canReverse(code, point, differences[i]);
        if (dest.first == -1)   // 돌을 differences[i]방향으로 못놓는다면,
            continue;           // 다음루프.
        /* 돌을 놓을 수 있다면 differences[i]방향으로 뒤집기. */
        Reverse(code, point, dest, differences[i]);
    }
}
/* 각각 점수를 계산해 pair<int, int>형태로 반환. */
std::pair<int, int> Board::calcScore() const {
    int nums[4];
    /* 0으로 초기화 */
    std::fill_n(nums, 4, 0);
    for (int i=0; i < boardsize_; i++) {
        for (int j=0; j < boardsize_; j++) {
            nums[gameboard_[i][j]]++;
        }
    }
    return make_pair(nums[2], nums[3]);
}
/* 메모리 해제 */
Board::~Board() {
    for (int i=0; i < boardsize_; i++) {
        delete[] gameboard_[i];
        for (int j=0; j < boardsize_; j++) {
            delwin(wboard_[i][j]);
        }
        delete[] wboard_[i];
    }
    delete[] wboard_;
    delete[] gameboard_;
}
