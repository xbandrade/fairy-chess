#ifndef BOARD_HPP
#define BOARD_HPP
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "piece.hpp"

class Board{
    private:
        void createDefaultPieces(int player);
        void createAlgebraicMap();
    public: 
        std::vector<Piece *> pieces;
        bool gameStarted;
        Board();
        std::unordered_map<std::string, int> positionMap;
        std::unordered_map<std::string, std::vector<int>> piecesLocation;
        void updatePiecesLocation();
        void printBoard();
        void clearBoard();
        ~Board();
};



#endif