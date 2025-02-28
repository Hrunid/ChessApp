#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include "Game.h"
#include "Move.h"

class Pawn : public Piece{
    private:
        Game& game;
        Move& getLastMove();
        void enPassant();
        int promotionRow;
        int enPassantRow;
        bool canEnPassant(int dx);
    public:
        Pawn(int id, bool isWhite, Position currentPosition, Board& board, Game& game);
        void calculateAvailableMoves() override;
        void scanForPin(Position startPosition, int dx, int dy) override;
};

#endif