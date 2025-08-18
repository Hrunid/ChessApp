#ifndef PAWN_H
#define PAWN_H

#include "Piece.hpp"
#include "Move.hpp"

#include <functional>

class Pawn : public Piece{
    private:
        int promotionRow;
        int enPassantRow;
        
    public:
        bool canEnPassant(int dx);
        Pawn(int id, bool isWhite, Position currentPosition);
        void scanForPin(Position startPosition, std::pair<int, int> dir) override;
        void findMovesInDirection(std::pair<int, int> direction) override;
        int getPromoRow();
};

#endif