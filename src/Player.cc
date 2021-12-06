#include "Player.hpp"
#include "Board.hpp"

Player::Player(int playercode):
    playercode_(playercode), score_(2) {}
int Player::getCode() const {
    return playercode_;
}
void Player::turnSet(bool turn) {
    turn_ = turn;
}
bool Player::is_turn() const {
    return turn_;
}
void Player::setScore(int score) {
    score_ = score;
}
int Player::getScore() const {
    return score_;
}
void Player::registerBoard(Board* b) {
    gameboard_ = b;
}
/*
bool Player::putPiece(int** b_, int y, int x) {
    
}
*/
