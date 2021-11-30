#ifndef __PANE_HPP__
#define __PANE_HPP__
class Pane{
 public:
    explicit Pane(int y, int x): starty_(y), startx_(x) {}
    virtual void UpdateWindow() const = 0;
    virtual ~Pane() {}
 protected:
    int startx_;
    int starty_;
};
#endif  // headerguard.
