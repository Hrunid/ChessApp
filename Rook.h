#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece{
    public:
        Rook(int id, bool isWhite, Position currentPosition);
};

#endif