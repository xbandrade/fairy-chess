#include <iostream>
#include <string>
#include <vector>
#include "headers/pieces.hpp"
#include "headers/players.hpp"


void createDefaultPieces(int player, std::vector<Piece *> &positions){
    if(player == 1){
        positions[0] = new Rook(0, 1);
        positions[1] = new Knight(1, 1);
        positions[2] = new Bishop(2, 1);
        positions[3] = new Queen(3, 1);
        positions[4] = new King(4, 1);
        positions[5] = new Bishop(5, 1);
        positions[6] = new Knight(6, 1);
        positions[7] = new Rook(7, 1);
        positions[8] = new Wazir(8, 1);
        positions[9] = new Camel(9, 1);
        for(int i = 2; i < 6; i++){
            positions[8 + i] = new Pawn(8 + i, 1);
        }
        positions[14] = new Grasshopper(14, 1);
        positions[15] = new Wazir(15, 1);
        positions[16] = new Pawn(16, 1);
        positions[17] = new Pawn(17, 1);
        positions[22] = new Pawn(22, 1);
        positions[23] = new Pawn(23, 1);
    }
    else{
        positions[63] = new Rook(63, 2);
        positions[62] = new Knight(62, 2);
        positions[61] = new Bishop(61, 2);
        positions[60] = new King(60, 2);
        positions[59] = new Queen(59, 2);
        positions[58] = new Bishop(58, 2);
        positions[57] = new Knight(57, 2);
        positions[56] = new Rook(56, 2);
        positions[55] = new Wazir(55, 2);
        positions[54] = new Grasshopper(54, 2);
        for(int i = 2; i < 6; i++){
            positions[48 + i] = new Pawn(8 + i, 2);
        }
        positions[49] = new Camel(49, 2);
        positions[48] = new Wazir(48, 2);
        positions[47] = new Pawn(47, 2);
        positions[46] = new Pawn(46, 2);
        positions[41] = new Pawn(41, 2);
        positions[40] = new Pawn(40, 2);
    }
} 
