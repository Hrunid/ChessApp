#include "Queen.h"

Queen::Queen(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'Q', isWhite, currentPosition, moveDirs)
        {
            availableMoves.reserve(16);
            seenBlockedSquares.reserve(8);
        }