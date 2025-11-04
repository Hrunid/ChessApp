#include "Square.h"

Square::Square(int currentPiece)
    :   currentPiece(currentPiece),
        piecesWithAcces()
        {}

int Square::getCurrentPieceId() const{
    return currentPiece;
}

std::bitset<32> Square::getPiecesWithAcces() const{
    return piecesWithAcces;
}

void Square::setCurrentPiece(int newPieceId){
    currentPiece = newPieceId;
}

void Square::removeAttacker(int pieceToRemove){
    piecesWithAcces[pieceToRemove] = 0;
}

void Square::addAttacker(int pieceToAdd){
    piecesWithAcces[pieceToAdd] = 1;
}

bool Square::empty() const{
    return currentPiece == -1;
}


