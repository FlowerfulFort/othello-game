#include <ncurses.h>
#include <cstdlib>
#include <clocale>
#include <csignal>
#include <unistd.h>
#include "GameManager.hpp"

constexpr int boardMsgSizeY = 7;
constexpr int boardMsgSizeX = 40;
constexpr int color_term = 1;
constexpr int color_board = 7;
GameManager* GameManager::instance_ = nullptr;
GameManager* GameManager::GetManager() {
    if (!instance_) {
        instance_ = new GameManager();
    }
    return instance_;
}
GameManager::GameManager() {
    board_ = nullptr;
}

/* signal handler for SIGINT */
void sigint_handle(int sig) {
    endwin();
    std::exit(0);
}

void GameManager::GameInitialize() const {
    std::setlocale(LC_ALL, "");     // for UNICODE output.
    std::signal(SIGINT, sigint_handle);
}
void GameManager::WindowInitialize() {
    initscr();
    noecho();
    curs_set(false);
    start_color();
    getmaxyx(stdscr, termY_, termX_);

    init_pair(color_term, COLOR_WHITE, COLOR_BLACK);
    /* draw Message into center of screen. */
    int msgStartY = (termY_ - boardMsgSizeY) / 2;
    int msgStartX = (termX_ - boardMsgSizeX) / 2;
    WINDOW* boardMsg = newwin(
        boardMsgSizeY, boardMsgSizeX, msgStartY, msgStartX);
    box(boardMsg, 0, 0);
    
    mvwprintw(boardMsg, 2, 12, "Set the board size.");
    init_pair(color_board, COLOR_BLACK, COLOR_WHITE);
    int type = 6;
    bool flag = true;
    keypad(boardMsg, true);
    while (flag) {
        mvwprintw(boardMsg, 4, 5, "6x6     8x8     10x10     12x12");
        wattrset(boardMsg, COLOR_PAIR(color_board));
        switch (type) {
            case 6:
            mvwprintw(boardMsg, 4, 5, "6x6");
            break;

            case 8:
            mvwprintw(boardMsg, 4, 13, "8x8");
            break;

            case 10:
            mvwprintw(boardMsg, 4, 21, "10x10");
            break;

            case 12:
            mvwprintw(boardMsg, 4, 31, "12x12");
            break;
        }   // endswitch
        wrefresh(boardMsg);
        wattroff(boardMsg, COLOR_PAIR(color_board));
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
    /* testing start*/
    mvprintw(1,1, "boardsize is %d", boardsize_);
    /* testing end*/
    touchwin(stdscr);
    refresh();
}
void GameManager::ExitGame() {
    if (board_) {
        for (int i=0;i<boardsize_;i++) {
            delete[] board_[i];
        }
        delete[] board_;
    }
    board_ = nullptr;
    endwin();
}
