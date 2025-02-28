#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece{

    King(int id, bool isWhite, Position currentPosition, Board& board);
    void calculateAvailableMoves() override;
    bool isSquareSafe(Position pos);

};

#endif