#include <ncurses.h>
#include "GameManager.hpp"

int main(void) {
    GameManager* gm = GameManager::GetManager();
    gm->GameInitialize();
    gm->WindowInitialize();
    getch();    // wait for confirm test.
    gm->ExitGame();
    return 0;
}
