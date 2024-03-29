#include "board.hpp"
#define WHITE_SQUARE_LINE "█████"
#define WHITE_SQUARE "█"
#define BLACK_SQUARE_LINE "     "
#define BLACK_SQUARE " "

Board::Board() : pieces(64, nullptr), gameStarted(false) {
    createAlgebraicMap();
    createDefaultPieces(1);
    createDefaultPieces(2);
}

void Board::printBoard() {
    std::cout << "\n";
    std::cout << "    A    B    C    D    E    F    G    H   \n";
    bool isWhiteSquare = false;
    for (int i = 7; i >= 0; --i) {
        isWhiteSquare = i % 2 == 0 ? false : true;
        for (int p = 0; p < 3; ++p) {
            if (p == 1) {
                std::cout << i + 1 << " ";
            }
            else {
                std::cout << "  ";
            }
            for (int j = 0; j < 8; ++j) {
                if (p == 1) {
                    const char *squareColor = isWhiteSquare ? WHITE_SQUARE : BLACK_SQUARE;
                    std::cout << squareColor;
                    if (pieces[i * 8 + j]) {
                        int player = pieces[i * 8 + j]->getPlayer();
                        std::string id = pieces[i * 8 + j]->getId();
                        /*if (id == "Q") {
                            id = "♕";
                        }
                        else if (id == "K") {
                            id = "♔";
                        }*/
                        std::cout << (player == 1 ? "\033[1;100;97m " : "\033[46;30m ") << id << " \033[0m";
                    }
                    else {
                        std::cout << squareColor << squareColor << squareColor;
                    }
                    std::cout  << squareColor;
                }
                else {
                    std::cout << (isWhiteSquare ? WHITE_SQUARE_LINE : BLACK_SQUARE_LINE);
                }
                isWhiteSquare = !isWhiteSquare;
            }
            if (p == 1) {
                std::cout << " " << i + 1;
            }
            else {
                std::cout << "  ";
            }
            std::cout << "\n";
        }
    }
    std::cout << "    A    B    C    D    E    F    G    H   \n";
}

void Board::clearBoard() {
    for (auto piece : pieces) {
        if (piece) {
            delete piece;
        }
    }
    pieces.assign(pieces.size(), nullptr);
    createDefaultPieces(1);
    createDefaultPieces(2);
}


void Board::updatePiecesLocation() {
    piecesLocation.clear();
    for (auto piece : pieces) {
        if (piece) {
            std::string key = std::to_string(piece->getPlayer()) + piece->getId();
            piecesLocation[key].push_back(piece->getPosition());
        }
    }
}


void Board::createDefaultPieces(int player) {
    if (player == 1) {
        pieces[0] = new Rook(0, 1);
        pieces[1] = new Knight(1, 1);
        pieces[2] = new Bishop(2, 1);
        pieces[3] = new Queen(3, 1);
        pieces[4] = new King(4, 1);
        pieces[5] = new Bishop(5, 1);
        pieces[6] = new Knight(6, 1);
        pieces[7] = new Rook(7, 1);
        pieces[8] = new Camel(8, 1);
        pieces[9] = new Wazir(9, 1);
        for (int i = 2; i < 6; ++i) {
            pieces[8 + i] = new Pawn(8 + i, 1);
        }
        pieces[14] = new Wazir(14, 1);
        pieces[15] = new Grasshopper(15, 1);
        pieces[16] = new Pawn(16, 1);
        pieces[17] = new Pawn(17, 1);
        pieces[22] = new Pawn(22, 1);
        pieces[23] = new Pawn(23, 1);
    }
    else {
        pieces[63] = new Rook(63, 2);
        pieces[62] = new Knight(62, 2);
        pieces[61] = new Bishop(61, 2);
        pieces[60] = new King(60, 2);
        pieces[59] = new Queen(59, 2);
        pieces[58] = new Bishop(58, 2);
        pieces[57] = new Knight(57, 2);
        pieces[56] = new Rook(56, 2);
        pieces[55] = new Grasshopper(55, 2);
        pieces[54] = new Wazir(54, 2);
        for (int i = 2; i < 6; ++i) {
            pieces[48 + i] = new Pawn(48 + i, 2);
        }
        pieces[49] = new Wazir(49, 2);
        pieces[48] = new Camel(48, 2);
        pieces[47] = new Pawn(47, 2);
        pieces[46] = new Pawn(46, 2);
        pieces[41] = new Pawn(41, 2);
        pieces[40] = new Pawn(40, 2);
    }
} 


void Board::createAlgebraicMap() {
    std::string files = "abcdefgh";
    int k = 0;
    for (int i = 1; i <= 8; ++i) {
        for (int j = 0; j < 8; ++j, ++k) {
            positionMap[files[j] + std::to_string(i)] = k;
        }
    }
}


Board::~Board() {
    for (auto piece : pieces) {
        delete piece;
    }
}
