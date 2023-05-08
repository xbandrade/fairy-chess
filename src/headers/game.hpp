#ifndef GAME_HPP
#define GAME_HPP
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <utility>
#include <queue>
#include <cstdlib>
#include <ctime>
#include "player.hpp"
#include "board.hpp"

int algebraicToPosition(std::string move, Board &board);

void positionToAlgebraic(int pos, char alg[2]);

std::string validateMove(std::string move, Player &currentPlayer);

void updateAllPossibleMoves(Board &board, Player &black, Player &white);

void updatePlayerScores(Board &board, Player &black, Player &white);

bool simulateMove(std::string move, Board &board, int turn, King *king, Player &black, Player &white);

void forceKingOutOfCheck(Board &board, int turn, King *king, Player &black, Player &white);

bool playMove(std::string move, Board &board, int turn, King *king, Player &black, Player &white);

void findKingThreats(King *king, Board &board);

void mateInOneB(Board &board, King *blackKing, King *whiteKing, Player &black, Player &white);

void mateInOneW(Board &board, King *blackKing, King *whiteKing, Player &black, Player &white);
              
bool isKingMated(Board &board, Player &black, Player &white, King *king, int player);

bool botPlay(Board &board, King *blackKing, King *whiteKing, Player &black, Player &white);
             
void play(Board &board, bool vsBot);

#endif