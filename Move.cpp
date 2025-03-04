#include "Move.h"

#include <string>

Move::Move(Position to, Position from, char movedPieceSymbol, std::string moveType)
    :   to(to),
        from(from),
        movedPieceSymbol(movedPieceSymbol),
        moveType(moveType)
        {}

Position Move::getPositionTo() const{
    return to;
}

Position Move::getPositionFrom() const{
    return from;
}

char Move::getMovedPieceSymbol() const{
    return movedPieceSymbol;
}

std::string Move::getMoveType() const{
    return moveType;
}

std::string Move::convertToPGN(){
    std::string PGN = "";
    char piece;
    if(movedPieceSymbol = 'P'){
        piece = '\0';
    }
    else{
        piece = movedPieceSymbol;
    }

    PGN = PGN + piece + moveType + convertPositionTo(to);

}

std::string convertPositionTo(Position pos){
    char x = 'a' + pos.x; 
    char y = '1' + pos.y;
    std::string s = "";
    s += x;
    s += y;
    return s;
}