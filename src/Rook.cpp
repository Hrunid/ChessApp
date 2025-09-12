#include "Rook.h"

Rook::Rook(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'R', isWhite, currentPosition, moveDirs)
    {
        availableMoves.reserve(12);
        seenBlockedSquares.reserve(4);
    }