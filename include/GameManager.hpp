#ifndef __GAMEMANAGER_HPP__
#define __GAMEMANAGER_HPP__
class GameManager {
 public:
    static GameManager* GetManager();
    void WindowInitialize();
    void ExitGame();
 private:
    GameManager();
    static GameManager* instance_;
};
#endif  // headerguard
