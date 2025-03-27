#include "Bishop.h"

Bishop::Bishop(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'B', isWhite, currentPosition, { {1,1}, {1, -1}, {-1, 1}, {-1, -1} } )
    {}