#ifndef __GAMEMANAGER_HPP__
#define __GAMEMANAGER_HPP__
class GameManager {
 public:
    /* this class is Singleton Class. */
    static GameManager* GetManager();

    /* before "Program" start, this function must be called. */
    void GameInitialize() const;
    
    /* before "Game" start, this function must be called. */
    /* It sets UI for players. */
    void WindowInitialize();

    /* draw Player UserInterface. */
    void drawPlayerUI();

    /* draw Memubar. */
    void drawMemubar() const;
    
    void setBoardSize(int size);
    int boardsize() const;

    void askExit();
    /* Free Memory, end ncurses. */
    void ExitGame(int mode=0);
 private:
    GameManager();  // private constructor.
    void resetTerm();
    static GameManager* instance_;
    int** board_;   // othello game board.
    int boardsize_;
    int termX_;     // terminal X size.
    int termY_;     // terminal Y size.
};
#endif  // headerguard
