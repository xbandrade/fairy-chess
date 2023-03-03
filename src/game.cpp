#include "headers/game.hpp"
#include "headers/board.hpp"
#include <algorithm>
#include <unordered_set>

void piecesLocation(std::unordered_map<std::string, std::vector<int>> &pieces, std::vector<Piece *> &positions){
    pieces.clear();
    for (auto piece : positions){
        if (piece){
            std::string key = std::to_string(piece->getPlayer()) + piece->getId();
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


std::string validateMove(std::string move, std::unordered_map<std::string, int> &piecesHashMap,
                         int turn, std::unordered_map<int, std::vector<std::string>> possibleMoves){
    std::unordered_set<std::string> values;
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    std::string validMove;
    for (const auto &m : possibleMoves){
        for (const auto &p : m.second){
            values.insert(p);
        }
    }
    if (piecesCodes.find(move[0]) != std::string::npos){
        if (files.find(move[1]) != std::string::npos){
            if (ranks.find(move[2]) != std::string::npos){
                validMove = move.substr(0, 3);
                if (values.find(validMove) != values.end()){
                    return validMove;
                }
            }
        }
        else if (move[1] == 'x'){
            if (files.find(move[2]) != std::string::npos){
                if (ranks.find(move[3]) != std::string::npos){
                    validMove = move.substr(0, 1) + move.substr(2, 2);
                    if (values.find(validMove) != values.end()){
                        return move.substr(0, 4);
                    }
                }
            }
        }
    }
    else if (files.find(move[0]) != std::string::npos){  // Pawn move
        if (ranks.find(move[1]) != std::string::npos){
            validMove = move.substr(0, 2);
            if (values.find(validMove) != values.end()){
                return validMove;
            }
        }
        else if (move[1] == 'x'){
            if (files.find(move[2]) != std::string::npos){
                int index = files.find(move[0]);
                if ((index > 0 && (move[2] == files[index - 1])) || (index < 7 && (move[2] == files[index + 1]))){
                    if (ranks.find(move[3]) != std::string::npos){
                        validMove = move.substr(0, 4);
                        if (values.find(validMove) != values.end() || values.find(move.substr(2, 2)) != values.end()){
                            return validMove;
                        }
                    }
                }
            }
        }
    }
    return "";
}

void updateAllPossibleMoves(std::vector<Piece *> &positions, 
                            std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                            std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves){
    blackPossibleMoves.clear();
    whitePossibleMoves.clear();
    King *blackKing, *whiteKing;
    char alg[2];
    for (auto piece : positions){
        if (piece){
            std::string pieceId = piece->getId() == "P" ? "" : piece->getId();
            if (King *king = dynamic_cast<King *>(piece)){
                if (piece->getPlayer() == 1){
                    whiteKing = king;
                }
                else{
                    blackKing = king;
                }
                continue;
            }
            piece->updateAllowedMoves(positions);
            for (auto p : piece->allowedMoves){
                positionToAlgebraic(p, alg);
                if (piece->getPlayer() == 1){
                    whitePossibleMoves[p].push_back(pieceId + std::string(alg, 2));
                }
                else{
                    blackPossibleMoves[p].push_back(pieceId + std::string(alg, 2));
                }
            }
        }
    }
    blackKing->updateAllowedMoves(positions, blackPossibleMoves, whitePossibleMoves);
    whiteKing->updateAllowedMoves(positions, whitePossibleMoves, blackPossibleMoves);
}


bool simulateMove(std::string move, std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, int turn, King *king,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
              ){
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    std::string newPos;
    std::string moveCode;
    std::vector<Piece *> threats;
    std::vector<Piece *> snapshot = positions;
    std::vector<int> oldAllowedMoves;
    std::vector<int> threatOldAllowedMoves;
    int newPosInt;
    newPos = (piecesCodes.find(move[0]) != std::string::npos) ? move.substr(1, 2) : move.substr(0, 2);
    newPosInt = algebraicToPosition(newPos, positionMap);
    moveCode = std::to_string(turn) + ((piecesCodes.find(move[0]) != std::string::npos) ? move.at(0) : 'P');
    if(!pieces[moveCode].empty()){
        for (auto pos : pieces[moveCode]){
            if (std::count(positions[pos]->allowedMoves.begin(), positions[pos]->allowedMoves.end(), newPosInt)){
                int oldPos = positions[pos]->position;
                positions[pos]->position = newPosInt;
                positions[newPosInt] = positions[pos];
                positions[pos] = nullptr;
                oldAllowedMoves = positions[newPosInt]->allowedMoves;
                positions[newPosInt]->updateAllowedMoves(positions);
                for (auto threat : king->getThreats()){
                    if (threat->position != newPosInt){  // threatening piece has not been taken
                        threatOldAllowedMoves = threat->allowedMoves;
                        threat->updateAllowedMoves(positions);
                        if (std::count(threat->allowedMoves.begin(), threat->allowedMoves.end(), king->position)){
                            threat->allowedMoves = threatOldAllowedMoves;
                            positions[newPosInt]->position = oldPos;
                            positions[newPosInt]->allowedMoves = oldAllowedMoves;
                            positions = snapshot;
                            return true; // still in check
                        }
                        threat->allowedMoves = threatOldAllowedMoves;
                    }
                    else{ // threatening piece was taken, check if king is still in check by another piece
                        updateAllPossibleMoves(positions, blackPossibleMoves, whitePossibleMoves);
                        std::unordered_map<int, std::vector<std::string>> &opponentMoves = (turn == 1 ? 
                                                                                            blackPossibleMoves : 
                                                                                            whitePossibleMoves);
                        if (opponentMoves.find(king->position) != opponentMoves.end()){
                            positions[newPosInt]->position = oldPos;
                            positions[newPosInt]->allowedMoves = oldAllowedMoves;
                            positions = snapshot;
                            updateAllPossibleMoves(positions, blackPossibleMoves, whitePossibleMoves);
                            return true; // still in check by another piece
                        }
                    }
                }
                positions[newPosInt]->position = oldPos;
                positions[newPosInt]->allowedMoves = oldAllowedMoves;
                positions = snapshot;
                updateAllPossibleMoves(positions, blackPossibleMoves, whitePossibleMoves);
            }
        }
    }
    return false;
}

void forceKingOutOfCheck(std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, int turn, King *king,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
              ){
    std::unordered_map<int, std::vector<std::string>> &myPossibleMoves = turn == 1 ? whitePossibleMoves : blackPossibleMoves;
    std::unordered_map<int, std::vector<std::string>> legalMoves = turn == 1 ? whitePossibleMoves : blackPossibleMoves;
    for (const auto &p : legalMoves){
        for (const auto &value : p.second){
            bool stillInCheck = simulateMove(value, positions, pieces, positionMap, turn, king, 
                                             blackPossibleMoves, whitePossibleMoves);
            if (stillInCheck){
                auto newEnd = std::remove(myPossibleMoves[p.first].begin(), myPossibleMoves[p.first].end(), value);
                myPossibleMoves[p.first].erase(newEnd, myPossibleMoves[p.first].end());
            }
        }
    }
    
    for (auto it = myPossibleMoves.begin(); it != myPossibleMoves.end();){
        if (it->second.empty()){
            it = myPossibleMoves.erase(it);
        }
        else{
            it++;
        }
    }
}

bool playMove(std::string move, std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, int turn, King *king,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
              ){
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
                std::cout << pieceName << " moved from " << oldPos << " to " << newPos << "!";
                piecesLocation(pieces, positions);
                updateAllPossibleMoves(positions, blackPossibleMoves, whitePossibleMoves);
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
                    std::cout << "Pawn moved from " << oldPos << " to " << newPos << "!";
                    piecesLocation(pieces, positions);
                    updateAllPossibleMoves(positions, blackPossibleMoves, whitePossibleMoves);
                    return true;
                }
            }
        }
    }
    std::cout << "This is an invalid move!\n";
    return false;
}

void findKingThreats(King *king, std::vector<Piece *> &positions){
    king->clearThreats();
    for (auto piece : positions){
        if (piece){
            if (piece->getPlayer() == king->getPlayer() || dynamic_cast<King *>(piece)){
                continue;
            }
            for (auto pos : piece->allowedMoves){
                if (pos == king->position){
                    king->setThreats(piece);
                }
            }
        }
    }
}


void mateInOneB(std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
              ){
    playMove("d4", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e5", positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("g4", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Qf6", positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Wg3", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e4", positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("d5", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e3", positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("d6", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
}

void mateInOneW(std::vector<Piece *> &positions, 
              std::unordered_map<std::string, std::vector<int>> &pieces, 
              std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves
              ){
    playMove("e4", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Gh5", positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Qf3", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Wh7", positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e5", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("a5", positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e6", positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("g5", positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
}

bool isKingMated(std::vector<Piece *> &positions, std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves, 
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<std::string, std::vector<int>> &pieces,
              std::unordered_map<std::string, int> &positionMap,
              King *king, int player
              ){
    std::unordered_map<int, std::vector<std::string>> &myMoves = player == 1 ? whitePossibleMoves : blackPossibleMoves;
    if (myMoves.empty()){   
        king->setIsCheckmated(true);
        king->setId("#");
        std::cout << " ### CHECKMATE ### ";
        printBoard(positions);
        return true;
    }
    std::cout << " +++ CHECK +++ ";
    return false;
}


void play(std::vector<Piece *> &positions){
    std::string move, validated;
    std::unordered_map<std::string, std::vector<int>> pieces;
    std::unordered_map<std::string, int> positionMap;
    std::unordered_map<int, std::vector<std::string>> whitePossibleMoves, blackPossibleMoves;
    createDefaultPieces(1, positions);
    createDefaultPieces(2, positions); 
    King *blackKing = dynamic_cast<King *>(positions[60]);
    King *whiteKing = dynamic_cast<King *>(positions[4]);
    updateAllPossibleMoves(positions, blackPossibleMoves, whitePossibleMoves);
    piecesLocation(pieces, positions);
    algebraicMap(positionMap);
    bool checkmate = false;
    int currentTurn = 1;
    while(!checkmate){
        printBoard(positions);
        std::cout << "Enter a move - " << (currentTurn == 1 ? "White" : "Black") << " to play: ";    
        std::cin >> move;
        validated = validateMove(move, positionMap, currentTurn, (currentTurn == 1 ? whitePossibleMoves : blackPossibleMoves));
        if (!validated.empty()){
            if (currentTurn == 1){
                if (playMove(validated, positions, pieces, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves)){
                    if (blackKing->getIsInCheck(whitePossibleMoves)){
                        findKingThreats(blackKing, positions);
                        forceKingOutOfCheck(positions, pieces, positionMap, 2, 
                                            blackKing, blackPossibleMoves, whitePossibleMoves);
                        if (isKingMated(positions, whitePossibleMoves, blackPossibleMoves, pieces, positionMap, blackKing, 2)){
                            checkmate = true;
                            break;
                        }
                    }
                    std::cout << "\n";
                    currentTurn = 2;
                }
            }
            else if (playMove(validated, positions, pieces, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves)){
                if (whiteKing->getIsInCheck(blackPossibleMoves)){
                    findKingThreats(whiteKing, positions);
                    forceKingOutOfCheck(positions, pieces, positionMap, 1, 
                                        whiteKing, blackPossibleMoves, whitePossibleMoves);     
                        
                    if (isKingMated(positions, whitePossibleMoves, blackPossibleMoves, pieces, positionMap, whiteKing, 1)){
                        checkmate = true;
                        break;
                    }
                }
                std::cout << "\n";
                currentTurn = 1;
            }
        }
        else if (move[0] == 'm'){
            int pos = algebraicToPosition(move, positionMap);
            if (move[1] == '*'){
                std::cout << "All allowed moves for " << (currentTurn == 1 ? "White" : "Black") << ": ";
                for (const auto &m : (currentTurn == 1 ? whitePossibleMoves : blackPossibleMoves)){
                    for (const auto &value : m.second){
                        std::cout << value << " ";
                    }
                }
                std::cout << "\n";
            }
            else if (pos >= 0 && positions[pos]){
                if (!positions[pos]->allowedMoves.empty()){
                    std::cout << "Allowed squares for this " << positions[pos]->name << " to move: ";
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
                std::cout << move << " is an invalid move! ";
                if((currentTurn == 1 ? whiteKing : blackKing)->getIsInCheck()){
                    std::cout << " +++ CHECK +++ \n";
                }
            }
        }
        else if (move == "M1B"){  // go to a mate in 1 position, black to play and win
            mateInOneB(positions, pieces, positionMap, blackKing, whiteKing, blackPossibleMoves, whitePossibleMoves);
            std::cout << "\nM1 position loaded\n";
            currentTurn = 2;
        }
        else if (move == "M1W"){  // go to a mate in 1 position, white to play and win
            mateInOneW(positions, pieces, positionMap, blackKing, whiteKing, blackPossibleMoves, whitePossibleMoves);
            std::cout << "\nM1 position loaded\n";
            currentTurn = 1;
        }
        else{
            std::cout << move << " is an invalid move!\n";
            if((currentTurn == 1 ? whiteKing : blackKing)->getIsInCheck()){
                std::cout << " +++ CHECK +++ \n";
            }
        }
    }
    if (checkmate){
        std::cout << "\n# CHECKMATE # - ";
        std::cout << "Player " << (whiteKing->getIsCheckmated() ? 2 : 1) << " won!\n\n";
    }
}
