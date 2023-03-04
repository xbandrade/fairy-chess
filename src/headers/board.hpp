#ifndef BOARD_HPP
#define BOARD_HPP
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "piece.hpp"

class Board{
    private:
        void createDefaultPieces(int player);
    public: 
        Board();
        std::vector<Piece *> pieces;
        void printBoard();
        ~Board();
};



#endif