#include <ncurses.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <clocale>
#include <csignal>
#include <unistd.h>
#include "GameManager.hpp"
#include "Board.hpp"

constexpr int boardMsgSizeY = 7;
constexpr int boardMsgSizeX = 40;
constexpr int exitMsgSizeY = 7;
constexpr int exitMsgSizeX = 40;
/*
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
    init_pair(color_p1, COLOR_BLACK, COLOR_BLACK);
    init_pair(color_p2, COLOR_WHITE, COLOR_WHITE);
    init_pair(color_pointer, COLOR_YELLOW, COLOR_YELLOW);
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
    /* testing start*/
    //mvprintw(1,1, "boardsize is %d", boardsize_);
    /* testing end*/
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
    int boardY = 1;
    int boardX = 1;
    clear();
    if (boardsize_ < 4) {
        std::cerr << "boardsize error" << std::endl;
    }
    windows->push_back(new Board(boardsize_, boardY, boardX));
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
