#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include "GameManager.h"

int main(void) {
    GameManager* gm = GameManager::GetManager();
    gm->WindowInitialize();
    
    printw("Hello Ncurses!");
    refresh();
    sleep(1);

    WINDOW * win = newwin(10, 40, 10, 20);  // height, width, start_y, start_x
    refresh();
    sleep(1);
    box(win, 0, 0);
    sleep(1);
    mvwprintw(win, 5, 10, "Hi %s", "Ncurses");
    wrefresh(win);
    for(int i=0;i<3;i++) {
        mvprintw(0, 0, "hi %d", i);
        refresh();
        sleep(1);
    }
    refresh();
    getch();

    gm->ExitGame();
    return 0;
}
