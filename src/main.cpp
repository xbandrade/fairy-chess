#include <iostream>
#include "headers/board.hpp"
#include "headers/players.hpp"
#include "headers/pieces.hpp"
#include "headers/game.hpp"

int main(void){
    std::cout << "\n~~~ ✰✰ FAIRY CHESS ✰✰ ~~~\n";
    int option;
    std::string options = "1 - Player vs Player\n2 - Player vs Bot\n3 - Help\n4 - Exit\n-> ";
    std::cout << "Choose an option:\n" << options;
    std::cin >> option;
    while (std::cin.fail() || option < 0 || option > 3) {
        std::cout << "\nInvalid input. Enter a valid option:\n" << options;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> option;
    }
    switch (option){
        case 1:{
            std::vector<Piece *> positions(64, nullptr);
            std::cout << "\n           ~ ✰ FAIRY CHESS ✰ ~\n";
            play(positions);
            break;
        }
        case 2:{
            std::cout << "This is not available yet!\n";
            return 0;
        }
        case 3:{
            std::cout << "\n~~~ ✰✰ FAIRY CHESS ✰✰ ~~~\n";
            std::cout << "\nFairy Chess is a type of chess variant that modifies some rules, pieces and/or \n"
                      << "the board of traditional chess!\n"
                      << "This implementation of Fairy Chess includes three new Fairy Pieces with unique \n"
                      << "movement patterns:\n"
                      << "The Wazir: It has the same movement pattern as the Rook, but can only\n"
                      << "\tmove a single square, making it very weak.\n"
                      << "\tIts symbol in algebraic notation is W and it is worth as much as a Pawn.\n"
                      << "The Camel: It moves 3 squares forward and one to the side, in a long L shape.\n"
                      << "\tIt can also jump over other pieces, just like the Knight.\n"
                      << "\tIts symbol in algebraic notation is C and it is worth as much as a Knight.\n"
                      << "The Grasshopper: It has the same movement pattern as the Queen, but can only\n"
                      << "\tmove by hopping over another piece, then it lands immediately behind that piece.\n"
                      << "\tIf there is no pieces to hop over, the Grasshopper cannot move!\n"
                      << "\tIts symbol in algebraic notation is G and it is worth as much as a Knight.\n"
                      << "All the other pieces have their traditional movement pattern and exchange value.\n"
                      << "Moves:\n"
                      << "You can move a piece by using its algebraic notation:\n"
                      << "\tQe5 moves the Queen to e5\n"
                      << "\tGxd4 moves the Grasshopper to d4, capturing a piece\n"
                      << "\th3 moves the a Pawn to h3\n"
                      << "➜Entering 'm' followed by a position will show all possible moves for the piece on \n"
                      << "\tthat position: if the Wazir is on b6, 'mb6' will show all available moves \n"
                      << "\tfor that Wazir.\n"
                      << "➜Entering 'm*' will show all possible moves for the current player.\n"
                      << "➜Entering 'M1' will take you to a mate in 1 position for the black pieces.\n\n";
            break;
        }
        case 4:{
            std::cout << "\nExiting...\n";
            return 0;
        }
        default:{
            std::cout << "\nInvalid option!\n";
            break;
        }
    }
    return 0;
}