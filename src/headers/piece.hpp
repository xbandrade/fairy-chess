#ifndef PIECE_HPP
#define PIECE_HPP
#include <string>
#include <vector>
#include <unordered_map>

class Piece{
    private:
        int player;
        int position;
        int piece_value;
        std::string id;
    protected:
        void setVal(int val);
    public:
        std::string name;
        std::vector<int> allowedMoves;
        Piece(int pos, int player_id);
        int getPosition();
        void setPosition(int pos);
        virtual void updateAllowedMoves(std::vector<Piece *> &pieces) = 0;
        virtual bool getHasMoved();
        virtual void setHasMoved(bool moved);
        std::string getId();
        void setId(std::string new_id);
        int getVal();
        int getPlayer();
        virtual ~Piece(){}
};

class Pawn : public Piece{
    private:
        bool hasMoved = false;
    public:
        Pawn(int pos, int player_id);
        bool getHasMoved();
        void setHasMoved(bool moved);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
};

class Knight : public Piece{
    public:
        Knight(int pos, int player_id);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
};

class Bishop : public Piece{
    public:
        Bishop(int pos, int player_id);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
};

class Rook : public Piece{
    public:
        Rook(int pos, int player_id);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
};

class Queen : public Piece{
    public:
        Queen(int pos, int player_id);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
};

class Wazir : public Piece{
    public:
        Wazir(int pos, int player_id);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
};

class Camel : public Piece{
    public:
        Camel(int pos, int player_id);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
};

class Grasshopper : public Piece{
    public:
        Grasshopper(int pos, int player_id);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
};

class King : public Piece{
    private:
        bool isInCheck;
        bool isCheckmated;
        std::vector<Piece *> threats;
    public:
        King(int pos, int player_id);
        void updateAllowedMoves(std::vector<Piece *> &pieces);
        void updateAllowedMoves(std::vector<Piece *> &pieces, 
                                std::unordered_map<int, std::vector<std::string>> &thisPossibleMoves,
                                std::unordered_map<int, std::vector<std::string>> &opponentPossibleMoves);
        bool getIsInCheck();
        bool getIsInCheck(std::unordered_map<int, std::vector<std::string>> &opponentPossibleMoves);
        void setThreats(Piece *piece);
        std::vector<Piece *> getThreats();
        void clearThreats();
        bool getIsCheckmated();
        void setIsCheckmated(bool checkmate);
};


#endif