#include <iostream>
#include "src/board.hpp"
#include "src/player.hpp"
#include "src/piece.hpp"
#include "src/game.hpp"


int main(int argc, char *argv[]){
    std::cout << "\n✰✰✰ FAIRY CHESS ✰✰✰    \n\n";
    int option;
    std::string options = "1 - Player vs Player\n2 - Player vs Bot\n3 - Help\n4 - Exit\n-> ";
    do {    
        std::cout << "\nChoose an option:\n" << options;
        std::cin >> option;
        switch (option){
            case 1:{
                Board board;
                std::cout << "\n           ~ ✰ FAIRY CHESS ✰ ~" << std::endl;
                play(board, false);
                break;
            }
            case 2:{
                Board board;
                std::cout << "\n           ~ ✰ FAIRY CHESS ✰ ~" << std::endl;
                play(board, true);
                break;
            }
            case 3:{
                std::cout << "\n    ✰✰✰ FAIRY CHESS ✰✰✰    \n";
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
                        << "The board and other chess rules remain unchanged.\n"
                        << "Moves:\n"
                        << "You can move a piece by using its algebraic notation:\n"
                        << "\tQe5 moves the Queen to e5\n"
                        << "\tGxd4 moves the Grasshopper to d4, capturing a piece\n"
                        << "\th3 moves the Pawn to h3\n"
                        << "➜ Entering 'm' followed by a position will show all possible moves for the piece on \n"
                        << "\tthat position: if the Wazir is on b6, 'mb6' will show all available moves \n"
                        << "\tfor that Wazir.\n"
                        << "➜ Entering 'm*' will show all possible moves for the current player.\n"
                        << "➜ Entering 'M1B' will take you to a mate in 1 position for the black pieces.\n"
                        << "➜ Entering 'M1W' will take you to a mate in 1 position for the white pieces.\n"
                        << "➜ Entering 'quit' during a match will take you to the main menu.\n"
                        << "❗When playing against the bot, it will play a valid move with high priority \n"
                        << "in the moves list. The move priority is decided by the sum of materials points \n"
                        << "after a move is played. The bot will always go for a checkmate if it sees a mate in one."
                        << std::endl;
                break;
            }
            case 4:{
                break;
            }
            default:{
                std::cout << "Invalid option!" << std::endl;;
                break;
            }
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (option != 4);
    std::cout << "\nExiting..." << std::endl;
    return 0;
}