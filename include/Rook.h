#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece{
private:
    inline static constexpr std::array<std::pair<int, int>, 4> moveDirs = {{ {-1, 0}, {1, 0}, {0, 1}, {0, -1} }};

public:
    Rook(int id, bool isWhite, Position currentPosition);
};

#endif