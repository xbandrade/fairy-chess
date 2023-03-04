#ifndef GAME_HPP
#define GAME_HPP
#include <iostream>
#include <vector>
#include <unordered_map>
#include "player.hpp"
#include "board.hpp"

void getPiecesLocation(Board &board, std::unordered_map<std::string, std::vector<int>> &piecesLocation);

void algebraicMap(std::unordered_map<std::string, int> &positionMap);

int algebraicToPosition(std::string move, std::unordered_map<std::string, int> &positionMap);

void positionToAlgebraic(int pos, char alg[2]);

std::string validateMove(std::string move, std::unordered_map<std::string, int> &piecesHashMap,
                         int turn, std::unordered_map<int, std::vector<std::string>> possibleMoves);

void updateAllPossibleMoves(Board &board, 
                            std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                            std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);

bool simulateMove(std::string move, Board &board, 
                  std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
                  std::unordered_map<std::string, int> &positionMap, int turn, King *king,
                  std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                  std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);

void forceKingOutOfCheck(Board &board, 
                         std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
                         std::unordered_map<std::string, int> &positionMap, int turn, King *king,
                         std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                         std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
                         );
bool playMove(std::string move, Board &board,
              std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
              std::unordered_map<std::string, int> &positionMap, int turn, King *king,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
              );
void findKingThreats(King *king, Board &board);
void mateInOneB(Board &board,
              std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
              std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);
void mateInOneW(Board &board,
              std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
              std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);
bool isKingMated(Board &board, std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves, 
                 std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                 std::unordered_map<std::string, std::vector<int>> &piecesLocation,
                 std::unordered_map<std::string, int> &positionMap,
                 King *king, int player);
bool botPlay(Board &board, 
             std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
             std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
             std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
             std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);
void play(Board &board, bool vsBot);

#endif