#include "Knight.h"
#include "Board.h"

Knight::Knight(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'N', isWhite, currentPosition, moveDirs)
    {
        availableMoves.reserve(8);
        seenBlockedSquares.reserve(4);
    }


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

void Knight::scanForPin(std::pair<int, int> dir, std::optional<Position> startPosition){
    
}