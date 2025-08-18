#include "Square.hpp"

#include <algorithm>

Square::Square(int currentPiece)
    :   currentPiece(currentPiece),
        piecesWithAcces()
        {}

int Square::getCurrentPieceId() const{
    return currentPiece;
}

const std::vector<int>& Square::getPiecesWithAcces() const{
    return piecesWithAcces;
}

void Square::setCurrentPiece(int newPieceId){
    currentPiece = newPieceId;
}

void Square::removeAttacker(int pieceToRemove){
    piecesWithAcces.erase(std::remove(piecesWithAcces.begin(), piecesWithAcces.end(), pieceToRemove), piecesWithAcces.end());
}

void Square::addAttacker(int pieceToAdd){
    piecesWithAcces.push_back(pieceToAdd);
}


