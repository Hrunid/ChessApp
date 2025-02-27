#include "Queen.h"

Queen::Queen(int id, bool isWhite, Position currentPosition, Board& board)
    :   Piece(id, 'Q', isWhite, currentPosition, board, { {1, 0}, {-1 ,0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} })
        {}