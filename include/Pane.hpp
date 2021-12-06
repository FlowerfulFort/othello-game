#ifndef INCLUDE_PANE_HPP_
#define INCLUDE_PANE_HPP_
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
#endif  // INCLUDE_PANE_HPP_
