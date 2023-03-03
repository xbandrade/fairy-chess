#include <iostream>
#include "headers/board.hpp"
#include "headers/players.hpp"
#include "headers/pieces.hpp"
#include "headers/game.hpp"

int main(void){
    std::cout << "\n           ~ ✰ FAIRY CHESS ✰ ~\n";
    std::vector<Piece *> positions(64, nullptr);
    play(positions);
    return 0;
}