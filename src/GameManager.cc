#include <ncurses.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <clocale>
#include <csignal>
#include "GameManager.hpp"
#include "Board.hpp"
#include "PlayerPane.hpp"
#include "Player.hpp"
#ifdef PRETESTING
#include "testplayer.hpp"
#endif
/* 토스트 메세지의 창 크기를 결정하는 상수입니다. */
constexpr int boardMsgSizeY = 7;
constexpr int boardMsgSizeX = 40;
constexpr int exitMsgSizeY = 7;
constexpr int exitMsgSizeX = 40;
// constexpr int winnerMsgSizeY = 36;
// constexpr int winnerMsgSizeX = 7;
/* moved into GameManager.hpp
constexpr int color_board = 1;
constexpr int color_p1 = 2;     // black.
constexpr int color_p2 = 3;     // white.
constexpr int color_pointer = 4;
constexpr int color_alert = 7;
*/
/* Singleton Class */
GameManager* GameManager::instance_ = nullptr;
GameManager* GameManager::GetManager() {
    if (!instance_) {
        instance_ = new GameManager();
    }
    return instance_;
}
/* 각 포인터를 nullptr로 초기화합니다. */
GameManager::GameManager() {
    p1_ = nullptr;
    p2_ = nullptr;
    windows = nullptr;
}

/* signal handler for SIGINT */
void sigint_handle(int sig) {
    GameManager* ins = GameManager::GetManager();
    // ins->ExitGame(1);
    ins->askExit();
}
/* 프로그램이 시작되기 전 초기 세팅단계입니다. */
void GameManager::GameInitialize() const {
    /* 유니코드 출력을 위한 locale 설정입니다. */
    std::setlocale(LC_ALL, "");
    /* Ctrl+C 입력에 대응하는 signal handler 입니다. */
    std::signal(SIGINT, sigint_handle);
}
/* COLOR_PAIR(x)로 표현되는 색깔을 세팅합니다.
   예를 들어 init_pair(2, COLOR_WHITE, COLOR_BLACK);
   의 경우에는 2번에 흰색글자, 검은색 배경을 설정합니다.
   따라서 COLOR_PAIR(2)로 검은배경에 흰글자를
   표현할 수 있습니다. */
void GameManager::InitializeColorSet() const {
    init_pair(color_board, COLOR_BLACK, COLOR_GREEN);
    init_pair(color_p1, COLOR_WHITE, COLOR_BLACK);
    init_pair(color_p2, COLOR_BLACK, COLOR_WHITE);
    init_pair(color_pointer, COLOR_BLACK, COLOR_YELLOW);
    init_pair(color_alert, COLOR_BLACK, COLOR_WHITE);
}
void GameManager::WindowInitialize() {
    initscr();              // Ncurses 시작.
    noecho();
    curs_set(false);        // 커서숨기기
    cbreak();
    start_color();          // 컬러표현 시작
    InitializeColorSet();   // 컬러세트 초기화
    resetTerm();            // 터미널 y, x크기 수집.
    // getmaxyx(stdscr, termY_, termX_); -> resetTerm();

    /* draw Message into center of screen. */
    int msgStartY = (termY_ - boardMsgSizeY) / 2;
    int msgStartX = (termX_ - boardMsgSizeX) / 2;
    /* 새로운 WINDOW를 생성합니다. */
    WINDOW* boardMsg = subwin(stdscr,
        boardMsgSizeY, boardMsgSizeX, msgStartY, msgStartX);
    box(boardMsg, 0, 0);        // 경계선을 만듭니다.
    /* 사용자에게 보드사이즈 크기를 입력받습니다. */
    mvwprintw(boardMsg, 2, 9, "Select the board size");
    init_pair(color_alert, COLOR_BLACK, COLOR_WHITE);
    int type = 6;           // 현재 선택중인 목록의 넘버입니다.
    bool flag = true;       // 루프 조건입니다.
    keypad(boardMsg, true);     // 특수키 입력을 허용합니다.
    while (flag) {          // flag가 false가 될때까지 반복
        /* 메세지 창에 출력합니다. */
        mvwprintw(boardMsg, 4, 2, "  6x6      8x8     10x10     Custom ");
        /* 색깔을 지정합니다(선택된 것을 하이라이팅). */
        wattrset(boardMsg, COLOR_PAIR(color_alert));
        /* 현재 선택중인 목록을 하이라이팅합니다. */
        switch (type) {
            case 6:
            mvwprintw(boardMsg, 4, 2, "  6x6  ");
            break;

            case 8:
            mvwprintw(boardMsg, 4, 11, "  8x8  ");
            break;

            case 10:
            mvwprintw(boardMsg, 4, 20, " 10x10 ");
            break;

            case 12:
            mvwprintw(boardMsg, 4, 30, " Custom ");
            break;
        }   // endswitch
        wrefresh(boardMsg);     // 창을 업데이트합니다.
        wattroff(boardMsg, COLOR_PAIR(color_alert));    // 색깔중지.
        int arrow = wgetch(boardMsg);       // 입력을 하나 받습니다.
        switch (arrow) {
            case KEY_LEFT:   // 왼쪽 화살표 입력
            if (type > 6) type-=2;
            break;

            case KEY_RIGHT:  // 오른쪽 화살표 입력
            if (type < 12) type+=2;
            break;

            case '\n':      // KEY_ENTER.
            if (type == 12) {   // when Custom board size.
                /* 커스텀일때, 사용자로부터 입력을 받아야 합니다. */
                mvwprintw(boardMsg, 5, 2,
                    "Enter even number(>=6): ");
                echo();         // 사용자의 입력을 보이게 합니다.
                curs_set(1);    // 커서를 보이게 합니다.
                /* goto에 사용할 INPUT 라벨입니다. */
                INPUT:
                wscanw(boardMsg, "%d", &type);  // type에 입력받습니다.
                /* 지정된 값을 벗어난 경우입니다. */
                if (type % 2 == 1 || type < 6 || type > 32) {
                    /* 입력란을 공백으로 채웁니다. */
                    mvwprintw(boardMsg, 5, 26, "    ");
                    /* 커서를 다시 옮깁니다. */
                    wmove(boardMsg, 5, 26);
                    /* 입력을 위해 INPUT라벨로 돌아갑니다. */
                    goto INPUT;
                }
                /* 적절한 입력이 된 경우입니다. */
                noecho();       // 사용자의 입력을 숨깁니다.
                curs_set(0);    // 커서를 숨깁니다.
                goto MSGEND;    // 루프를 즉시 탈출합니다.
            }
            /* 커스텀사이즈가 아닌 경우입니다. */
            flag = false;   // flag를 false로 세팅해 루프탈출.
            break;
        }   // endswitch
        // sleep(1);   // for testing.
    }   // endwhile
    MSGEND:
    boardsize_ = type;      // 결정된 보드사이즈를 입력합니다.
    delwin(boardMsg);       // 메세지UI를 삭제합니다.
    clear();                // 화면을 지웁니다.
    refresh();              // 지운 화면을 업데이트합니다.

    /* windows 변수는 Pane의 주소값을 저장하는 vector입니다. */
    windows = new std::vector<Pane*>();

    /* 흑돌이 언제나 선공입니다. */
    p1_ = new Player(2);    // 흑돌.
    p2_ = new Player(3);    // 백돌.
    p1_->turnSet(true);     // 선공.
    p2_->turnSet(false);    // 후공.

    /* Ncurses 표준 스크린을 리프레쉬합니다. */
    touchwin(stdscr);
    refresh();

    /* 초기화가 완료되었음을 나타냅니다. */
    is_init = true;
}
/* 현재 터미널사이즈를 termY_, termX_에 저장합니다. */
void GameManager::resetTerm() {
    getmaxyx(stdscr, termY_, termX_);
}

/* 게임을 종료할때 묻는 메세지입니다.
   이걸 상세하게 보고서에 쓸 필요는 없을것 같습니다. */
void GameManager::askExit() {
    int msgStartY = (termY_ - exitMsgSizeY) / 2;
    int msgStartX = (termX_ - exitMsgSizeX) / 2;
    int type = 0;
    WINDOW* msg = newwin(exitMsgSizeY, exitMsgSizeX,
        msgStartY, msgStartX);
    box(msg, 0, 0);
    mvwprintw(msg, 2, 5, "Are you sure want to exit game?");
    keypad(msg, true);
    bool flag = true;
    while (flag) {
        mvwprintw(msg, 4, 8, "   OK   ");
        mvwprintw(msg, 4, 23, " Cancel ");
        wattrset(msg, COLOR_PAIR(color_alert));
        switch (type) {
            case 0:
            mvwprintw(msg, 4, 23, " Cancel ");
            break;

            case 1:
            mvwprintw(msg, 4, 8, "   OK   ");
            break;
        }
        wattroff(msg, COLOR_PAIR(color_alert));
        int input = wgetch(msg);
        switch (input) {
            case KEY_LEFT:
            type = type ? 0 : 1;
            break;

            case KEY_RIGHT:
            type = !type ? 1 : 0;
            break;

            case '\n':
            flag = false;
            break;
        }
    }
    if (type) {     // when pressed OK
        ExitGame();
    }
    delwin(msg);
    touchwin(stdscr);
    refresh();
    // is_init = false;
}
/* 게임을 종료하는 함수입니다.
   이 역시 자세하게 쓸 필요는 없습니다. */
void GameManager::ExitGame(int mode /*default=0*/) {
    if (mode == 0) {
        endwin();   // Ncurses end.
        exit(0);    // exit successful.
    }
    /* must be filled Player delete */
    for (auto w : *windows) {
        delete w;
    }
    endwin();
    delete windows;
    /* whem mode != 0,
       the program will not be ended. 
       it exists for game restart. */
}
/* 초기 화면을 구성하는 함수입니다. */
void GameManager::drawUI() {
    /* 흑돌의 PlayerPane UI의 시작좌표 */
    int p1Y = 0;
    int p1X = 0;

    /* 백돌의 PlayerPane UI의 시작좌표 */
    int p2Y = p1Y + height_;
    int p2X = 0;

    /* 보드UI의 시작좌표 */
    int boardY = 0;
    int boardX = width_;
    clear();        // 화면을 지웁니다.

    if (boardsize_ < 4) {   // 보드사이즈 예외입니다.
        endwin();
        std::cerr << "boardsize error" << std::endl;
        exit(1);
    }
    /* 새로운 보드를 만듭니다. */
    Board* b_ = new Board(boardsize_, boardY, boardX);
    /* windows 벡터에 푸쉬합니다. Board가 Pane을 상속하기에 
       가능합니다. */
    windows->push_back(b_);

    /* 흑돌의 PlayerPane을 생성합니다. 이 역시 windows에
       푸쉬하는데, PlayerPane도 Pane을 상속하기에 가능합니다.
       이렇게 Pane을 상속하는 인스턴스들을 푸쉬하는 이유는
       virtual 함수인 UpdateWindow() 함수를 일괄적으로
       호출하기 위함입니다. */
    PlayerPane* pp = new PlayerPane(p1_, p1Y, p1X);
    windows->push_back(pp);

    /* 백돌도 마찬가지입니다. */
    pp = new PlayerPane(p2_, p2Y, p2X);
    windows->push_back(pp);

    p1_->registerBoard(b_);
    p2_->registerBoard(b_);

    /* windows 벡터의 세팅이 끝났으니,
       RefreshWindow()로 UI를 업데이트 합니다. */
    RefreshWindow();
}

/* UI업데이트를 위한 함수입니다. 
   Pane을 상속하는 클래스들의 virtual 함수인
   UpdateWindow() 함수를 일괄적으로 호출합니다. */
void GameManager::RefreshWindow() const {
    if (!is_init) return;
    for (auto w : *windows) {
        w->UpdateWindow();
    }
    touchwin(stdscr);       // 화면을 리프레쉬합니다.
    refresh();
}
/* 나중에 bool형을 반환하여 게임을 재시작할 것인지
   표시할 수도 있을듯. */
void GameManager::GameProcess() {
    /* 게임 보드. */
    Board* bd = dynamic_cast<Board*>((*windows)[0]);
    if (bd == nullptr) {    // 예외처리입니다.
        endwin();
        std::cerr << "Board Point Error!" << std::endl;
        exit(1);
    }
    /* 보드포인터를 입력받습니다. */
    int** gameboard = bd->returnBoard();
    bd->nowPointing(0, 0);  // 주황색 포인터를 0, 0으로 세팅합니다.
#ifndef PRETESTING
    Player* now = p1_;      // 흑돌이 선공입니다.
#else
    __testplayer* now = p1_;
#endif

    keypad(stdscr, true);   // 특수키 입력을 허용합니다.
    /* 각 플레이어의 돌을 놓을수 있는 위치들을
       갱신합니다. 이제 항상 나오지만 code를 입력받는
       함수들은 2가 흑돌이고 3이 백돌임을 기억하시면 됩니다. */
    bd->UpdateRevMap(2);    // 흑돌
    bd->UpdateRevMap(3);    // 백돌
    while (!isGameEnded(gameboard)) { /* !isGameEnded(gameboard) */
        /* 현재 플레이어의 코드를 받습니다. */
        int pcode = now->getCode();
        /* 본인의 턴을 진행할 수 없다면,
           다음 플레이어의 턴으로 넘김. */
        if (!bd->isTurnAval(pcode)) {
            /* 턴을 넘깁니다. */
            p1_->turnSet(!(p1_->is_turn()));
            p2_->turnSet(!(p2_->is_turn()));
            now = (p1_->is_turn()) ? p1_ : p2_;
            continue;
        }
        int key = getch();      // 입력을 받습니다.
        switch (key) {
            /* 주황색 포인터를 움직입니다. */
            case KEY_UP:
            bd->pointUp();
            break;

            case KEY_DOWN:
            bd->pointDown();
            break;

            case KEY_LEFT:
            bd->pointLeft();
            break;

            case KEY_RIGHT:
            bd->pointRight();
            break;

            /* 엔터를 입력할 시 상황입니다. */
            case '\n': {
                /* 현재 포인터 좌표를 입력받습니다. */
                Pos point = bd->getPointing();
                /* 현재 플레이어가 그곳에 말을
                   놓을수 있는지 봅니다. */
                if (bd->pieceAval(pcode, point)) {
                    /* 놓을수 잇으면 놓습니다. */
                    bd->ReverseCaller(pcode, point);
                } else {
                    continue;   // 아니면 루프로 돌아갑니다.
                }
                /* 점수를 갱신해야 합니다. */
                auto [ sc_1, sc_2 ] = bd->calcScore();
                p1_->setScore(sc_1);    // 흑돌 점수
                p2_->setScore(sc_2);    // 백돌 점수

                /* 턴을 넘깁니다. */
                p1_->turnSet(!(p1_->is_turn()));
                p2_->turnSet(!(p2_->is_turn()));
                now = (p1_->is_turn()) ? p1_ : p2_;
                /* 어디에 돌을 놓을수 있는지를 다시 갱신합니다. */
                bd->UpdateRevMap(2);
                bd->UpdateRevMap(3);
                break;
            }

            default:    // 이외의 입력은 루프로 돌아갑니다.
            continue;
            break;
        }
        /* 판이 바뀌면 업데이트 합니다. */
        RefreshWindow();
    }
    // int msgStartY = (termY_ - winnerMsgSizeY) / 2;
    // int msgStartX = (termX_ - winnerMsgSizeX) / 2;

    /* 점수가 더 높은 쪽에 Winner 타이틀을 걸어줍니다. 
       windows에 푸쉬한 순서가 있기 때문에 하드코딩 했습니다. */
    if (p1_->getScore() > p2_->getScore()) {
        dynamic_cast<PlayerPane*>((*windows)[1])->setWin();
    } else if (p2_->getScore() > p1_->getScore()) {
        dynamic_cast<PlayerPane*>((*windows)[2])->setWin();
    }
    RefreshWindow();    // 화면을 업데이트 합니다.
}
/*
#ifndef PRETESTING
void GameManager::registerPlayer(Player* p1, Player* p2) {
    p1_ = p1; p2_ = p2;
}
#endif
*/
/* 정우건님 개선코드
   isAll과 isFull을 합쳤습니다. */
bool GameManager::isGameEnded(int** b_) {
    bool isTrue = true;
    int countarr[4];
    std::fill_n(countarr, 4, 0);

    for (int i = 0; i < boardsize_; i++) {
        for (int j = 0; j < boardsize_; j++) {
            countarr[b_[i][j]]++;
        }
    }
    /* 보드판이 꽉차거나, 플레이어가 말을 놓을 수 없을때 */
    if (countarr[1] == 0 || countarr[2] <= 0 || countarr[3] <= 0) {
        return true;
    }
    return false;
}
