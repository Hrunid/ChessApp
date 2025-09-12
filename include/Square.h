#ifndef SQUARE_H
#define SQUARE_H

#include <bitset>

class Square{
    private:
        int currentPiece;                                       
        std::bitset<32> piecesWithAcces;

    public:
        Square(int currentPiece = -1);
        
        int getCurrentPieceId() const;
        std::bitset<32> getPiecesWithAcces() const;

        void setCurrentPiece(int newPieceId);
        void removeAttacker(int pieceToRemove);
        void addAttacker(int pieceToAdd);

};

#endif