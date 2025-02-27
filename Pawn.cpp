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
    enPassant();
    int x = currentPosition.x;
    int y = currentPosition.y;
    int dy = moveDirections[0].second;
    for(int i = 0; i < 2; i++){

        y += dy;

        if(board.isSquareEmpty(Position(x, y))){
            availableMoves.push_back(Position(x, y));
        }
        else{
            seenBlockedSquares.push_back(Position(x, y));
            break;
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

        if(!board.isSquareEmpty(tempPosition) && (board.getPieceById(board.getPieceIdAtPosition(tempPosition)).isWhite() != isWhite)){
            availableMoves.push_back(tempPosition);
        }
        else{
            seenBlockedSquares.push_back(tempPosition);
        }


        
    }



    
}

void Pawn::scanForPin(Position startPosition, int dx, int dy){}

void Pawn::enPassant(){

    int dy = moveDirections[0].second;
    int yDiff;
    if(isWhite){
        yDiff = -2;
    }
    else{
        yDiff = 2;
    }
    if(currentPosition.y == enPassantRow){
        Move& lastMove = getLastMove();
        char movedPiece = getPieceById(getPieceIdAtPosition(lastMove.to)).getSymbol();
        if(movedPiece == 'P'){
            int movedDist = lastMove.to.y - lastMove.from.y;
            int yDiff
            if(movedDist == yDiff){
                if((currentPosition.x - lastMove.to.x) == 1){
                    availableMoves.push_back(Position(currentPosition.x -1, currentPosition.y + dy));
                }
                else if((currentPosition.x - lastMove.to.x) == -1){
                    availableMoves.push_back(Position(currentPosition.x + 1, currentPosition.y + dy));
                }
            }
        }
    }
}
