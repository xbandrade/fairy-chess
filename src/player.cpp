#include <iostream>
#include <string>
#include <vector>
#include "headers/piece.hpp"
#include "headers/player.hpp"

Player::Player(int playerId, bool isBot){
    id = playerId;
    color = id == 1 ? "White" : "Black";
    this->isBot = isBot;
}

int Player::getId(){
    return id;
}

std::string Player::getColor(){
    return color;
}

int Player::getScore(){
    return player_score;
}

void Player::setScore(int score){
    player_score = score;
}

void Player::subtractScore(int sub){
    player_score -= sub;
}
