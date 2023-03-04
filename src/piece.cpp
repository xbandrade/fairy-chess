#include "headers/piece.hpp"
#include <iostream>

int calcPos(int x, int y){
    return x * 8 + y;
}

Piece::Piece(int pos, int player_id){
    position = pos;
    player = player_id;
}

int Piece::getPosition(){
    return position;
}

void Piece::setPosition(int pos){
    position = pos;
}

bool Piece::getHasMoved(){
    return false;
}

void Piece::setHasMoved(bool moved){
}

std::string Piece::getId(){
    return id;
}

void Piece::setId(std::string new_id){
    id = new_id;
}

int Piece::getPlayer(){
    return player;
}

Pawn::Pawn(int pos, int player_id) : Piece(pos, player_id){
    name = "Pawn";
    setId("P");
    // std::cout << name << " created at " << position << "\n";
}

bool Pawn::getHasMoved(){
    return hasMoved;
}

void Pawn::setHasMoved(bool moved)
{
    hasMoved = moved;
}

void Pawn::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves 1 (or 2) forward, captures diagonally
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    if (getPlayer() == 1){
        int pos = calcPos(x + 1, y);
        if (x < 7 && !pieces[pos]){
            allowedMoves.push_back(pos);
        }
        pos = calcPos(x + 2, y);
        if (!hasMoved && !allowedMoves.empty() && !pieces[calcPos(x + 1, y)] && !pieces[pos]){
            allowedMoves.push_back(pos);
        }
        pos = calcPos(x + 1, y - 1);
        if (x < 7 && y > 0 && pieces[pos] && (pieces[pos]->getPlayer() != getPlayer())){
            allowedMoves.push_back(pos);
        }
        pos = calcPos(x + 1, y + 1);
        if (x < 7 && y < 7 && pieces[pos] && (pieces[pos]->getPlayer() != getPlayer())){
            allowedMoves.push_back(pos);
        }
    }
    else{
        int pos = calcPos(x - 1, y);
        if (x > 0 && !pieces[pos]){
            allowedMoves.push_back(pos);
        }
        pos = calcPos(x - 2, y);
        if (!hasMoved && !allowedMoves.empty() && !pieces[calcPos(x - 1, y)] && !pieces[pos]){
            allowedMoves.push_back(pos);
        }
        pos = calcPos(x - 1, y - 1);
        if (x > 0 && y > 0 && pieces[pos] && (pieces[pos]->getPlayer() != getPlayer())){
            allowedMoves.push_back(pos);
        }
        pos = calcPos(x - 1, y + 1);
        if (x > 0 && y < 7 && pieces[pos] && (pieces[pos]->getPlayer() != getPlayer())){
            allowedMoves.push_back(pos);
        }
    }
}

Knight::Knight(int pos, int player_id) : Piece(pos, player_id){
    name = "Knight";
    setId("N");
    // std::cout << name << " created at " << position << "\n";  
}

void Knight::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves in an L shape (1-2), can jump over pieces
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    int pos = calcPos(x - 2, y - 1);
    if (x > 1 && y > 0 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x - 2, y + 1);
    if (x > 1 && y < 7 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 2, y - 1);
    if (x < 6 && y > 0 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 2, y + 1);
    if (x < 6 && y < 7 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x - 1, y - 2);
    if (x > 0 && y > 1 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 1, y - 2);
    if (x < 7 && y > 1 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x - 1, y + 2);
    if (x > 0 && y < 6 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 1, y + 2);
    if (x < 7 && y < 6 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
}

Bishop::Bishop(int pos, int player_id) : Piece(pos, player_id){
    name = "Bishop";
    setId("B");
    // std::cout << name << " created at " << position << "\n"; 
}

bool pieceMovement(int i, int j, std::vector<Piece *> &pieces, Piece *piece){
    int pos = calcPos(i, j);
    if (pieces[pos]){
        if (pieces[pos]->getPlayer() != piece->getPlayer()){
            piece->allowedMoves.push_back(pos);
        }
        return false;
    }
    piece->allowedMoves.push_back(pos);
    return true;
}

void Bishop::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves diagonally
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--){   
        if (!pieceMovement(i, j, pieces, this)){
            break;
        }
    }
    for (int i = x - 1, j = y + 1; i >= 0 && j < 8; i--, j++){
        if (!pieceMovement(i, j, pieces, this)){
            break;
        }
    }
    for (int i = x + 1, j = y - 1; i < 8 && j >= 0; i++, j--){
        if (!pieceMovement(i, j, pieces, this)){
            break;
        }
    }
    for (int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++){
        if (!pieceMovement(i, j, pieces, this)){
            break;
        }
    }
}

Rook::Rook(int pos, int player_id) : Piece(pos, player_id){
    name = "Rook";
    setId("R");
    // std::cout << name << " created at " << position << "\n"; 
}

void Rook::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves horizontally/vertically
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    for (int i = x - 1; i >= 0; i--){
        if (!pieceMovement(i, y, pieces, this)){
            break;
        }
    }
    for (int i = x + 1; i < 8; i++){
        if (!pieceMovement(i, y, pieces, this)){
            break;
        }
    }
    for (int j = y - 1; j >= 0; j--){
        if (!pieceMovement(x, j, pieces, this)){
            break;
        }
    }
    for (int j = y + 1; j < 8; j++){
        if (!pieceMovement(x, j, pieces, this)){
            break;
        }
    }
}

Queen::Queen(int pos, int player_id) : Piece(pos, player_id){
    name = "Queen";
    // id = player_id == 1 ? "♕": "♛";
    setId("Q");
    // std::cout << name << " created at " << position << "\n"; 
}

void Queen::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves horizontally/vertically/diagonally
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    for (int i = x - 1; i >= 0; i--){
        if (!pieceMovement(i, y, pieces, this)){
            break;
        }
    }
    for (int i = x + 1; i < 8; i++){
        if (!pieceMovement(i, y, pieces, this)){
            break;
        }
    }
    for (int j = y - 1; j >= 0; j--){
        if (!pieceMovement(x, j, pieces, this)){
            break;
        }
    }
    for (int j = y + 1; j < 8; j++){
        if (!pieceMovement(x, j, pieces, this)){
            break;
        }
    }
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--){
        if (!pieceMovement(i, j, pieces, this)){
            break;
        }
    }
    for (int i = x - 1, j = y + 1; i >= 0 && j < 8; i--, j++){
        if (!pieceMovement(i, j, pieces, this)){
            break;
        }
    }
    for (int i = x + 1, j = y - 1; i < 8 && j >= 0; i++, j--){
        if (!pieceMovement(i, j, pieces, this)){
            break;
        }
    }
    for (int i = x + 1, j = y + 1; i < 8 && j < 8; i++, j++){
        if (!pieceMovement(i, j, pieces, this)){
            break;
        }
    }
}


Wazir::Wazir(int pos, int player_id) : Piece(pos, player_id){
    name = "Wazir";
    setId("W");
    // std::cout << name << " created at " << position << "\n"; 
}


void Wazir::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves 1 square horizontally/vertically
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    int pos = calcPos(x - 1, y);
    if (x > 0 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x + 1, y);
    if (x < 7 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x, y - 1);
    if (y > 0 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
    pos = calcPos(x, y + 1);
    if (y < 7 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(pos);
    }
}

Camel::Camel(int pos, int player_id) : Piece(pos, player_id){
    name = "Camel";
    setId("C");
    // std::cout << name << " created at " << position << "\n"; 
}

void Camel::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves in a long L shape (1-3), can jump over pieces
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    int pos = calcPos(x - 3, y - 1);
    if (x > 2 && y > 0 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(calcPos(x - 3, y - 1));
    }
    pos = calcPos(x - 3, y + 1);
    if (x > 2 && y < 7 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(calcPos(x - 3, y + 1));
    }
    pos = calcPos(x + 3, y - 1);
    if (x < 5 && y > 0 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(calcPos(x + 3, y - 1));
    }
    pos = calcPos(x + 3, y + 1);
    if (x < 5 && y < 7 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(calcPos(x + 3, y + 1));
    }
    pos = calcPos(x - 1, y - 3);
    if (x > 0 && y > 2 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(calcPos(x - 1, y - 3));
    }
    pos = calcPos(x + 1, y - 3);
    if (x < 7 && y > 2 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(calcPos(x + 1, y - 3));
    }
    pos = calcPos(x - 1, y + 3);
    if (x > 0 && y < 5 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(calcPos(x - 1, y + 3));
    }
    pos = calcPos(x + 1, y + 3);
    if (x < 7 && y < 5 && (!pieces[pos] || (pieces[pos]->getPlayer() != getPlayer()))){
        allowedMoves.push_back(calcPos(x + 1, y + 3));
    }
}

Grasshopper::Grasshopper(int pos, int player_id) : Piece(pos, player_id){
    name = "Grasshopper";
    setId("G");
    // std::cout << name << " created at " << position << "\n"; 
}

bool grasshopperMovement(int i, int j, int nextI, int nextJ, std::vector<Piece *> &pieces, Piece *piece){
        int pos = calcPos(i, j);
        if (pieces[pos]){
            int landPos = calcPos(nextI, nextJ);
            if (!pieces[landPos] || (pieces[landPos]->getPlayer() != piece->getPlayer())){
                piece->allowedMoves.push_back(landPos);
            }
            return true;
        }
        return false;
}


void Grasshopper::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves horizontally/vertically/diagonally, but can only move if
    // it's possible to jump over another piece, then it lands immediately beyond that piece
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    for (int i = x - 1; i > 0; i--){
        if (grasshopperMovement(i, y, i - 1, y, pieces, this)){
            break;
        }
    }
    for (int i = x + 1; i < 7; i++){
        if (grasshopperMovement(i, y, i + 1, y, pieces, this)){
            break;
        }
    }
    for (int j = y - 1; j > 0; j--){
        if (grasshopperMovement(x, j, x, j - 1, pieces, this)){
            break;
        }
    }
    for (int j = y + 1; j < 7; j++){
        if (grasshopperMovement(x, j, x, j + 1, pieces, this)){
            break;
        }
    }
    for (int i = x - 1, j = y - 1; i > 0 && j > 0; i--, j--){
        if (grasshopperMovement(i, j, i - 1, j - 1, pieces, this)){
            break;
        }
    }
    for (int i = x - 1, j = y + 1; i > 0 && j < 7; i--, j++){
        if (grasshopperMovement(i, j, i - 1, j + 1,pieces, this)){
            break;
        }
    }
    for (int i = x + 1, j = y - 1; i < 7 && j > 0; i++, j--){
        if (grasshopperMovement(i, j, i + 1, j - 1, pieces, this)){
            break;
        }
    }
    for (int i = x + 1, j = y + 1; i < 7 && j < 7; i++, j++){
        if (grasshopperMovement(i, j, i + 1, j + 1, pieces, this)){
            break;
        }
    }
}


King::King(int pos, int player_id) : Piece(pos, player_id){
    name = "King";
    setId("K");
    isInCheck = false;
    // std::cout << name << " created at " << position << "\n"; 
}

void King::updateAllowedMoves(std::vector<Piece *> &pieces){
    // Moves 1 square in any direction, cannot be captured
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    int pos = calcPos(x - 1, y);
    int diagPos = calcPos(x - 1, y - 1);
    if (x > 0 && (!pieces[pos] || pieces[pos]->getPlayer() != getPlayer())){
        allowedMoves.push_back(pos);
        if (y > 0 && (!pieces[diagPos] || pieces[diagPos]->getPlayer() != getPlayer())){
            allowedMoves.push_back(diagPos);
        }
    }
    pos = calcPos(x + 1, y);
    diagPos = calcPos(x + 1, y + 1);
    if (x < 7 && (!pieces[pos] || pieces[pos]->getPlayer() != getPlayer())){
        allowedMoves.push_back(pos);
        if (y < 7 && (!pieces[diagPos] || pieces[diagPos]->getPlayer() != getPlayer())){
            allowedMoves.push_back(diagPos);
        }
    }
    pos = calcPos(x, y - 1);
    diagPos = calcPos(x + 1, y - 1);
    if (y > 0 && (!pieces[pos] || pieces[pos]->getPlayer() != getPlayer())){
        allowedMoves.push_back(pos);
        if (x < 7 && (!pieces[diagPos] || pieces[diagPos]->getPlayer() != getPlayer())){
            allowedMoves.push_back(diagPos);
        }
    }
    pos = calcPos(x, y + 1);
    diagPos = calcPos(x - 1, y + 1);
    if (y < 7 && (!pieces[pos] || pieces[pos]->getPlayer() != getPlayer())){
        allowedMoves.push_back(pos);
        if (x > 0 && (!pieces[diagPos] || pieces[diagPos]->getPlayer() != getPlayer())){
            allowedMoves.push_back(diagPos);
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

void King::updateAllowedMoves(std::vector<Piece *> &pieces, 
                        std::unordered_map<int, std::vector<std::string>> &thisPossibleMoves,
                        std::unordered_map<int, std::vector<std::string>> &opponentPossibleMoves){
    // Moves 1 square in any direction, cannot be captured
    allowedMoves.clear();
    int x = getPosition() / 8;
    int y = getPosition() % 8;
    char alg[2];
    int pos = calcPos(x - 1, y);
    int diagPos = calcPos(x - 1, y - 1);
    if (x > 0 && (!pieces[pos] || pieces[pos]->getPlayer() != getPlayer())){
        checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, pos, alg);
        if (y > 0 && (!pieces[diagPos] || pieces[diagPos]->getPlayer() != getPlayer())){
            checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, diagPos, alg);
        }
    }
    pos = calcPos(x + 1, y);
    diagPos = calcPos(x + 1, y + 1);
    if (x < 7 && (!pieces[pos] || pieces[pos]->getPlayer() != getPlayer())){
        checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, pos, alg);
        if (y < 7 && (!pieces[diagPos] || pieces[diagPos]->getPlayer() != getPlayer())){
            checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, diagPos, alg);
        }
    }
    pos = calcPos(x, y - 1);
    diagPos = calcPos(x + 1, y - 1);
    if (y > 0 && (!pieces[pos] || pieces[pos]->getPlayer() != getPlayer())){
        checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, pos, alg);
        if (x < 7 && (!pieces[diagPos] || pieces[diagPos]->getPlayer() != getPlayer())){
            checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, diagPos, alg);
        }
    }
    pos = calcPos(x, y + 1);
    diagPos = calcPos(x - 1, y + 1);
    if (y < 7 && (!pieces[pos] || pieces[pos]->getPlayer() != getPlayer())){
        checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, pos, alg);
        if (x > 0 && (!pieces[diagPos] || pieces[diagPos]->getPlayer() != getPlayer())){
            checkPositionUnderAttack(thisPossibleMoves, opponentPossibleMoves, this, diagPos, alg);
        }

    }
}

bool King::getIsInCheck(){
    return isInCheck;
}

bool King::getIsInCheck(std::unordered_map<int, std::vector<std::string>> &opponentPossibleMoves){
    if (opponentPossibleMoves.find(getPosition()) != opponentPossibleMoves.end()){
        isInCheck = true;
    }
    else{
        isInCheck = false;
    }
    return isInCheck;
}

void King::setThreats(Piece *piece){
    threats.push_back(piece);
}

std::vector<Piece *> King::getThreats(){
    return threats;
}

void King::clearThreats(){
    threats.clear();
}

bool King::getIsCheckmated()
{
    return isCheckmated;
}

void King::setIsCheckmated(bool checkmate)
{
    isCheckmated = checkmate;
}






