#include "Pawn.h"

Pawn::Pawn(int id, bool isWhite, Position currentPosition, Board& board, Game& game)
    :   Piece(id, 'P', isWhite, currentPosition, board,
        isWhite ? std::vector<std::pair<int, int>>{{0, 1}, {1, 1}, {-1, 1}} : std::vector<std::pair<int, int>>{{0, -1}, {1, -1}, {-1, -1}}),
        game(game),
        promotionRow(isWhite ? 7 : 0),
        enPassantRow(isWhite ? 5 : 3)
    {}

Move& Pawn::getLastMove(){
    return game.getLastMove();
}

void Pawn::calculateAvailableMoves(){
    availableMoves.clear();
    int x = currentPosition.x;
    int y = currentPosition.y;
    int dy = moveDirections[0].second;

    Position tempPosition(x, y + dy);
    if(isOnBoard(tempPosition)){
        if(board.isSquareEmpty(tempPosition)){
            availableMoves.push_back(tempPosition);
            if(!hasMoved){
                tempPosition = Position(tempPosition.x, tempPosition.y + dy);
                if(isOnBoard(tempPosition) && board.isSquareEmpty(tempPosition)){
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

    for(int i = 1; i < 3; i++){
        x = currentPosition.x;
        y = currentPosition.y;
        int dx = moveDirections[i].first;
        int dy = moveDirections[i].second;
        x += dx;
        y += dy;

        Position tempPosition(x, y);

        if(isOnBoard(tempPosition)){
            if(board.isSquareEmpty(tempPosition)){
                if(canEnPassant(dx)){
                    availableMoves.push_back(tempPosition);
                }
                else{
                    seenBlockedSquares.push_back(tempPosition);
                }
            }
            else if(board.getPieceById(getPieceIdAtPosition(tempPosition)).isPieceWhite() == isWhite){
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
        Move& lastMove = getLastMove();
        char movedPiece = board.getPieceById(board.getPieceIdAtPosition(lastMove.to)).getSymbol();
        if(movedPiece == 'P'){
            int movedDist = lastMove.to.y - lastMove.from.y;
            if(movedDist == yDiff){
                if(lastMove.to.x - currentPosition.x == dx){
                    return true;
                }
            }
        }
    }
    return false;
}
