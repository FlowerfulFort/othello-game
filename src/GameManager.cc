#include <ncurses.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <clocale>
#include <csignal>
#include <unistd.h>
#include "GameManager.hpp"
#include "Board.hpp"
#include "PlayerPane.hpp"
#include "Player.hpp"
#ifdef PRETESTING
#include "testplayer.hpp"
#endif

constexpr int boardMsgSizeY = 7;
constexpr int boardMsgSizeX = 40;
constexpr int exitMsgSizeY = 7;
constexpr int exitMsgSizeX = 40;
constexpr int winnerMsgSizeY = 36;
constexpr int winnerMsgSizeX = 7;
/* moved into GameManager.hpp
constexpr int color_board = 1;
constexpr int color_p1 = 2;     // black.
constexpr int color_p2 = 3;     // white.
constexpr int color_pointer = 4;
constexpr int color_alert = 7;
*/
GameManager* GameManager::instance_ = nullptr;
GameManager* GameManager::GetManager() {
    if (!instance_) {
        instance_ = new GameManager();
    }
    return instance_;
}
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

void GameManager::GameInitialize() const {
    std::setlocale(LC_ALL, "");     // for UNICODE output.
    std::signal(SIGINT, sigint_handle);
}
void GameManager::InitializeColorSet() const {
    init_pair(color_board, COLOR_BLACK, COLOR_GREEN);
    init_pair(color_p1, COLOR_WHITE, COLOR_BLACK);
    init_pair(color_p2, COLOR_BLACK, COLOR_WHITE);
    init_pair(color_pointer, COLOR_BLACK, COLOR_YELLOW);
    init_pair(color_alert, COLOR_BLACK, COLOR_WHITE);
}
void GameManager::WindowInitialize() {
    initscr();
    noecho();
    curs_set(false);
    start_color();
    InitializeColorSet();
    resetTerm();
    // getmaxyx(stdscr, termY_, termX_); -> resetTerm();

    /* draw Message into center of screen. */
    int msgStartY = (termY_ - boardMsgSizeY) / 2;
    int msgStartX = (termX_ - boardMsgSizeX) / 2;
    WINDOW* boardMsg = subwin(stdscr,
        boardMsgSizeY, boardMsgSizeX, msgStartY, msgStartX);
    box(boardMsg, 0, 0);
    
    mvwprintw(boardMsg, 2, 9, "Select the board size");
    init_pair(color_alert, COLOR_BLACK, COLOR_WHITE);
    int type = 6;
    bool flag = true;
    keypad(boardMsg, true);
    while (flag) {
        mvwprintw(boardMsg, 4, 2, "  6x6      8x8     10x10     12x12 ");
        wattrset(boardMsg, COLOR_PAIR(color_alert));
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
            mvwprintw(boardMsg, 4, 30, " 12x12 ");
            break;
        }   // endswitch
        wrefresh(boardMsg);
        wattroff(boardMsg, COLOR_PAIR(color_alert));
        int arrow = wgetch(boardMsg);
        switch (arrow) {
            case KEY_LEFT:
            if (type > 6) type-=2;
            break;

            case KEY_RIGHT:
            if (type < 12) type+=2;
            break;
            
            case '\n':      // KEY_ENTER.
            flag = false;
            break;
        }   // endswitch
        //sleep(1);   // for testing.
    }   // endwhile
    boardsize_ = type;
    delwin(boardMsg);
    clear();
    refresh();

    windows = new std::vector<Pane*>;
    p1_ = new Player(2);    // black.
    p2_ = new Player(3);    // whilte.
    p1_->turnSet(true);     // 선공.
    p2_->turnSet(false);

    touchwin(stdscr);
    refresh();
    is_init = true;
}
void GameManager::resetTerm() {
    getmaxyx(stdscr, termY_, termX_);
}
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
    if (type) {
        ExitGame();
    }
    delwin(msg);
    touchwin(stdscr);
    refresh();
    is_init = false;
}
void GameManager::ExitGame(int mode /*default=0*/) {
    if(mode == 0) {
        endwin();
        exit(0);
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
void GameManager::drawUI() {
    /* Hard coding, will be edited */
    int p1Y = 0;
    int p1X = 0;
    int p2Y = p1Y + height_;
    int p2X = 0;
    int boardY = 0;
    int boardX = width_;
    clear();
    if (boardsize_ < 4) {
        std::cerr << "boardsize error" << std::endl;
    }
    Board* b_ = new Board(boardsize_, boardY, boardX);
    windows->push_back(b_);
    /* !!!!!TESTING CODE START!!!!!*/
    //p1_ = new __testplayer(2);
    PlayerPane* pp = new PlayerPane(p1_, p1Y, p1X);
    //pp->player_->score_ = 2;
    //pp->player_->turn_ = true;
    windows->push_back(pp);
    
    //p2_ = new __testplayer(3);
    pp = new PlayerPane(p2_, p2Y, p2X);
    //pp->player_->score_ = 2;
    //pp->player_->turn_ = false;
    windows->push_back(pp);

    p1_->registerBoard(b_);
    p2_->registerBoard(b_);
    /* !!!!!TESTING CODE ENDED!!!!!*/
    RefreshWindow();
}
void GameManager::RefreshWindow() const {
    if (!is_init) return;
    for (auto w : *windows) {
        w->UpdateWindow();
    }
    touchwin(stdscr);
    refresh();
}
/* 나중에 bool형을 반환하여 게임을 재시작할 것인지
   표시할 수도 있을듯. */
void GameManager::GameProcess() {
    /* 게임 보드. */
    Board* bd = dynamic_cast<Board*>((*windows)[0]);
    if (bd == nullptr) {
        endwin();
        std::cerr << "Board Point Error!" << std::endl;
        exit(1);
    }
    int** gameboard = bd->returnBoard();
    bd->nowPointing(0, 0);
#ifndef PRETESTING
    Player* now = p1_;
#else
    __testplayer* now = p1_;
#endif
    /* 지금은 무한루프지만, GameManager가 
       보드판 조건을 판별해야 함. */
    keypad(stdscr, true);
    bd->UpdateRevMap(2);
    bd->UpdateRevMap(3);
    while (!isGameEnded(gameboard)) { /* !isGameEnded(gameboard) */
        int pcode = now->getCode();
        /* 본인의 턴을 진행할 수 없다면,
           다음 플레이어의 턴으로 넘김. */
        if (!bd->isTurnAval(pcode)) {
            p1_->turnSet(!(p1_->is_turn()));
            p2_->turnSet(!(p2_->is_turn()));
            now = (p1_->is_turn()) ? p1_ : p2_;
            continue;
        }
        int key = getch();
        switch (key) {
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

            case '\n': {
                Pos point = bd->getPointing();
                if (bd->pieceAval(pcode, point)) {
                    bd->ReverseCaller(pcode, point);
                } else continue;
                auto [ sc_1, sc_2 ] = bd->calcScore();
                p1_->setScore(sc_1);
                p2_->setScore(sc_2);

                p1_->turnSet(!(p1_->is_turn()));
                p2_->turnSet(!(p2_->is_turn()));
                now = (p1_->is_turn()) ? p1_ : p2_;
                /* Update PlayerScore Required */
                bd->UpdateRevMap(2);
                bd->UpdateRevMap(3);
                break;
            }

            default:
            continue;
            break;
        }
        RefreshWindow();
    }
    int msgStartY = (termY_ - winnerMsgSizeY) / 2;
    int msgStartX = (termX_ - winnerMsgSizeX) / 2;
    if (p1_->getScore() > p2_->getScore()) {
        dynamic_cast<PlayerPane*>((*windows)[1])->setWin();
    } else if (p2_->getScore() > p1_->getScore()) {
        dynamic_cast<PlayerPane*>((*windows)[2])->setWin();
    }
    RefreshWindow();
}
/*
#ifndef PRETESTING
void GameManager::registerPlayer(Player* p1, Player* p2) {
    p1_ = p1; p2_ = p2;
}
#endif
*/
/* 정우건님 개선코드 */
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
