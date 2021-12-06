#ifndef INCLUDE_PANE_HPP_
#define INCLUDE_PANE_HPP_
/* 다형성을 위한 추상 클래스 */
class Pane{
 public:
    /* 생성자로 좌표를 입력받습니다. */
    explicit Pane(int y, int x): starty_(y), startx_(x) {}
#ifdef PRETESTING
    virtual void UpdateWindow() = 0;
#else
    /* UpdateWindow()로 각각의 UI창을 업데이트 합니다. */
    virtual void UpdateWindow() const = 0;
#endif
    virtual ~Pane() {}
 protected:
    int startx_;        // UI의 시작 x좌표
    int starty_;        // UI의 시작 y좌표
};
#endif  // INCLUDE_PANE_HPP_
