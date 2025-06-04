#include "Square.hpp"

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
    for(int i=0; i < piecesWithAcces.size(); i++){
        
        if(int pieceId = piecesWithAcces[i]; pieceId == pieceToRemove){
            piecesWithAcces.erase(piecesWithAcces.begin() + i);
            break;
        } 
    }
}

void Square::addAttacker(int pieceToAdd){
    piecesWithAcces.push_back(pieceToAdd);
}


