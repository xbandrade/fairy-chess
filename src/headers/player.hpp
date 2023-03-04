#ifndef PLAYERS_HPP
#define PLAYERS_HPP
#include <iostream>
#include <string>
#include <vector>
#include "piece.hpp"

class Player{
    private:
        int id;
        bool isBot;
        std::string color;
    public:
        Player(int playerId, bool isBot);
        std::unordered_map<int, std::vector<std::string>> possibleMoves;
        int getId();
        std::string getColor();
};

#endif