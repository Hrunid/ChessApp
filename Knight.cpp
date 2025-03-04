#include "Knight.h"
#include "Board.h"

Knight::Knight(int id, bool isWhite, Position currentPosition, Board& board)
    :   Piece(id, 'N', isWhite, currentPosition, board, { {1, 2}, {1, -2}, {2, 1}, {2, -1}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1} })
    {}


void Knight::findMovesInDirection(std::pair<int, int> direction){
    int x = currentPosition.x;
    int y = currentPosition.y;

    int dx = direction.first;
    int dy = direction.second;

    x += dx;
    y += dy;

    Position tempPosition(x, y);

    if(board.isOnBoard(tempPosition)){
        if(board.isSquareEmpty(tempPosition)){
            availableMoves.push_back(tempPosition);
        }
        else{
            int tempPieceId = board.getPieceIdAtPosition(tempPosition);
            bool tempIsWhite = board.getPieceById(tempPieceId).isPieceWhite();
            if(tempIsWhite == isWhite){
                seenBlockedSquares.push_back(tempPosition);
            }
            else{
                availableMoves.push_back(tempPosition);
            }
        }
    }

}

void Knight::scanForPin(Position startPosition, int dx, int dy){}