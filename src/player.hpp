#ifndef PLAYERS_HPP
#define PLAYERS_HPP
#include <iostream>
#include <string>
#include <vector>
#include "piece.hpp"

class Player {
private:
    int id;
    int player_score;
    bool isBot;
    std::string color;
public:
    Player(int playerId, bool isBot);
    std::unordered_map<int, std::vector<std::string>> possibleMoves;
    int getId();
    int getScore();
    void setScore(int score);
    void subtractScore(int sub);
    std::string getColor();
};

#endif