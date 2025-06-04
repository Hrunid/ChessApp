#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.hpp"

class Bishop : public Piece{

    public:
        Bishop(int id, bool isWhite, Position currentPosition);

};

#endif