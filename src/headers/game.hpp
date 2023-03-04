#ifndef GAME_HPP
#define GAME_HPP
#include <iostream>
#include <vector>
#include <unordered_map>
#include "players.hpp"

void piecesLocation(std::unordered_map<std::string, std::vector<int>> &pieces, std::vector<Piece *> &positions);

void algebraicMap(std::unordered_map<std::string, int> &positionMap);

int algebraicToPosition(std::string move, std::unordered_map<std::string, int> &positionMap);

void positionToAlgebraic(int pos, char alg[2]);

std::string validateMove(std::string move, std::unordered_map<std::string, int> &piecesHashMap,
                         int turn, std::unordered_map<int, std::vector<std::string>> possibleMoves);

void updateAllPossibleMoves(std::vector<Piece *> &positions, 
                            std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                            std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);

bool simulateMove(std::string move, std::vector<Piece *> &positions, 
                  std::unordered_map<std::string, std::vector<int>> &pieces, 
                  std::unordered_map<std::string, int> &positionMap, int turn, King *king,
                  std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                  std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);

void forceKingOutOfCheck(std::vector<Piece *> &positions, 
                         std::unordered_map<std::string, std::vector<int>> &pieces, 
                         std::unordered_map<std::string, int> &positionMap, int turn, King *king,
                         std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                         std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
                         );
bool playMove(std::string move, std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, int turn, King *king,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
              );
void findKingThreats(King *king, std::vector<Piece *> &positions);
void mateInOneB(std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);
void mateInOneW(std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);
bool isKingMated(std::vector<Piece *> &positions, std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves, 
                 std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                 std::unordered_map<std::string, std::vector<int>> &pieces,
                 std::unordered_map<std::string, int> &positionMap,
                 King *king, int player);
bool botPlay(std::vector<Piece *> &positions, 
             std::unordered_map<std::string, std::vector<int>> &pieces, 
             std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
             std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
             std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves);
void play(std::vector<Piece *> &positions, bool vsBot);

#endif