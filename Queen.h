#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece{
    public:

        Queen(int id, bool isWhite, Position currentPosition);

};

#endif