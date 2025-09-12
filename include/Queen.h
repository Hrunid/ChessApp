#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece{

private:
    inline static constexpr std::array<std::pair<int, int>, 8> moveDirs = {{ {1, 0}, {-1 ,0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} }};

public:
    Queen(int id, bool isWhite, Position currentPosition);

};

#endif