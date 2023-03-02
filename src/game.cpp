#include "headers/game.hpp"
#include "headers/board.hpp"
#include <algorithm>

void piecesLocation(std::unordered_map<std::string, std::vector<int>> &pieces, std::vector<Piece *> &positions){
    for (auto piece : positions){
        if (piece){
            std::string key = std::to_string(piece->player) + piece->id;
            pieces[key].push_back(piece->position);
        }
    }
}

void algebraicMap(std::unordered_map<std::string, int> &positionMap){
    std::string files = "abcdefgh";
    int k = 0;
    for (int i = 1; i <= 8; i++){
        for (int j = 0; j < 8; j++, k++){
            positionMap[files[j] + std::to_string(i)] = k;
        }
    }
}

int algebraicToPosition(std::string move, std::unordered_map<std::string, int> &positionMap){
    if (positionMap.count(move) > 0){
        return positionMap[move];
    }
    if (positionMap.count(move.substr(1, 2)) > 0){
        return positionMap[move.substr(1, 2)];
    }
    return -1;
}

void positionToAlgebraic(int pos, char alg[2]){
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    int x = pos / 8;
    int y = pos % 8;
    alg[0] = files[y];
    alg[1] = ranks[x];
}


std::string validateMove(std::string move, std::unordered_map<std::string, int> &piecesHashMap){
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    if (piecesCodes.find(move[0]) != std::string::npos){
        if (files.find(move[1]) != std::string::npos){
            if (ranks.find(move[2]) != std::string::npos){
                return move.substr(0, 3);
            }
        }
        else if (move[1] == 'x'){
            if (files.find(move[2]) != std::string::npos){
                if (ranks.find(move[3]) != std::string::npos){
                    return move.substr(0, 4);
                }
            }
        }
    }
    else if (files.find(move[0]) != std::string::npos){  // Pawn move
        if (ranks.find(move[1]) != std::string::npos){
            return move.substr(0, 2);
        }
        else if (move[1] == 'x'){
            if (files.find(move[2]) != std::string::npos){
                int index = files.find(move[0]);
                if ((index > 0 && (move[2] == files[index - 1])) || (index < 7 && (move[2] == files[index + 1]))){
                    if (ranks.find(move[3]) != std::string::npos){
                        return move.substr(0, 4);
                    }
                }
            }
        }
    }
    return "";
}

void updateAllPossibleMoves(std::vector<Piece *> &positions){
    for (auto piece : positions){
        if (piece){
            piece->updateAllowedMoves(positions);
        }
    }
}

bool playMove(std::string move, std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, int turn){
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    if (piecesCodes.find(move[0]) != std::string::npos){  // moving pieces included in piecesCodes
        std::string moveCode = std::to_string(turn) + move[0];
        std::string newPos = move[1] == 'x' ? move.substr(2, 2) : move.substr(1, 2);
        char oldPos[2];
        int newPosInt = algebraicToPosition(newPos, positionMap);
        for (auto pos : pieces[moveCode]){
            if (std::count(positions[pos]->allowedMoves.begin(), positions[pos]->allowedMoves.end(), newPosInt)){
                std::string pieceName = positions[pos]->name;
                positionToAlgebraic(positions[pos]->position, oldPos);
                positions[pos]->position = newPosInt;
                pieces[moveCode].erase(std::remove(pieces[moveCode].begin(), 
                                       pieces[moveCode].end(), pos), pieces[moveCode].end());
                pieces[moveCode].push_back(newPosInt);
                if (positions[newPosInt]){
                    Piece *takenPiece = positions[newPosInt];
                    std::cout << pieceName << " captured " << takenPiece->name << "! ";
                    delete takenPiece;
                }
                positions[newPosInt] = positions[pos];
                positions[pos] = nullptr;
                std::cout << pieceName << " moved from " << oldPos << " to " << newPos << "!\n";
                return true;
            }
        }
    }
    else if (files.find(move[0]) != std::string::npos){  // moving a pawn
        char originalFile = move[0];
        char oldPos[2];
        std::string newPos = move[1] == 'x' ? move.substr(2, 2) : move.substr(0, 2);
        int newPosInt = algebraicToPosition(newPos, positionMap);
        std::string moveCode = std::to_string(turn) + 'P';
        for (auto pos : pieces[moveCode]){
            if (std::count(positions[pos]->allowedMoves.begin(), positions[pos]->allowedMoves.end(), newPosInt)){
                char thisFile = files[pos % 8];
                if(thisFile == originalFile){
                    positionToAlgebraic(positions[pos]->position, oldPos);
                    positions[pos]->position = newPosInt;
                    if (!positions[pos]->getHasMoved()){
                        positions[pos]->setHasMoved(true);
                    }
                    pieces[moveCode].erase(std::remove(pieces[moveCode].begin(), 
                                           pieces[moveCode].end(), pos), pieces[moveCode].end());
                    pieces[moveCode].push_back(newPosInt);
                    if (positions[newPosInt]){
                        Piece *takenPiece = positions[newPosInt];
                        std::cout << "Pawn captured " << takenPiece->name << "! ";
                        delete takenPiece;
                    }
                    positions[newPosInt] = positions[pos];
                    positions[pos] = nullptr;
                    std::cout << "Pawn moved from " << oldPos << " to " << newPos << "!\n";
                    return true;
                }
            }
        }
    }
    std::cout << "This is an invalid move!\n";
    return false;
}

void play(std::vector<Piece *> &positions)
{
    std::string move, validated;
    std::unordered_map<std::string, std::vector<int>> pieces;
    std::unordered_map<std::string, int> positionMap;
    createDefaultPieces(1, positions);
    createDefaultPieces(2, positions); 
    piecesLocation(pieces, positions);
    algebraicMap(positionMap);
    bool checkmate = false;
    bool draw = false;
    int currentTurn = 1;
    while(!checkmate && !draw){
        printBoard(positions);
        updateAllPossibleMoves(positions);
        std::cout << "Enter a move - " << (currentTurn == 1 ? "White" : "Black") << " to play: ";
        std::cin >> move;
        validated = validateMove(move, positionMap);
        if (!validated.empty()){
            if (currentTurn == 1){
                if (playMove(validated, positions, pieces, positionMap, 1)){
                    currentTurn = 2;
                }
            }
            else if (playMove(validated, positions, pieces, positionMap, 2)){
                currentTurn = 1;
            }
            piecesLocation(pieces, positions);
        } 
        else if (move[0] == 'm'){
            int pos = algebraicToPosition(move, positionMap);
            if (pos >= 0 && positions[pos]){
                if (!positions[pos]->allowedMoves.empty()){
                    std::cout << "Allowed moves for this " << positions[pos]->name << ": ";
                    for (auto a : positions[pos]->allowedMoves){
                        char alg[2];
                        positionToAlgebraic(a, alg);
                        std::cout << alg << " ";
                    }
                    std::cout << "\n";
                }
                else{
                    std::cout << "No moves allowed for this " << positions[pos]->name << "!\n";
                }
            }
            else{
                std::cout << move << " is an invalid move!\n";
            }
        }
        else{
            std::cout << move << " is an invalid move!\n";
        }
    }
}
