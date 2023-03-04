#include "headers/pieces.hpp"
#include <iostream>

int calcPos(int x, int y){
    return x * 8 + y;
}

Piece::Piece(int pos, int player_id){
    this->position = pos;
    this->player = player_id;
}

int Piece::getPosition(){
    return this->position;
}

void Piece::setPosition(int pos){
    this->position = pos;
}

bool Piece::getHasMoved(){
    return false;
}

void Piece::setHasMoved(bool moved){
}

std::string Piece::getId(){
    return this->id;
}

void Piece::setId(std::string new_id){
    this->id = new_id;
}

int Piece::getPlayer(){
    return this->player;
}

Pawn::Pawn(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Pawn";
    this->setId("P");
    // std::cout << this->name << " created at " << this->position << "\n";
}

bool Pawn::getHasMoved(){
    return this->hasMoved;
}

void Pawn::setHasMoved(bool moved)
{
    this->hasMoved = moved;
}

void Pawn::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves 1 (or 2) forward, captures diagonally
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    if (this->getPlayer() == 1){
        int pos = calcPos(x + 1, y);
        if (x < 7 && !positions[pos]){
            this->allowedMoves.push_back(pos);
        }
        pos = calcPos(x + 2, y);
        if (!this->hasMoved && !this->allowedMoves.empty() && !positions[calcPos(x + 1, y)] && !positions[pos]){
            this->allowedMoves.push_back(pos);
        }
        pos = calcPos(x + 1, y - 1);
        if (x < 7 && y > 0 && positions[pos] && (positions[pos]->getPlayer() != this->getPlayer())){
            this->allowedMoves.push_back(pos);
        }
        pos = calcPos(x + 1, y + 1);
        if (x < 7 && y < 7 && positions[pos] && (positions[pos]->getPlayer() != this->getPlayer())){
            this->allowedMoves.push_back(pos);
        }
    }
    else{
        int pos = calcPos(x - 1, y);
        if (x > 0 && !positions[pos]){
            this->allowedMoves.push_back(pos);
        }
        pos = calcPos(x - 2, y);
        if (!this->hasMoved && !this->allowedMoves.empty() && !positions[calcPos(x - 1, y)] && !positions[pos]){
            this->allowedMoves.push_back(pos);
        }
        pos = calcPos(x - 1, y - 1);
        if (x > 0 && y > 0 && positions[pos] && (positions[pos]->getPlayer() != this->getPlayer())){
            this->allowedMoves.push_back(pos);
        }
        pos = calcPos(x - 1, y + 1);
        if (x > 0 && y < 7 && positions[pos] && (positions[pos]->getPlayer() != this->getPlayer())){
            this->allowedMoves.push_back(pos);
        }
    }
}

Knight::Knight(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Knight";
    this->setId("N");
    // std::cout << this->name << " created at " << this->position << "\n";  
}

void Knight::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves in an L shape (1-2), can jump over pieces
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    int pos = calcPos(x - 2, y - 1);
    if (x > 1 && y > 0 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x - 2, y + 1);
    if (x > 1 && y < 7 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 2, y - 1);
    if (x < 6 && y > 0 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 2, y + 1);
    if (x < 6 && y < 7 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x - 1, y - 2);
    if (x > 0 && y > 1 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 1, y - 2);
    if (x < 7 && y > 1 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x - 1, y + 2);
    if (x > 0 && y < 6 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 1, y + 2);
    if (x < 7 && y < 6 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
}

Bishop::Bishop(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Bishop";
    this->setId("B");
    // std::cout << this->name << " created at " << this->position << "\n"; 
}

bool pieceMovement(int i, int j, std::vector<Piece *> &positions, Piece *piece){
    int pos = calcPos(i, j);
    if (positions[pos]){
        if (positions[pos]->getPlayer() != piece->getPlayer()){
            piece->allowedMoves.push_back(pos);
        }
        return false;
    }
    piece->allowedMoves.push_back(pos);
    return true;
}

void Bishop::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves diagonally
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--){   
        if (!pieceMovement(i, j, positions, this)){
            break;
        }
    }
    for (int i = x - 1, j = y + 1; i >= 0 && j < 8; i--, j++){
        if (!pieceMovement(i, j, positions, this)){
            break;
        }
    }
    for (int i = x + 1, j = y - 1; i < 8 && j >= 0; i++, j--){
        if (!pieceMovement(i, j, positions, this)){
            break;
        }
    }
    for (int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++){
        if (!pieceMovement(i, j, positions, this)){
            break;
        }
    }
}

Rook::Rook(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Rook";
    this->setId("R");
    // std::cout << this->name << " created at " << this->position << "\n"; 
}

void Rook::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves horizontally/vertically
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    for (int i = x - 1; i >= 0; i--){
        if (!pieceMovement(i, y, positions, this)){
            break;
        }
    }
    for (int i = x + 1; i < 8; i++){
        if (!pieceMovement(i, y, positions, this)){
            break;
        }
    }
    for (int j = y - 1; j >= 0; j--){
        if (!pieceMovement(x, j, positions, this)){
            break;
        }
    }
    for (int j = y + 1; j < 8; j++){
        if (!pieceMovement(x, j, positions, this)){
            break;
        }
    }
}

Queen::Queen(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Queen";
    // this->id = player_id == 1 ? "♕": "♛";
    this->setId("Q");
    // std::cout << this->name << " created at " << this->position << "\n"; 
}

void Queen::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves horizontally/vertically/diagonally
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    for (int i = x - 1; i >= 0; i--){
        if (!pieceMovement(i, y, positions, this)){
            break;
        }
    }
    for (int i = x + 1; i < 8; i++){
        if (!pieceMovement(i, y, positions, this)){
            break;
        }
    }
    for (int j = y - 1; j >= 0; j--){
        if (!pieceMovement(x, j, positions, this)){
            break;
        }
    }
    for (int j = y + 1; j < 8; j++){
        if (!pieceMovement(x, j, positions, this)){
            break;
        }
    }
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--){
        if (!pieceMovement(i, j, positions, this)){
            break;
        }
    }
    for (int i = x - 1, j = y + 1; i >= 0 && j < 8; i--, j++){
        if (!pieceMovement(i, j, positions, this)){
            break;
        }
    }
    for (int i = x + 1, j = y - 1; i < 8 && j >= 0; i++, j--){
        if (!pieceMovement(i, j, positions, this)){
            break;
        }
    }
    for (int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++){
        if (!pieceMovement(i, j, positions, this)){
            break;
        }
    }
}


Wazir::Wazir(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Wazir";
    this->setId("W");
    // std::cout << this->name << " created at " << this->position << "\n"; 
}


void Wazir::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves 1 square horizontally/vertically
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    int pos = calcPos(x - 1, y);
    if (x > 0 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 1, y);
    if (x < 7 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x, y - 1);
    if (y > 0 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
    pos = calcPos(x, y + 1);
    if (y < 7 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(pos);
    }
}

Camel::Camel(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Camel";
    this->setId("C");
    // std::cout << this->name << " created at " << this->position << "\n"; 
}

void Camel::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves in a long L shape (1-3), can jump over pieces
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    int pos = calcPos(x - 3, y - 1);
    if (x > 2 && y > 0 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(calcPos(x - 3, y - 1));
    }
    pos = calcPos(x - 3, y + 1);
    if (x > 2 && y < 7 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(calcPos(x - 3, y + 1));
    }
    pos = calcPos(x + 3, y - 1);
    if (x < 5 && y > 0 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(calcPos(x + 3, y - 1));
    }
    pos = calcPos(x + 3, y + 1);
    if (x < 5 && y < 7 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(calcPos(x + 3, y + 1));
    }
    pos = calcPos(x - 1, y - 3);
    if (x > 0 && y > 2 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(calcPos(x - 1, y - 3));
    }
    pos = calcPos(x + 1, y - 3);
    if (x < 7 && y > 2 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(calcPos(x + 1, y - 3));
    }
    pos = calcPos(x - 1, y + 3);
    if (x > 0 && y < 5 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(calcPos(x - 1, y + 3));
    }
    pos = calcPos(x + 1, y + 3);
    if (x < 7 && y < 5 && (!positions[pos] || (positions[pos]->getPlayer() != this->getPlayer()))){
        this->allowedMoves.push_back(calcPos(x + 1, y + 3));
    }
}

Grasshopper::Grasshopper(int pos, int player_id) : Piece(pos, player_id){
    this->name = "Grasshopper";
    this->setId("G");
    // std::cout << this->name << " created at " << this->position << "\n"; 
}

bool grasshopperMovement(int i, int j, int nextI, int nextJ, std::vector<Piece *> &positions, Piece *piece){
        int pos = calcPos(i, j);
        if (positions[pos]){
            int landPos = calcPos(nextI, nextJ);
            if (!positions[landPos] || (positions[landPos]->getPlayer() != piece->getPlayer())){
                piece->allowedMoves.push_back(landPos);
            }
            return true;
        }
        return false;
}


void Grasshopper::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves horizontally/vertically/diagonally, but can only move if
    // it's possible to jump over another piece, then it lands immediately beyond that piece
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    for (int i = x - 1; i > 0; i--){
        if (grasshopperMovement(i, y, i - 1, y, positions, this)){
            break;
        }
    }
    for (int i = x + 1; i < 7; i++){
        if (grasshopperMovement(i, y, i + 1, y, positions, this)){
            break;
        }
    }
    for (int j = y - 1; j > 0; j--){
        if (grasshopperMovement(x, j, x, j - 1, positions, this)){
            break;
        }
    }
    for (int j = y + 1; j < 7; j++){
        if (grasshopperMovement(x, j, x, j + 1, positions, this)){
            break;
        }
    }
    for (int i = x - 1, j = y - 1; i > 0 && j > 0; i--, j--){
        if (grasshopperMovement(i, j, i - 1, j - 1, positions, this)){
            break;
        }
    }
    for (int i = x - 1, j = y + 1; i > 0 && j < 7; i--, j++){
        if (grasshopperMovement(i, j, i - 1, j + 1,positions, this)){
            break;
        }
    }
    for (int i = x + 1, j = y - 1; i < 7 && j > 0; i++, j--){
        if (grasshopperMovement(i, j, i + 1, j - 1, positions, this)){
            break;
        }
    }
    for (int i = x + 1, j = y + 1; i < 7 && j < 7; i++, j++){
        if (grasshopperMovement(i, j, i + 1, j + 1, positions, this)){
            break;
        }
    }
}


King::King(int pos, int player_id) : Piece(pos, player_id){
    this->name = "King";
    this->setId("K");
    this->isInCheck = false;
    // std::cout << this->name << " created at " << this->position << "\n"; 
}

void King::updateAllowedMoves(std::vector<Piece *> &positions){
    // Moves 1 square in any direction, cannot be captured
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    int pos = calcPos(x - 1, y);
    int diagPos = calcPos(x - 1, y - 1);
    if (x > 0 && (!positions[pos] || positions[pos]->getPlayer() != this->getPlayer())){
        this->allowedMoves.push_back(pos);
        if (y > 0 && (!positions[diagPos] || positions[diagPos]->getPlayer() != this->getPlayer())){
            this->allowedMoves.push_back(diagPos);
        }
    }
    pos = calcPos(x + 1, y);
    diagPos = calcPos(x + 1, y + 1);
    if (x < 7 && (!positions[pos] || positions[pos]->getPlayer() != this->getPlayer())){
        this->allowedMoves.push_back(pos);
        if (y < 7 && (!positions[diagPos] || positions[diagPos]->getPlayer() != this->getPlayer())){
            this->allowedMoves.push_back(diagPos);
        }
    }
    pos = calcPos(x, y - 1);
    diagPos = calcPos(x + 1, y - 1);
    if (y > 0 && (!positions[pos] || positions[pos]->getPlayer() != this->getPlayer())){
        this->allowedMoves.push_back(pos);
        if (x < 7 && (!positions[diagPos] || positions[diagPos]->getPlayer() != this->getPlayer())){
            this->allowedMoves.push_back(diagPos);
        }
    }
    pos = calcPos(x, y + 1);
    diagPos = calcPos(x - 1, y + 1);
    if (y < 7 && (!positions[pos] || positions[pos]->getPlayer() != this->getPlayer())){
        this->allowedMoves.push_back(pos);
        if (x > 0 && (!positions[diagPos] || positions[diagPos]->getPlayer() != this->getPlayer())){
            this->allowedMoves.push_back(diagPos);
        }
    }
}

void checkPositionUnderAttack(std::unordered_map<int, std::vector<std::string>> &thisPossibleMoves,
                    std::unordered_map<int, std::vector<std::string>> &opponentPossibleMoves,
                    King *king, int pos, char alg[2]){
    if (opponentPossibleMoves.find(pos) == opponentPossibleMoves.end()){
        king->allowedMoves.push_back(pos);
        std::string files = "abcdefgh";
        std::string ranks = "12345678";
        int x = pos / 8;
        int y = pos % 8;
        alg[0] = files[y];
        alg[1] = ranks[x];
        thisPossibleMoves[pos].push_back("K" + std::string(alg, 2));
    }
}   

void King::updateAllowedMoves(std::vector<Piece *> &positions, 
                        std::unordered_map<int, std::vector<std::string>> &thisPossibleMoves,
                        std::unordered_map<int, std::vector<std::string>> &opponentPossibleMoves){
    // Moves 1 square in any direction, cannot be captured
    this->allowedMoves.clear();
    int x = this->getPosition() / 8;
    int y = this->getPosition() % 8;
    char alg[2];
    int pos = calcPos(x - 1, y);
    int diagPos = calcPos(x - 1, y - 1);
    if (x > 0 && (!positions[pos] || positions[pos]->getPlayer() != this->getPlayer())){
        checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, pos, alg);
        if (y > 0 && (!positions[diagPos] || positions[diagPos]->getPlayer() != this->getPlayer())){
            checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, diagPos, alg);
        }
    }
    pos = calcPos(x + 1, y);
    diagPos = calcPos(x + 1, y + 1);
    if (x < 7 && (!positions[pos] || positions[pos]->getPlayer() != this->getPlayer())){
        checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, pos, alg);
        if (y < 7 && (!positions[diagPos] || positions[diagPos]->getPlayer() != this->getPlayer())){
            checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, diagPos, alg);
        }
    }
    pos = calcPos(x, y - 1);
    diagPos = calcPos(x + 1, y - 1);
    if (y > 0 && (!positions[pos] || positions[pos]->getPlayer() != this->getPlayer())){
        checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, pos, alg);
        if (x < 7 && (!positions[diagPos] || positions[diagPos]->getPlayer() != this->getPlayer())){
            checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, diagPos, alg);
        }
    }
    pos = calcPos(x, y + 1);
    diagPos = calcPos(x - 1, y + 1);
    if (y < 7 && (!positions[pos] || positions[pos]->getPlayer() != this->getPlayer())){
        checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, pos, alg);
        if (x > 0 && (!positions[diagPos] || positions[diagPos]->getPlayer() != this->getPlayer())){
            checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, diagPos, alg);
        }

    }
}

bool King::getIsInCheck(){
    return this->isInCheck;
}

bool King::getIsInCheck(std::unordered_map<int, std::vector<std::string>> &opponentPossibleMoves){
    if (opponentPossibleMoves.find(this->getPosition()) != opponentPossibleMoves.end()){
        this->isInCheck = true;
    }
    else{
        this->isInCheck = false;
    }
    return this->isInCheck;
}

void King::setThreats(Piece *piece){
    this->threats.push_back(piece);
}

std::vector<Piece *> King::getThreats(){
    return this->threats;
}

void King::clearThreats(){
    this->threats.clear();
}

bool King::getIsCheckmated()
{
    return this->isCheckmated;
}

void King::setIsCheckmated(bool checkmate)
{
    this->isCheckmated = checkmate;
}






