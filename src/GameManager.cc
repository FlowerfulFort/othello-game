#include <ncurses.h>
#include <cstdlib>
#include <clocale>
#include <csignal>
#include "GameManager.h"

GameManager* GameManager::instance_ = nullptr;
GameManager* GameManager::GetManager() {
    if (!instance_) {
        instance_ = new GameManager();
    }
    return instance_;
}
GameManager::GameManager() {}
void sigint_handle(int sig);
void GameManager::WindowInitialize() {
    std::setlocale(LC_ALL, "");      // for UNICODE Output.
    initscr();
    std::signal(SIGINT, sigint_handle);

    noecho();
    curs_set(false);
}
void GameManager::ExitGame() {
    endwin();
}
void sigint_handle(int sig) {
    endwin();
    std::exit(0);
}
