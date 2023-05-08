#include "headers/game.hpp"
#include "headers/board.hpp"


int algebraicToPosition(std::string move, Board &board){
    if (board.positionMap.count(move) > 0){
        return board.positionMap[move];
    }
    if (move.length() > 1 && board.positionMap.count(move.substr(1, 2)) > 0){
        return board.positionMap[move.substr(1, 2)];
    }
    if (move.length() > 2 && board.positionMap.count(move.substr(2, 3)) > 0){
        return board.positionMap[move.substr(2, 3)];
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


std::string validateMove(std::string move, Player &currentPlayer){
    std::unordered_set<std::string> values;
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    std::string validMove;
    for (const auto &m : currentPlayer.possibleMoves){
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
    else if (files.find(move[0]) != std::string::npos){  // pawn move
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


void updateAllPossibleMoves(Board &board, Player &black, Player &white){
    black.possibleMoves.clear();
    white.possibleMoves.clear();
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
                    white.possibleMoves[p].push_back(newMove);
                }
                else{
                    black.possibleMoves[p].push_back(newMove);
                }
            }
        }
    }
    blackKing->updateAllowedMoves(board.pieces, black.possibleMoves, white.possibleMoves);
    whiteKing->updateAllowedMoves(board.pieces, white.possibleMoves, black.possibleMoves);
}


void updatePlayerScores(Board &board, Player &black, Player &white){
    int score1 = 0, score2 = 0;
    for (auto piece : board.pieces){
        if (piece){
            (piece->getPlayer() == 1 ? score1 : score2) += piece->getVal();
        }
    }
    white.setScore(score1);
    black.setScore(score2);
}


bool simulateMove(std::string move, Board &board, int turn, King *king, Player &black, Player &white){
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    std::string newPos;
    std::string moveCode;
    std::vector<Piece *> threats;
    std::vector<Piece *> snapshot = board.pieces;
    std::vector<int> oldAllowedMoves;
    std::vector<int> threatOldAllowedMoves;
    std::unordered_map<int, std::vector<std::string>> &opponentMoves = (turn == 1 ? black.possibleMoves : white.possibleMoves);
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
    newPosInt = algebraicToPosition(newPos, board);
    moveCode = std::to_string(turn) + ((piecesCodes.find(move[0]) != std::string::npos) ? move.at(0) : 'P');
    if(!board.piecesLocation[moveCode].empty()){
        for (auto pos : board.piecesLocation[moveCode]){
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
                        updateAllPossibleMoves(board, black, white);
                        if (opponentMoves.find(king->getPosition()) != opponentMoves.end()){
                            board.pieces[newPosInt]->setPosition(oldPos);
                            board.pieces[newPosInt]->allowedMoves = oldAllowedMoves;
                            board.pieces = snapshot;
                            updateAllPossibleMoves(board, black, white);
                            return true; // still in check by another piece
                        }
                    }
                }
                board.pieces[newPosInt]->setPosition(oldPos);
                board.pieces[newPosInt]->allowedMoves = oldAllowedMoves;
                board.pieces = snapshot;
                updateAllPossibleMoves(board, black, white);
            }
        }
    }
    return false;
}


void forceKingOutOfCheck(Board &board, int turn, King *king, Player &black, Player &white){
    std::unordered_map<int, std::vector<std::string>> myPossibleMoves = turn == 1 ? white.possibleMoves : black.possibleMoves;
    std::unordered_map<int, std::vector<std::string>> allowedMoves;
    for (const auto &p : myPossibleMoves){
        for (const auto &value : p.second){
            bool stillInCheck = simulateMove(value, board, turn, king, black, white);
            if (!stillInCheck){
                allowedMoves[p.first].push_back(value);
            }
        }
    }
    (turn == 1 ? white.possibleMoves : black.possibleMoves) = allowedMoves;
}


bool playMove(std::string move, Board &board, int turn, King *king, Player &black, Player &white){
    std::string piecesCodes = "QBKNRCWG";
    std::string files = "abcdefgh";
    std::string ranks = "12345678";
    if (piecesCodes.find(move[0]) != std::string::npos){  // moving pieces included in piecesCodes
        std::string moveCode = std::to_string(turn) + move[0];
        std::string newPos = move[1] == 'x' ? move.substr(2, 2) : move.substr(1, 2);
        char oldPos[2];
        int newPosInt = algebraicToPosition(newPos, board);
        for (auto pos : board.piecesLocation[moveCode]){
            if (std::count(board.pieces[pos]->allowedMoves.begin(), board.pieces[pos]->allowedMoves.end(), newPosInt)){
                if (board.pieces[newPosInt] && dynamic_cast<King *>(board.pieces[newPosInt])){
                    std::cout << "It is illegal to take the King!\n";
                    return false;
                }
                std::string pieceName = board.pieces[pos]->name;
                positionToAlgebraic(board.pieces[pos]->getPosition(), oldPos);
                board.pieces[pos]->setPosition(newPosInt);
                board.piecesLocation[moveCode].erase(std::remove(board.piecesLocation[moveCode].begin(), 
                                       board.piecesLocation[moveCode].end(), pos), board.piecesLocation[moveCode].end());
                board.piecesLocation[moveCode].push_back(newPosInt);
                if (board.pieces[newPosInt]){
                    Piece *takenPiece = board.pieces[newPosInt];
                    std::cout << pieceName << " captured " << takenPiece->name << "! ";
                    if (takenPiece->getPlayer() == 1){
                        white.subtractScore(takenPiece->getVal());
                    }
                    else{
                        black.subtractScore(takenPiece->getVal());
                    }
                    delete takenPiece;
                }
                board.pieces[newPosInt] = board.pieces[pos];
                board.pieces[pos] = nullptr;
                std::cout << pieceName << " moved from " << oldPos << " to " << newPos << "!";
                board.updatePiecesLocation();
                updateAllPossibleMoves(board, black, white);
                return true;
            }
        }
    }
    else if (files.find(move[0]) != std::string::npos){  // moving a pawn
        char originalFile = move[0];
        char oldPos[2];
        std::string newPos = move[1] == 'x' ? move.substr(2, 2) : move.substr(0, 2);
        int newPosInt = algebraicToPosition(newPos, board);
        std::string moveCode = std::to_string(turn) + 'P';
        for (auto pos : board.piecesLocation[moveCode]){
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
                    board.piecesLocation[moveCode].erase(std::remove(board.piecesLocation[moveCode].begin(), 
                                           board.piecesLocation[moveCode].end(), pos), board.piecesLocation[moveCode].end());
                    board.piecesLocation[moveCode].push_back(newPosInt);
                    if (board.pieces[newPosInt]){
                        Piece *takenPiece = board.pieces[newPosInt];
                        std::cout << "Pawn captured " << takenPiece->name << "! ";
                        if (takenPiece->getPlayer() == 1){
                            white.subtractScore(takenPiece->getVal());
                        }
                        else{
                            black.subtractScore(takenPiece->getVal());
                        }
                        delete takenPiece;
                    }
                    board.pieces[newPosInt] = board.pieces[pos];
                    board.pieces[pos] = nullptr;
                    std::cout << "Pawn moved from " << oldPos << " to " << newPos << "!";
                    board.updatePiecesLocation();
                    updateAllPossibleMoves(board, black, white);
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


void mateInOneB(Board &board, King *blackKing, King *whiteKing, Player &black, Player &white){
    playMove("d4", board, 1, whiteKing, black, white);
    std::cout << "\n";
    playMove("e5", board, 2, blackKing, black, white);
    std::cout << "\n";
    playMove("g4", board, 1, whiteKing, black, white);
    std::cout << "\n";
    playMove("Qf6", board, 2, blackKing, black, white);
    std::cout << "\n";
    playMove("Wg3", board, 1, whiteKing, black, white);
    std::cout << "\n";
    playMove("e4", board, 2, blackKing, black, white);
    std::cout << "\n";
    playMove("d5", board, 1, whiteKing, black, white);
    std::cout << "\n";
    playMove("e3", board, 2, blackKing, black, white);
    std::cout << "\n";
    playMove("d6", board, 1, whiteKing, black, white);
}


void mateInOneW(Board &board, King *blackKing, King *whiteKing, Player &black, Player &white){
    playMove("e4", board, 1, whiteKing, black, white);
    std::cout << "\n";
    playMove("Gh5", board, 2, blackKing, black, white);
    std::cout << "\n";
    playMove("Qf3", board, 1, whiteKing, black, white);
    std::cout << "\n";
    playMove("Wh7", board, 2, blackKing, black, white);
    std::cout << "\n";
    playMove("e5", board, 1, whiteKing, black, white);
    std::cout << "\n";
    playMove("a5", board, 2, blackKing, black, white);
    std::cout << "\n";
    playMove("e6", board, 1, whiteKing, black, white);
    std::cout << "\n";
    playMove("g5", board, 2, blackKing, black, white);
}


bool isKingMated(Board &board, Player &black, Player &white, King *king, int player){
    std::unordered_map<int, std::vector<std::string>> &myMoves = player == 1 ? white.possibleMoves : black.possibleMoves;
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


bool botPlay(Board &board, King *blackKing, King *whiteKing, Player &black, Player &white){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::map<int, std::vector<std::string>> bestMoves;
    std::string turn = "2";
    std::string piecesCodes = "QBKNRCWG";
    int currStateScore = white.getScore() - black.getScore();
    for (auto p : black.possibleMoves){
        int pos = p.first;
        for (auto move : p.second){
            int newPos = algebraicToPosition(move, board);
            Piece *piece = board.pieces[newPos];
            bestMoves[currStateScore - (piece ? piece->getVal() : 0)].push_back(move);
        }
    }
    auto it = bestMoves.begin();
    int smallestKey = it->first;
    std::vector<std::string> &moves = it->second;
    std::uniform_int_distribution<> dis(0, moves.size() - 1);
    std::string botMove = moves[dis(gen)];
    std::cout << "Bot move -> ";
    std::cout << botMove << " - \n";
    if (playMove(botMove, board, 2, blackKing, black, white)){
        if (whiteKing->getIsInCheck(black.possibleMoves)){
            findKingThreats(whiteKing, board);
            forceKingOutOfCheck(board, 1, whiteKing, black, white);
            if (isKingMated(board, black, white, whiteKing, 1)){
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
    Player white(1, false);
    Player black(2, vsBot);
    Player *currentPlayer = &white;
    King *blackKing = dynamic_cast<King *>(board.pieces[60]);
    King *whiteKing = dynamic_cast<King *>(board.pieces[4]);
    updatePlayerScores(board, black, white);
    updateAllPossibleMoves(board, black, white);
    board.updatePiecesLocation();
    bool checkmate = false;
    while(!checkmate){
        board.printBoard();
        std::cout << "Enter a move - " << currentPlayer->getColor() << " to play: ";    
        std::cin >> move;
        validated = validateMove(move, *currentPlayer);
        if (!validated.empty()){
            if (currentPlayer->getId() == 1){
                if (playMove(validated, board, 1, whiteKing, black, white)){
                    if (!board.gameStarted){
                        board.gameStarted = true;
                    }
                    if (blackKing->getIsInCheck(white.possibleMoves)){
                        findKingThreats(blackKing, board);
                        forceKingOutOfCheck(board, 2, blackKing, black, white);
                        if (isKingMated(board, black, white, blackKing, 2)){
                            checkmate = true;
                            break;
                        }
                    }
                    std::cout << "\n";
                    currentPlayer = &black;
                }
                if (vsBot){
                    board.printBoard();
                    if (botPlay(board, blackKing, whiteKing, black, white)){
                        checkmate = true;
                        break;
                    }
                    currentPlayer = &white;
                }
            }
            else if (playMove(validated, board, 2, blackKing, black, white)){
                if (whiteKing->getIsInCheck(black.possibleMoves)){
                    findKingThreats(whiteKing, board);
                    forceKingOutOfCheck(board, 1, whiteKing, black, white);     
                        
                    if (isKingMated(board, black, white, whiteKing, 1)){
                        checkmate = true;
                        break;
                    }
                }
                std::cout << "\n";
                currentPlayer = &white;
            }
        }
        else if (move[0] == 'm'){
            int pos = algebraicToPosition(move, board);
            if (move[1] == '*'){
                std::cout << "All allowed moves for " << currentPlayer->getColor() << ": ";
                for (const auto &m : currentPlayer->possibleMoves){
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
                if((currentPlayer->getId() == 1 ? whiteKing : blackKing)->getIsInCheck()){
                    std::cout << " +++ CHECK +++ \n";
                }
            }
        }
        else if (move == "M1B" && !vsBot){  // go to a mate in 1 position, black to play and win
            if (board.gameStarted){
                std::cout << "\nYou can only this command before the first move!\n";
                continue;
            }
            mateInOneB(board, blackKing, whiteKing, black, white);
            std::cout << "\nM1 position loaded\n";
            currentPlayer = &black;
        }
        else if (move == "M1W" && !vsBot){  // go to a mate in 1 position, white to play and win
            if (board.gameStarted){
                std::cout << "\nYou can only this command before the first move!\n";
                continue;
            }
            mateInOneW(board, blackKing, whiteKing, black, white);
            std::cout << "\nM1 position loaded\n";
            currentPlayer = &white;
        }
        else{
            std::cout << move << " is an invalid move!\n";
            if((currentPlayer->getId() == 1 ? whiteKing : blackKing)->getIsInCheck()){
                std::cout << " +++ CHECK +++ \n";
            }
        }
        updatePlayerScores(board, black, white);
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
