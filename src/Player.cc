#include "Player.hpp"

Player::Player(short playercode):playercode_(playercode) {}
void Player::turnSet(bool turn) {
    turn_ = turn;
}
void setScore(int score) {
    score_ = score;
}
int Player::getScore() const {
    return score_;
}
/*
bool Player::putPiece(int** b_, int y, int x) {
    
}
*/
