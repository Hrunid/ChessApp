#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece{
private:
    inline static constexpr std::array<std::pair<int, int>, 4> moveDirs = { {{1,1}, {1,-1}, {-1,1}, {-1,-1}} };

public:
    Bishop(int id, bool isWhite, Position currentPosition);

};

#endif