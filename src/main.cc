#include <ncurses.h>
#include "GameManager.hpp"

int main(int argc, char** argv) {
    GameManager* gm = GameManager::GetManager();
    gm->GameInitialize();
    gm->WindowInitialize();
    getch();    // wait for confirm test.
    gm->drawUI();
    getch();    // wait for comfirm test.
    gm->GameProcess();
    gm->askExit();
    mvprintw(2,1,"Exit Canceled");
    getch();    // wait for comfirm test.
    gm->ExitGame();
    return 0;
}
