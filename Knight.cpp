#include "Knight.h"

Knight::Knight(int id, bool isWhite, Position currentPosition, Board& board)
    :   Piece(id, 'N', isWhite, currentPosition, board, { {1, 2}, {1, -2}, {2, 1}, {2, -1}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1} })
    {}

void Knight::calculateAvailableMoves(){

    availableMoves.clear();

    for(auto [dx, dy] : moveDirections){
        int x = currentPosition.x;
        int y = currentPosition.y;

        x += dx;
        y += dy;

        if(x >= 0 && x < 8 && y >= 0 && y < 8){
            Position tempPosition(x, y);
            
            if(board.isSquareEmpty(tempPosition)){
                availableMoves.push_back(tempPosition);
            }
            else{
                int tempPieceId = board.getPieceIdAtPosition(tempPosition);
                bool tempIsWhite = board.getPieceById(tempPieceId).isPieceWhite();
                if(isWhite != tempIsWhite){
                    availableMoves.push_back(tempPosition);
                }

                
            }
        }

    }
}

void Knight::scanForPin(Position startPosition, int dx, int dy){}