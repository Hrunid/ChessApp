#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'P', isWhite, currentPosition,
        isWhite ? std::vector<std::pair<int, int>>{{0, 1}, {1, 1}, {-1, 1}} : std::vector<std::pair<int, int>>{{0, -1}, {1, -1}, {-1, -1}}),
        promotionRow(isWhite ? 7 : 0),
        enPassantRow(isWhite ? 5 : 3)
    {}

void Pawn::setGetLastMoveFunction(std::function<Move()> func){
    getLastMoveFunction = func;
}

Move Pawn::getLastMove(){
    return getLastMoveFunction();
}

void Pawn::findMovesInDirection(std::pair<int, int> direction){
    if(direction.first == 0){
        int x = currentPosition.x;
        int y = currentPosition.y;
        int dy = direction.second;

        Position tempPosition(x, y + dy);
        if(board->isOnBoard(tempPosition)){
            if(board->isSquareEmpty(tempPosition)){
                availableMoves.push_back(tempPosition);
                if(!hasMoved){
                    tempPosition = Position(tempPosition.x, tempPosition.y + dy);
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
            int x = currentPosition.x;
            int y = currentPosition.y;
            int dx = direction.first;
            int dy = direction.second;
            x += dx;
            y += dy;
    
            Position tempPosition(x, y);
    
            if(board->isOnBoard(tempPosition)){
                if(board->isSquareEmpty(tempPosition)){
                    if(canEnPassant(dx)){
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

void Pawn::scanForPin(Position startPosition, int dx, int dy){}

bool Pawn::canEnPassant(int dx){

    int yDiff;
    if(isWhite){
        yDiff = -2;
    }
    else{
        yDiff = 2;
    }
    if(currentPosition.y == enPassantRow){
        const Move& lastMove = getLastMove();
        char movedPiece = board->getPieceById(board->getPieceIdAtPosition(lastMove.getPositionTo())).getSymbol();
        if(movedPiece == 'P'){
            int movedDist = lastMove.getPositionTo().y - lastMove.getPositionFrom().y;
            if(movedDist == yDiff){
                if(lastMove.getPositionTo().x - currentPosition.x == dx){
                    return true;
                }
            }
        }
    }
    return false;
}

int Pawn::getPromoRow(){
    return promotionRow;
}
