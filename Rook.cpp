#include "Rook.h"

Rook::Rook(int id, bool isWhite, Position currentPosition, Board& board)
    :   Piece(id, 'R', isWhite, currentPosition, board, { {-1, 0}, {1, 0}, {0, 1}, {0, -1} })
    {}