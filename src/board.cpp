#include "headers/board.hpp"
#define WHITE_SQUARE_LINE "\u2588\u2588\u2588\u2588\u2588"
#define WHITE_SQUARE "\u2588"
#define BLACK_SQUARE_LINE "     "
#define BLACK_SQUARE " "

void printBoard(std::vector<Piece *> &positions){
    std::cout << "\n";
    std::cout << "    A    B    C    D    E    F    G    H   \n";
    bool isWhiteSquare = false;
    for (int i = 7; i >= 0; i--){
        isWhiteSquare = i % 2 == 0 ? false : true;
        for (int p = 0; p < 3; p++){
            if (p == 1){
                std::cout << i + 1 << " ";
            }
            else{
                std::cout << "  ";
            }
            for (int j = 0; j < 8; j++){
                if (p == 1){
                    const char *squareColor = isWhiteSquare ? WHITE_SQUARE : BLACK_SQUARE;
                    std::cout << squareColor;
                    if (positions[i * 8 + j]){
                        int player = positions[i * 8 + j]->player;
                        std::string id = positions[i * 8 + j]->id;
                        std::cout << (player == 1 ? "\033[1;100;97m " : "\033[46;30m ") << id << " \033[0m";
                    }
                    else{
                        std::cout << squareColor << squareColor << squareColor;
                    }
                    std::cout  << squareColor;
                }
                else{
                    std::cout << (isWhiteSquare ? WHITE_SQUARE_LINE : BLACK_SQUARE_LINE);
                }
                isWhiteSquare = !isWhiteSquare;
            }
            if (p == 1){
                std::cout << " " << i + 1;
            }
            else{
                std::cout << "  ";
            }
            std::cout << "\n";
        }
    }
    std::cout << "    A    B    C    D    E    F    G    H   \n";
} 

