#ifndef __PANE_HPP__
#define __PANE_HPP__
class Pane{
 public:
    explicit Pane(int y, int x): starty_(y), startx_(x) {}
#ifdef PRETESTING
    virtual void UpdateWindow() = 0;
#else
    virtual void UpdateWindow() const = 0;
#endif
    virtual ~Pane() {}
 protected:
    int startx_;
    int starty_;
};
#endif  // headerguard.
