#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include "Move.h"

#include <functional>

class Pawn : public Piece{
    private:
        std::function<Move()> getLastMoveFunction;

        Move getLastMove();
        void enPassant();
        int promotionRow;
        int enPassantRow;
        bool canEnPassant(int dx);
    public:
        Pawn(int id, bool isWhite, Position currentPosition);
        void scanForPin(Position startPosition, int dx, int dy) override;
        void findMovesInDirection(std::pair<int, int> direction) override;
        void setGetLastMoveFunction(std::function<Move()> func);
        int getPromoRow();
};

#endif