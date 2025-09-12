#include "Pawn.h"
#include "Board.h"
 
Pawn::Pawn(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'P', isWhite, currentPosition, isWhite ? moveDirsW : moveDirsB),
        promotionRow(isWhite ? 0 : 7),
        enPassantRow(isWhite ? 3 : 4)
    {
        availableMoves.reserve(4);
        seenBlockedSquares.reserve(4);
    }

void Pawn::findMovesInDirection(std::pair<int, int> direction){
    if(direction.first == 0){
        Position tempPosition = currentPosition;
        tempPosition += direction;

        if(!(board->isOnBoard(tempPosition))) return;
        if(board->isSquareEmpty(tempPosition)){
            availableMoves.emplace_back(tempPosition);
            if(!(hasMoved)){
                tempPosition += direction;
                if(!(board->isOnBoard(tempPosition))) return;
                if(board->isSquareEmpty(tempPosition)) availableMoves.emplace_back(tempPosition);
                else seenBlockedSquares.emplace_back(tempPosition);
            }
            
        }
        else seenBlockedSquares.emplace_back(tempPosition);

    }
    else{
        Position tempPosition = currentPosition;
        tempPosition += direction;
        if(!(board->isOnBoard(tempPosition))) return;

        if(board->isSquareEmpty(tempPosition)){
            seenBlockedSquares.emplace_back(tempPosition);
            if(canEnPassant(direction.first)){
                availableMoves.emplace_back(tempPosition);
                enPassantCol = direction.first;
            }
            else if(currentPosition.y == enPassantRow){
                seenBlockedSquares.emplace_back(Position(tempPosition.x, currentPosition.y));
            }
        }
        else if(board->getPieceById(board->getPieceIdAtPosition(tempPosition)).isPieceWhite() == isWhite){
            seenBlockedSquares.emplace_back(tempPosition);
        }
        else{
            availableMoves.emplace_back(tempPosition);
        }
    }
}

void Pawn::scanForPin(std::pair<int, int> dir, std::optional<Position> startPosition){
    
}

bool Pawn::canEnPassant(int dx){
    if(currentPosition.y != enPassantRow) return false;

    const Move* lastMove = board->getLastMove();
    if(!lastMove) return false;

    if(lastMove->getPieceSymbol() != 'P') return false;

    int yDiff = isWhite ? -2 : 2;
    Position lastTo = lastMove->getPositionTo();
    int movedDist = lastMove->getPositionFrom().y - lastTo.y;
    if((yDiff != movedDist) || (lastTo.x  != (currentPosition.x + dx))) return false;

    return true;
}
/*
bool Pawn::canEnPassant(int dx){
    if(currentPosition.y != enPassantRow)
        return false;   
    int yDiff = isWhite ? -2 : 2;
    if(const Move* lastMove = board->getLastMove(); lastMove){
            
        }
    char movedPiece = board->getPieceById(board->getPieceIdAtPosition(lastMove->getPositionTo())).getSymbol();
            if(movedPiece == 'P'){
                int movedDist = lastMove->getPositionFrom().y - lastMove->getPositionTo().y;
                if(movedDist == yDiff){
                    if(lastMove->getPositionTo().x - currentPosition.x == dx){
                        return true;
                    }
                }
            }
    return false;
}*/

int Pawn::getPromoRow(){
    return promotionRow;
}
