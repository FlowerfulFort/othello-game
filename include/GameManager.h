class GameManager {
 public:
    static GameManager* GetManager();
    void WindowInitialize();
    void ExitGame();
 private:
    GameManager();
    static GameManager* instance_;
};
