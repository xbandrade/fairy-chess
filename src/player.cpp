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
