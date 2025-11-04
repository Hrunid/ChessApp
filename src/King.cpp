#include "King.h"
#include "Board.h"

#include <algorithm>

King::King(int id, bool isWhite, Position currentPosition)
    :   Piece(id, 'K', isWhite, currentPosition, moveDirs),
        sCastle(false),
        lCastle(false)
    {
        availableMoves.reserve(8);
        seenBlockedSquares.reserve(8);
    }

void King::calculateAvailableMoves(){

    availableMoves.clear();
    seenBlockedSquares.clear();

    for(auto dir : moveDirs){
        Position tempPosition = currentPosition;
        tempPosition += dir;
        
        if(!board->isOnBoard(tempPosition)) continue;;
        if(!isSquareSafe(tempPosition)){
            seenBlockedSquares.emplace_back(tempPosition);
            continue;
        }

        auto pieceAtPos = board->getPieceAtPosition(tempPosition);
        if(!pieceAtPos.has_value()){
            availableMoves.emplace_back(tempPosition);
            if(dir.second == 0){
                CastleSide side = (dir.first == 1) ? CastleSide::Short : CastleSide::Long;
                findCastleMove(side, dir, tempPosition);
            }
        }
        else if(auto piece = pieceAtPos->get(); piece.isPieceWhite() == isWhite){
            seenBlockedSquares.emplace_back(tempPosition);
        }
        else{
            availableMoves.emplace_back(tempPosition);
        }
    }
}

void King::findCastleMove(CastleSide side, std::pair<int, int> dir, Position start){
    if(!board->hasPlayerCastleRights(isWhite, side)) return;
    int n = (side == CastleSide::Short) ? 1 : 2;
    start += dir;
    for(int i = 0; i < n; i++){
        if(!board->isOnBoard(start)) return;
        if(isSquareSafe(start)){
            auto p = board->getPieceAtPosition(start);
            if(!p.has_value() && i < 1) availableMoves.emplace_back(start);
            else seenBlockedSquares.emplace_back(start);
        }
    }


}

bool King::isSquareSafe(Position pos) const{
    std::bitset<32> piecesWithAccess = board->getSquareAtPosition(pos).getPiecesWithAcces();
    for(int id = piecesWithAccess._Find_first(); id < Board::allPiecesMAX; id = piecesWithAccess._Find_next(id)){
        if(board->getPieceById(id).isPieceWhite() != this->isWhite) return false;
    }
    return true;
}

void King::scanForPin(std::pair<int, int> dir, std::optional<Position> startPosition){
    
}

bool King::canCastleL() const{
    return lCastle;
}

bool King::canCastleS() const{
    return sCastle;
}