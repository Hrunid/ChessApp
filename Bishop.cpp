#include "Bishop.h"

Bishop::Bishop(int id, bool isWhite, Position currentPosition, Board& board)
    :   Piece(id, 'B', isWhite, currentPosition, board, { {1,1}, {1, -1}, {-1, 1}, {-1, -1} } )
    {}