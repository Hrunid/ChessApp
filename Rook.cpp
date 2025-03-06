#include "Rook.h"

Rook::Rook(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'R', isWhite, currentPosition, { {-1, 0}, {1, 0}, {0, 1}, {0, -1} })
    {}