#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece{
    private:
        bool isSquareSafe(Position pos);
        void findCastleMove(int dx);
    public:
        King(int id, bool isWhite, Position currentPosition, Board& board);
        void calculateAvailableMoves() override;
        

};

#endif