#ifndef GAME_HPP
#define GAME_HPP
#include <iostream>
#include <vector>
#include <unordered_map>
#include "players.hpp"

void piecesLocation(std::unordered_map<std::string, std::vector<int>> &pieces, std::vector<Piece *> &positions);
void algebraicMap(std::unordered_map<std::string, int> &positionMap);
int algebraicToPosition(std::string move, std::unordered_map<std::string, int> &positionMap);
std::string validateMove(std::string move, std::unordered_map<std::string, int> &piecesHashMap);
void play(std::vector<Piece *> &positions);
void updateAllowedMoves(std::vector<Piece *> &positions);

#endif