#include "headers/game.hpp"
#include "headers/board.hpp"
#include <algorithm>
#include <unordered_set>
#include <cstdlib>
#include <ctime>

void getPiecesLocation(Board &board, std::unordered_map<std::string, std::vector<int>> &piecesLocation){
    piecesLocation.clear();
    for (auto piece : board.pieces){
        if (piece){
            std::string key = std::to_string(piece->getPlayer()) + piece->getId();
            piecesLocation[key].push_back(piece->getPosition());
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

void updateAllPossibleMoves(Board &board,
                            std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
                            std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves){
    blackPossibleMoves.clear();
    whitePossibleMoves.clear();
    King *blackKing, *whiteKing;
    std::string newMove, pieceId, file;
    char alg[2];
    for (auto piece : board.pieces){
        if (piece){
            if (King *king = dynamic_cast<King *>(piece)){
                if (piece->getPlayer() == 1){
                    whiteKing = king;
                }
                else{
                    blackKing = king;
                }
                continue;
            }
            piece->updateAllowedMoves(board.pieces);
            pieceId = piece->getId();
            if (pieceId == "P"){
                pieceId = "";
                file = std::string(1, "abcdefgh"[piece->getPosition() % 8]);
            }
            for (auto p : piece->allowedMoves){
                positionToAlgebraic(p, alg);
                newMove = pieceId + std::string(alg, 2);
                if (pieceId == "" && board.pieces[p] && board.pieces[p]->getPlayer() != piece->getPlayer()){
                    newMove = file + "x" + newMove;
                }
                if (piece->getPlayer() == 1){
                    whitePossibleMoves[p].push_back(newMove);
                }
                else{
                    blackPossibleMoves[p].push_back(newMove);
                }
            }
        }
    }
    blackKing->updateAllowedMoves(board.pieces, blackPossibleMoves, whitePossibleMoves);
    whiteKing->updateAllowedMoves(board.pieces, whitePossibleMoves, blackPossibleMoves);
}


bool simulateMove(std::string move, Board &board,
              std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
              std::unordered_map<std::string, int> &positionMap, int turn, King *king,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves){
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    std::string newPos;
    std::string moveCode;
    std::vector<Piece *> threats;
    std::vector<Piece *> snapshot = board.pieces;
    std::vector<int> oldAllowedMoves;
    std::vector<int> threatOldAllowedMoves;
    std::unordered_map<int, std::vector<std::string>> &opponentMoves = (turn == 1 ? blackPossibleMoves : whitePossibleMoves);
    int newPosInt, originalFile;
    bool isPawnCapture = false;
    if (files.find(move[0]) != std::string::npos && move[1] == 'x'){  // pawn capture
        newPos = move.substr(2, 2);
        isPawnCapture = true;
        originalFile = files.find(move[0]);
    }
    else{
        newPos = (piecesCodes.find(move[0]) != std::string::npos) ? move.substr(1, 2) : move.substr(0, 2);
    }
    newPosInt = algebraicToPosition(newPos, positionMap);
    moveCode = std::to_string(turn) + ((piecesCodes.find(move[0]) != std::string::npos) ? move.at(0) : 'P');
    if(!piecesLocation[moveCode].empty()){
        for (auto pos : piecesLocation[moveCode]){
            if (std::count(board.pieces[pos]->allowedMoves.begin(), board.pieces[pos]->allowedMoves.end(), newPosInt)){
                if (isPawnCapture && ((board.pieces[pos]->getPosition() % 8) != originalFile)){
                    continue;
                }
                int oldPos = board.pieces[pos]->getPosition();
                board.pieces[pos]->setPosition(newPosInt);
                board.pieces[newPosInt] = board.pieces[pos];
                board.pieces[pos] = nullptr;
                oldAllowedMoves = board.pieces[newPosInt]->allowedMoves;
                board.pieces[newPosInt]->updateAllowedMoves(board.pieces);
                for (auto threat : king->getThreats()){
                    if (threat->getPosition() != newPosInt){  // threatening piece has not been taken
                        threatOldAllowedMoves = threat->allowedMoves;
                        threat->updateAllowedMoves(board.pieces);
                        if (std::count(threat->allowedMoves.begin(), threat->allowedMoves.end(), king->getPosition())){
                            threat->allowedMoves = threatOldAllowedMoves;
                            board.pieces[newPosInt]->setPosition(oldPos);
                            board.pieces[newPosInt]->allowedMoves = oldAllowedMoves;
                            board.pieces = snapshot;
                            return true; // still in check
                        }
                        threat->allowedMoves = threatOldAllowedMoves;
                    }
                    else{ // threatening piece was taken, check if king is still in check by another piece
                        updateAllPossibleMoves(board, blackPossibleMoves, whitePossibleMoves);
                        if (opponentMoves.find(king->getPosition()) != opponentMoves.end()){
                            board.pieces[newPosInt]->setPosition(oldPos);
                            board.pieces[newPosInt]->allowedMoves = oldAllowedMoves;
                            board.pieces = snapshot;
                            updateAllPossibleMoves(board, blackPossibleMoves, whitePossibleMoves);
                            return true; // still in check by another piece
                        }
                    }
                }
                board.pieces[newPosInt]->setPosition(oldPos);
                board.pieces[newPosInt]->allowedMoves = oldAllowedMoves;
                board.pieces = snapshot;
                updateAllPossibleMoves(board, blackPossibleMoves, whitePossibleMoves);
            }
        }
    }
    return false;
}

void forceKingOutOfCheck(Board &board,
              std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
              std::unordered_map<std::string, int> &positionMap, int turn, King *king,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves){
    std::unordered_map<int, std::vector<std::string>> &myPossibleMoves = turn == 1 ? whitePossibleMoves : blackPossibleMoves;
    std::unordered_map<int, std::vector<std::string>> legalMoves = turn == 1 ? whitePossibleMoves : blackPossibleMoves;
    for (const auto &p : legalMoves){
        for (const auto &value : p.second){
            bool stillInCheck = simulateMove(value, board, piecesLocation, positionMap, turn, king, 
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

bool playMove(std::string move, Board &board,
              std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
              std::unordered_map<std::string, int> &positionMap, int turn, King *king,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves){
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    if (piecesCodes.find(move[0]) != std::string::npos){  // moving pieces included in piecesCodes
        std::string moveCode = std::to_string(turn) + move[0];
        std::string newPos = move[1] == 'x' ? move.substr(2, 2) : move.substr(1, 2);
        char oldPos[2];
        int newPosInt = algebraicToPosition(newPos, positionMap);
        for (auto pos : piecesLocation[moveCode]){
            if (std::count(board.pieces[pos]->allowedMoves.begin(), board.pieces[pos]->allowedMoves.end(), newPosInt)){
                if (board.pieces[newPosInt] && dynamic_cast<King *>(board.pieces[newPosInt])){
                    std::cout << "It is illegal to take the King!\n";
                    return false;
                }
                std::string pieceName = board.pieces[pos]->name;
                positionToAlgebraic(board.pieces[pos]->getPosition(), oldPos);
                board.pieces[pos]->setPosition(newPosInt);
                piecesLocation[moveCode].erase(std::remove(piecesLocation[moveCode].begin(), 
                                       piecesLocation[moveCode].end(), pos), piecesLocation[moveCode].end());
                piecesLocation[moveCode].push_back(newPosInt);
                if (board.pieces[newPosInt]){
                    Piece *takenPiece = board.pieces[newPosInt];
                    std::cout << pieceName << " captured " << takenPiece->name << "! ";
                    delete takenPiece;
                }
                board.pieces[newPosInt] = board.pieces[pos];
                board.pieces[pos] = nullptr;
                std::cout << pieceName << " moved from " << oldPos << " to " << newPos << "!";
                getPiecesLocation(board, piecesLocation);
                updateAllPossibleMoves(board, blackPossibleMoves, whitePossibleMoves);
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
        for (auto pos : piecesLocation[moveCode]){
            if (std::count(board.pieces[pos]->allowedMoves.begin(), board.pieces[pos]->allowedMoves.end(), newPosInt)){
                if (board.pieces[newPosInt] && dynamic_cast<King *>(board.pieces[newPosInt])){
                    std::cout << "It is illegal to take the King!\n";
                    return false;
                }
                char thisFile = files[pos % 8];
                if(thisFile == originalFile){
                    positionToAlgebraic(board.pieces[pos]->getPosition(), oldPos);
                    board.pieces[pos]->setPosition(newPosInt);
                    if (!board.pieces[pos]->getHasMoved()){
                        board.pieces[pos]->setHasMoved(true);
                    }
                    piecesLocation[moveCode].erase(std::remove(piecesLocation[moveCode].begin(), 
                                           piecesLocation[moveCode].end(), pos), piecesLocation[moveCode].end());
                    piecesLocation[moveCode].push_back(newPosInt);
                    if (board.pieces[newPosInt]){
                        Piece *takenPiece = board.pieces[newPosInt];
                        std::cout << "Pawn captured " << takenPiece->name << "! ";
                        delete takenPiece;
                    }
                    board.pieces[newPosInt] = board.pieces[pos];
                    board.pieces[pos] = nullptr;
                    std::cout << "Pawn moved from " << oldPos << " to " << newPos << "!";
                    getPiecesLocation(board, piecesLocation);
                    updateAllPossibleMoves(board, blackPossibleMoves, whitePossibleMoves);
                    return true;
                }
            }
        }
    }
    std::cout << "This is an invalid move!\n";
    return false;
}

void findKingThreats(King *king, Board &board){
    king->clearThreats();
    for (auto piece : board.pieces){
        if (piece){
            if (piece->getPlayer() == king->getPlayer() || dynamic_cast<King *>(piece)){
                continue;
            }
            for (auto pos : piece->allowedMoves){
                if (pos == king->getPosition()){
                    king->setThreats(piece);
                }
            }
        }
    }
}


void mateInOneB(Board &board,
              std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
              std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves){
    playMove("d4", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e5", board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("g4", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Qf6", board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Wg3", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e4", board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("d5", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e3", board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("d6", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
}

void mateInOneW(Board &board, 
              std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
              std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves){
    playMove("e4", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Gh5", board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Qf3", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("Wh7", board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e5", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("a5", board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("e6", board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves);
    std::cout << "\n";
    playMove("g5", board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves);
}

bool isKingMated(Board &board, std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves, 
              std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
              std::unordered_map<std::string, std::vector<int>> &piecesLocation,
              std::unordered_map<std::string, int> &positionMap,
              King *king, int player){
    std::unordered_map<int, std::vector<std::string>> &myMoves = player == 1 ? whitePossibleMoves : blackPossibleMoves;
    if (myMoves.empty()){   
        king->setIsCheckmated(true);
        king->setId("#");
        std::cout << " ### CHECKMATE ### ";
        board.printBoard();
        return true;
    }
    std::cout << " +++ CHECK +++ ";
    return false;
}


bool botPlay(Board &board,
             std::unordered_map<std::string, std::vector<int>> &piecesLocation, 
             std::unordered_map<std::string, int> &positionMap, King *blackKing, King *whiteKing,
             std::unordered_map<int, std::vector<std::string>> &blackPossibleMoves,
             std::unordered_map<int, std::vector<std::string>> &whitePossibleMoves){
        auto it = blackPossibleMoves.begin();
        std::advance(it, rand() % blackPossibleMoves.size());
        std::vector<std::string> &botMoves = it->second;
        std::string botMove = botMoves[rand() % botMoves.size()];
        std::cout << "Bot move -> ";
        if (playMove(botMove, board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves)){
            if (whiteKing->getIsInCheck(blackPossibleMoves)){
                findKingThreats(whiteKing, board);
                forceKingOutOfCheck(board, piecesLocation, positionMap, 1, 
                                    whiteKing, blackPossibleMoves, whitePossibleMoves);     
                if (isKingMated(board, whitePossibleMoves, blackPossibleMoves, piecesLocation, positionMap, whiteKing, 1)){
                    return true;
                }
            }
            std::cout << "\n";
        }
    return false;
}

void play(Board &board, bool vsBot){
    srand(time(NULL));
    std::string move, validated;
    std::unordered_map<std::string, std::vector<int>> piecesLocation;
    std::unordered_map<std::string, int> positionMap;
    std::unordered_map<int, std::vector<std::string>> whitePossibleMoves, blackPossibleMoves;
    King *blackKing = dynamic_cast<King *>(board.pieces[60]);
    King *whiteKing = dynamic_cast<King *>(board.pieces[4]);
    updateAllPossibleMoves(board, blackPossibleMoves, whitePossibleMoves);
    getPiecesLocation(board, piecesLocation);
    algebraicMap(positionMap);
    bool checkmate = false;
    int currentTurn = 1;
    while(!checkmate){
        board.printBoard();
        std::cout << "Enter a move - " << (currentTurn == 1 ? "White" : "Black") << " to play: ";    
        std::cin >> move;
        validated = validateMove(move, positionMap, currentTurn, (currentTurn == 1 ? whitePossibleMoves : blackPossibleMoves));
        if (!validated.empty()){
            if (currentTurn == 1){
                if (playMove(validated, board, piecesLocation, positionMap, 1, whiteKing, blackPossibleMoves, whitePossibleMoves)){
                    if (blackKing->getIsInCheck(whitePossibleMoves)){
                        findKingThreats(blackKing, board);
                        forceKingOutOfCheck(board, piecesLocation, positionMap, 2, 
                                            blackKing, blackPossibleMoves, whitePossibleMoves);
                        if (isKingMated(board, whitePossibleMoves, blackPossibleMoves, piecesLocation, positionMap, blackKing, 2)){
                            checkmate = true;
                            break;
                        }
                    }
                    std::cout << "\n";
                    currentTurn = 2;
                }
                if (vsBot){
                    board.printBoard();
                    if (botPlay(board, piecesLocation, positionMap, blackKing, whiteKing, blackPossibleMoves, whitePossibleMoves)){
                        checkmate = true;
                        break;
                    }
                    currentTurn = 1;
                }
            }
            else if (playMove(validated, board, piecesLocation, positionMap, 2, blackKing, blackPossibleMoves, whitePossibleMoves)){
                if (whiteKing->getIsInCheck(blackPossibleMoves)){
                    findKingThreats(whiteKing, board);
                    forceKingOutOfCheck(board, piecesLocation, positionMap, 1, 
                                        whiteKing, blackPossibleMoves, whitePossibleMoves);     
                        
                    if (isKingMated(board, whitePossibleMoves, blackPossibleMoves, piecesLocation, positionMap, whiteKing, 1)){
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
            else if (pos >= 0 && board.pieces[pos]){
                if (!board.pieces[pos]->allowedMoves.empty()){
                    std::cout << "Allowed squares for this " << board.pieces[pos]->name << " to move: ";
                    for (auto a : board.pieces[pos]->allowedMoves){
                        char alg[2];
                        positionToAlgebraic(a, alg);
                        std::cout << alg << " ";
                    }
                    std::cout << "\n";
                }
                else{
                    std::cout << "No moves allowed for this " << board.pieces[pos]->name << "!\n";
                }
            }
            else{
                std::cout << move << " is an invalid move! ";
                if((currentTurn == 1 ? whiteKing : blackKing)->getIsInCheck()){
                    std::cout << " +++ CHECK +++ \n";
                }
            }
        }
        else if (move == "M1B" && !vsBot){  // go to a mate in 1 position, black to play and win
            mateInOneB(board, piecesLocation, positionMap, blackKing, whiteKing, blackPossibleMoves, whitePossibleMoves);
            std::cout << "\nM1 position loaded\n";
            currentTurn = 2;
        }
        else if (move == "M1W" && !vsBot){  // go to a mate in 1 position, white to play and win
            mateInOneW(board, piecesLocation, positionMap, blackKing, whiteKing, blackPossibleMoves, whitePossibleMoves);
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
        if (!vsBot){
            std::cout << "Player " << (whiteKing->getIsCheckmated() ? 2 : 1) << " won!\n\n";
        }
        else if (whiteKing->getIsCheckmated()){
            std::cout << "The bot won!\n\n";
        }
        else{
            std::cout << "You won!\n\n";
        }
    }
}
