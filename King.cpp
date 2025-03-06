#include "King.h"
#include "Board.h"

King::King(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'K', isWhite, currentPosition, { {1, 0}, {-1 ,0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} })
    {}

void King::calculateAvailableMoves(){

    availableMoves.clear();
    seenBlockedSquares.clear();

    for(auto [dx, dy] : moveDirections){
        int x = currentPosition.x;
        int y = currentPosition.y;

        x += dx;
        y += dy;
        Position tempPosition(x, y);
        if(board->isOnBoard(tempPosition)){
            if(isSquareSafe(tempPosition)){
                if(board->isSquareEmpty(tempPosition)){
                    availableMoves.push_back(tempPosition);
                }
                else{
                    int tempPieceId = board->getSquareAtPosition(tempPosition).getCurrentPieceId();
                    bool tempIsWhite = board->getPieceById(tempPieceId).isPieceWhite();
                    if(tempIsWhite == isWhite){
                        seenBlockedSquares.push_back(tempPosition);
                    }
                    else{
                        availableMoves.push_back(tempPosition);
                    }
                }
            }
            else{
                seenBlockedSquares.push_back(tempPosition);
            }
        }
    }

    if(board->canPlayerCastle(isWhite)){
        for(int i = 0; i < 2; i++){
            int dx = moveDirections[i].first;
            bool isRookInDir;
            Position rookPosition(0, 0);
            if(dx < 0){
                rookPosition.x = 0;
                rookPosition.y = currentPosition.y;
            }
            else{
                rookPosition.x = 7;
                rookPosition.y = currentPosition.y;
            }
            if(!board->isSquareEmpty(rookPosition)){
                int pieceId = board->getPieceIdAtPosition(rookPosition);
                char pieceSymbol = board->getPieceById(pieceId).getSymbol();
                if(pieceSymbol == 'R'){
                    findCastleMove(dx);
                }
            }

        }
        
    }
    

}

void King::findCastleMove(int dx){

    Position tempPosition(currentPosition.x + dx, currentPosition.y);
    if(board->isSquareEmpty(tempPosition) && isSquareSafe(tempPosition)){
        Position castle(tempPosition.x + dx, tempPosition.y);
        if(board->isSquareEmpty(castle) && isSquareSafe(castle)){
            availableMoves.push_back(castle);
        }
        else{
            seenBlockedSquares.push_back(castle);
        }
    }

}

bool King::isSquareSafe(Position pos){
    const std::vector<int>& pieces = board->getSquareAtPosition(pos).getPiecesWithAcces();

    for(int pieceId : pieces){
        bool tempIsWhite = board->getPieceById(pieceId).isPieceWhite();
        if(isWhite != tempIsWhite){
            return false;
        }
    }
    return true;

}