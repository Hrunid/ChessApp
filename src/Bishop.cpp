#include "Bishop.h"

Bishop::Bishop(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'B', isWhite, currentPosition, moveDirs)
    {
        availableMoves.reserve(8);
        seenBlockedSquares.reserve(4);
    }