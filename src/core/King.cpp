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

    for(int i = 0; i < 2; i++){
        int dx = moveDirections[i].first;
        if(board->canPlayerCastle(this->isWhite, dx)){
            findCastleMove(dx);
        }

    }

    

}

void King::findCastleMove(int dx){
    int n;
    if(dx > 0){
        n = 2;
    }
    else{
        n = 3;
    }
    bool canCastle = true;
    for(int i = 1; i <= n; i++){
        Position tempPosition(currentPosition.x + dx * i, currentPosition.y);
        if(i == 3){
            if(!(board->isSquareEmpty(tempPosition))){
                canCastle = false;
                break;
            }
        }
        else{
            if(!(isSquareSafe(tempPosition)) || !(board->isSquareEmpty(tempPosition))){
                canCastle = false;
                break;
            }
        }
        
    }
    if(canCastle){
        availableMoves.push_back(Position(currentPosition.x + 2 * dx, currentPosition.y));
    }
    else{
        seenBlockedSquares.push_back(Position(currentPosition.x + 2 * dx, currentPosition.y));
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