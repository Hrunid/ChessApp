#include "Move.h"

Move::Move(Position to, Position from, char movedPieceSymbol, string moveType)
    :   to(to),
        from(from),
        movedPieceSymbol(movedPieceSymbol),
        moveType(moveType)
        {}

Position Move::getPositionTo(){
    return to;
}

Position Move::getPositionFrom(){
    return from;
}

char Move::getMovedPieceSymbol(){
    return getMovedPieceSymbol;
}

std::string Move::getMoveType(){
    return moveType;
}

std::string Move::convertToPGN(){
    string PGN = "";
    char piece;
    if(movedPieceSymbol = 'P'){
        piece = '';
    }
    else{
        piece = movedPieceSymbol;
    }

}

std::string convertPositionTo(Position pos){
    
}