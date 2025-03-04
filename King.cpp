#include "King.h"

King::King(int id, bool isWhite, Position currentPosition, Board& board)
    :   Piece(id, 'K', isWhite, currentPosition, board, { {1, 0}, {-1 ,0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} })
    {}

void King::calculateAvailableMoves(){
    for(auto [dx, dy] : moveDirections){
        int x = currentPosition.x;
        int y = currentPosition.y;

        x += dx;
        y += dy;
        Position tempPosition(x, y);
        if(board.isOnBoard(tempPosition)){
            if(isSquareSafe(tempPosition)){
                if(board.isSquareEmpty(tempPosition)){
                    availableMoves.push_back(tempPosition);
                }
                else{
                    int tempPieceId = board.getSquareAtPosition(tempPosition).getCurrentPieceId();
                    bool tempIsWhite = board.getPieceById(tempPieceId).isPieceWhite();
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
}

bool King::isSquareSafe(Position pos){
    std::vector<int>& pieces = board.getSquareAtPosition(pos).getPiecesWithAcces();

    for(int pieceId : pieces){
        bool tempIsWhite = board.getPieceById(pieceId).isPieceWhite();
        if(isWhite != tempIsWhite){
            return false;
        }
    }
    return true;

}