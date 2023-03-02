#include "headers/pieces.hpp"
#include <iostream>

int calcPos(int x, int y){
    return x * 8 + y;
}

Piece::Piece(int pos, int player_id){
    this->position = pos;
    this->player = player_id;
}

int Piece::getPos(){
    return 0;
}

Pawn::Pawn(int pos, int player_id) : Piece(pos, player_id)
{
    this->name = "Pawn";
    this->id = "P";
    std::cout << this->name << " created at " << this->position << "\n";
}

void Pawn::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves 1 (or 2) forward, captures diagonally
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    int pos = calcPos(x + 1, y);
    if(x < 7 && !positions[pos]){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 2, y);
    if(!this->hasMoved && !this->allowedMoves.empty() && !positions[pos]){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 1, y - 1);
    if(x < 7 && y > 0 && positions[pos] && (positions[pos]->player != this->player)){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 1, y + 1);
    if(x < 7 && y < 7 && positions[pos] && (positions[pos]->player != this->player)){
        this->allowedMoves.push_back(pos);
    }
}

Knight::Knight(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Knight";
    this->id = "N";
    std::cout << this->name << " created at " << this->position << "\n";  
}

void Knight::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves in an L shape (1-2), can jump over pieces
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    if(x > 1 && y > 0){
        this->allowedMoves.push_back(calcPos(x - 2, y - 1));
    }
    if(x > 1 && y < 7){
        this->allowedMoves.push_back(calcPos(x - 2, y + 1));
    }
    if(x < 6 && y > 0){
        this->allowedMoves.push_back(calcPos(x + 2, y - 1));
    }
    if(x < 6 && y < 7){
        this->allowedMoves.push_back(calcPos(x + 2, y + 1));
    }
    if(x > 0 && y > 1){
        this->allowedMoves.push_back(calcPos(x - 1, y - 2));
    }
    if(x < 7 && y > 1){
        this->allowedMoves.push_back(calcPos(x + 1, y - 2));
    }
    if(x > 0 && y < 6){
        this->allowedMoves.push_back(calcPos(x - 1, y + 2));
    }
    if(x < 7 && y < 6){
        this->allowedMoves.push_back(calcPos(x + 1, y + 2));
    }
}

Bishop::Bishop(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Bishop";
    this->id = "B";
    std::cout << this->name << " created at " << this->position << "\n"; 
}

void Bishop::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves diagonally
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    for(int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--){
        int pos = calcPos(i, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int i = x - 1, j = y + 1; i >= 0 && j < 8; i--, j++){
        int pos = calcPos(i, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int i = x + 1, j = y - 1; i < 8 && j >= 0; i++, j--){
        int pos = calcPos(i, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j--){
        int pos = calcPos(i, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
}

Rook::Rook(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Rook";
    this->id = "R";
    std::cout << this->name << " created at " << this->position << "\n"; 
}

void Rook::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves horizontally and vertically
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    for(int i = x - 1; i >= 0; i--){
        int pos = calcPos(i, y);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int i = x + 1; i < 8; i++){
        int pos = calcPos(i, y);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int j = y - 1; j >= 0; j--){
        int pos = calcPos(x, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int j = y + 1; j < 8; j++){
        int pos = calcPos(x, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
}

Queen::Queen(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Queen";
    // this->id = player_id == 1 ? "♕": "♛";
    this->id = "Q";
    std::cout << this->name << " created at " << this->position << "\n"; 
}

void Queen::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves horizontally, vertically and diagonally
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    for(int i = x - 1; i >= 0; i--){
        int pos = calcPos(i, y);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int i = x + 1; i < 8; i++){
        int pos = calcPos(i, y);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int j = y - 1; j >= 0; j--){
        int pos = calcPos(x, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int j = y + 1; j < 8; j++){
        int pos = calcPos(x, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--){
        int pos = calcPos(i, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
    }
    }
    for(int i = x - 1, j = y + 1; i >= 0 && j < 8; i--, j++){
        int pos = calcPos(i, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int i = x + 1, j = y - 1; i < 8 && j >= 0; i++, j--){
        int pos = calcPos(i, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
    for(int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++){
        int pos = calcPos(i, j);
        this->allowedMoves.push_back(pos);
        if(positions[pos]){
            break;
        }
    }
}


Wazir::Wazir(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Wazir";
    this->id = "W";
    std::cout << this->name << " created at " << this->position << "\n"; 
}


void Wazir::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves 1 square horizontally and vertically
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    if(x > 0){
        this->allowedMoves.push_back(calcPos(x - 1, y));
    }
    if(x < 7){
        this->allowedMoves.push_back(calcPos(x + 1, y));
    }
    if(y > 0){
        this->allowedMoves.push_back(calcPos(x, y - 1));
    }
    if(y < 7){
        this->allowedMoves.push_back(calcPos(x, y + 1));
    }
}

Camel::Camel(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Camel";
    this->id = "C";
    std::cout << this->name << " created at " << this->position << "\n"; 
}

void Camel::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves in a long L shape (1-3), can jump over pieces
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    if(x > 2 && y > 0){
        this->allowedMoves.push_back(calcPos(x - 3, y - 1));
    }
    if(x > 2 && y < 7){
        this->allowedMoves.push_back(calcPos(x - 3, y + 1));
    }
    if(x < 5 && y > 0){
        this->allowedMoves.push_back(calcPos(x + 3, y - 1));
    }
    if(x < 5 && y < 7){
        this->allowedMoves.push_back(calcPos(x + 3, y + 1));
    }
    if(x > 0 && y > 2){
        this->allowedMoves.push_back(calcPos(x - 1, y - 3));
    }
    if(x < 7 && y > 2){
        this->allowedMoves.push_back(calcPos(x + 1, y - 3));
    }
    if(x > 0 && y < 5){
        this->allowedMoves.push_back(calcPos(x - 1, y + 3));
    }
    if(x < 7 && y < 5){
        this->allowedMoves.push_back(calcPos(x + 1, y + 3));
    }
}

Grasshopper::Grasshopper(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Grasshopper";
    this->id = "G";
    std::cout << this->name << " created at " << this->position << "\n"; 
}

void Grasshopper::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves horizontally, vertically and diagonally, but can only move if
    // it's possible to jump over another piece, then it lands immediately beyond that piece
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    for(int i = x - 1; i > 0; i--){
        int pos = calcPos(i, y);
        if(positions[pos]){
            this->allowedMoves.push_back(calcPos(i - 1, y));
            break;
        }
    }
    for(int i = x + 1; i < 7; i++){
        int pos = calcPos(i, y);
        if(positions[pos]){
            this->allowedMoves.push_back(calcPos(i + 1, y));
            break;
        }
    }
    for(int j = y - 1; j > 0; j--){
        int pos = calcPos(x, j);
        if(positions[pos]){
            this->allowedMoves.push_back(calcPos(x, j - 1));
            break;
        }
    }
    for(int j = y + 1; j < 7; j++){
        int pos = calcPos(x, j);
        if(positions[pos]){
            this->allowedMoves.push_back(calcPos(x, j + 1));
            break;
        }
    }
    for(int i = x - 1, j = y - 1; i > 0 && j > 0; i--, j--){
        int pos = calcPos(i, j);
        if(positions[pos]){
            this->allowedMoves.push_back(calcPos(i - 1, j - 1));
            break;
        }
    }
    for(int i = x - 1, j = y + 1; i > 0 && j < 7; i--, j++){
        int pos = calcPos(i, j);
        if(positions[pos]){
            this->allowedMoves.push_back(calcPos(i - 1, j + 1));
            break;
        }
    }
    for(int i = x + 1, j = y - 1; i < 7 && j > 0; i++, j--){
        int pos = calcPos(i, j);
        if(positions[pos]){
            this->allowedMoves.push_back(calcPos(i + 1, j - 1));
            break;
        }
    }
    for(int i = x + 1, j = y + 1; i < 7 && j < 7; i++, j++){
        int pos = calcPos(i, j);
        if(positions[pos]){
            this->allowedMoves.push_back(calcPos(i + 1, j + 1));
            break;
        }
    }
}


King::King(int pos, int player_id) : Piece(pos, player_id){
    this->name = "King";
    this->id = "K";
    std::cout << this->name << " created at " << this->position << "\n"; 
}

void King::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves 1 square in any direction, cannot be captured
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    // 
    if(x > 0){
        this->allowedMoves.push_back(calcPos(x - 1, y));
        if(y > 0){
            this->allowedMoves.push_back(calcPos(x - 1, y - 1));
        }
    }
    if(x < 7){
        this->allowedMoves.push_back(calcPos(x + 1, y));
        if(y < 7){
            this->allowedMoves.push_back(calcPos(x + 1, y + 1));
        }
    }
    if(y > 0){
        this->allowedMoves.push_back(calcPos(x, y - 1));
        if(x < 7){
            this->allowedMoves.push_back(calcPos(x + 1, y - 1));
        }
    }
    if(y < 7){
        this->allowedMoves.push_back(calcPos(x, y + 1));
        if(x > 0){
            this->allowedMoves.push_back(calcPos(x - 1, y + 1));
        }
    }
}

void King::updateAllowedMoves(std::vector<Piece *> &positions, std::unordered_set<int> &positionsUnderAttack){
    // Moves 1 square in any direction, cannot be captured
    this->allowedMoves.clear();
    int x = this->position / 8;
    int y = this->position % 8;
    // 
    if(x > 0){
        this->allowedMoves.push_back(calcPos(x - 1, y));
        if(y > 0){
            this->allowedMoves.push_back(calcPos(x - 1, y - 1));
        }
    }
    if(x < 7){
        this->allowedMoves.push_back(calcPos(x + 1, y));
        if(y < 7){
            this->allowedMoves.push_back(calcPos(x + 1, y + 1));
        }
    }
    if(y > 0){
        this->allowedMoves.push_back(calcPos(x, y - 1));
        if(x < 7){
            this->allowedMoves.push_back(calcPos(x + 1, y - 1));
        }
    }
    if(y < 7){
        this->allowedMoves.push_back(calcPos(x, y + 1));
        if(x > 0){
            this->allowedMoves.push_back(calcPos(x - 1, y + 1));
        }
    }
}
