#ifndef INCLUDE_TESTPLAYER_HPP_
#define INCLUDE_TESTPLAYER_HPP_

struct __testplayer {
    explicit __testplayer(int playercode):playercode_(playercode) {}
    int score_;
    int playercode_;
    bool turn_;
};

#endif  // INCLUDE_TESTPLAYER_HPP_
