#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece{

    public:
        Bishop(int id, bool isWhite, Position currentPosition);

};

#endif