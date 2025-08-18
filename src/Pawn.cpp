#include "Pawn.hpp"
#include "Board.hpp"

Pawn::Pawn(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'P', isWhite, currentPosition,
        isWhite ? std::vector<std::pair<int, int>>{{0, -1}, {1, -1}, {-1, -1}} : std::vector<std::pair<int, int>>{{0, 1}, {1, 1}, {-1, 1}}),
        promotionRow(isWhite ? 0 : 7),
        enPassantRow(isWhite ? 3 : 4)
    {}

void Pawn::findMovesInDirection(std::pair<int, int> direction){
    if(direction.first == 0){
        Position tempPosition = currentPosition;

        tempPosition += direction;
        if(board->isOnBoard(tempPosition)){
            if(board->isSquareEmpty(tempPosition)){
                availableMoves.push_back(tempPosition);
                if(!hasMoved){
                    tempPosition += direction;
                    if(board->isOnBoard(tempPosition) && board->isSquareEmpty(tempPosition)){
                        availableMoves.push_back(tempPosition);
                    }
                    else{
                        seenBlockedSquares.push_back(tempPosition);
                    }
                }
            }
            else{
                seenBlockedSquares.push_back(tempPosition);
            }
        }

    }
    else{
        Position tempPosition = currentPosition;
        tempPosition += direction;
        if(board->isOnBoard(tempPosition)){
            if(board->isSquareEmpty(tempPosition)){
                if(canEnPassant(direction.first)){
                    availableMoves.push_back(tempPosition);
                }
                else{
                    seenBlockedSquares.push_back(tempPosition);
                }
            }
            else if(board->getPieceById(board->getPieceIdAtPosition(tempPosition)).isPieceWhite() == isWhite){
                seenBlockedSquares.push_back(tempPosition);
            }
            else{
                availableMoves.push_back(tempPosition);
            }
        }        
    }
}

void Pawn::scanForPin(Position startPosition, std::pair<int, int> dir){}

bool Pawn::canEnPassant(int dx){

    int yDiff;
    if(isWhite){
        yDiff = -2;
    }
    else{
        yDiff = 2;
    }
    if(currentPosition.y == enPassantRow){
        if(const Move* lastMove = board->getLastMove(); !lastMove){
            char movedPiece = board->getPieceById(board->getPieceIdAtPosition(lastMove->getPositionTo())).getSymbol();
            if(movedPiece == 'P'){
                int movedDist = lastMove->getPositionTo().y - lastMove->getPositionFrom().y;
                if(movedDist == yDiff){
                    if(lastMove->getPositionTo().x - currentPosition.x == dx){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int Pawn::getPromoRow(){
    return promotionRow;
}
