#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.hpp"

class Queen : public Piece{
    public:

        Queen(int id, bool isWhite, Position currentPosition);

};

#endif