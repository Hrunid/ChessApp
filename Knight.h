#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece{
    public:
        Knight(int id, bool isWhite, Position currentPosition, Board& board);

        void calculateAvailableMoves() override;
        void scanForPin(Position startPosition, int dx, int dy) override;
};

#endif