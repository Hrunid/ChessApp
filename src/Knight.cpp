#include "Knight.hpp"
#include "Board.hpp"

Knight::Knight(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'N', isWhite, currentPosition, { {1, 2}, {1, -2}, {2, 1}, {2, -1}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1} })
    {}


void Knight::findMovesInDirection(std::pair<int, int> direction){

    Position tempPosition = currentPosition;
    tempPosition += direction;

    if(board->isOnBoard(tempPosition)){
        if(board->isSquareEmpty(tempPosition)){
            availableMoves.push_back(tempPosition);
        }
        else{
            int tempPieceId = board->getPieceIdAtPosition(tempPosition);
            bool tempIsWhite = board->getPieceById(tempPieceId).isPieceWhite();
            if(tempIsWhite == isWhite){
                seenBlockedSquares.push_back(tempPosition);
            }
            else{
                availableMoves.push_back(tempPosition);
            }
        }
    }

}

void Knight::scanForPin(Position startPosition, std::pair<int, int> dir){
    
}