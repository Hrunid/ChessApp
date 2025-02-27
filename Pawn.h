#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include "Game.h"
#include "Move.h"

#include <utility>

class Pawn : public Piece{
    private:
        Game& game;
        Move& getLastMove();
        void enPassant();
        int promotionRow;
        int enPassantRow;
    public:
        Pawn(int id, bool isWhite, Position currentPosition, Board& board, Game& game);
        void calculateAvailableMoves() override;
        void scanForPin(Position startPosition, int dx, int dy) override;
};

#endif