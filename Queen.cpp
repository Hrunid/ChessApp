#include "Queen.h"

Queen::Queen(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'Q', isWhite, currentPosition, { {1, 0}, {-1 ,0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} })
        {}