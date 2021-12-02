#ifdef PRETESTING
#ifndef __TESTPLAYER_HPP__
#define __TESTPLAYER_HPP__

struct __testplayer {
    __testplayer(int playercode):playercode_(playercode) {}
    int score_;
    int playercode_;
    bool turn_;
};

#endif  // __TESTPLAYER_HPP__
#endif  // PRETESTING
