#ifndef SQUARE_H
#define SQUARE_H

#include <vector>

class Square{
    private:
        int currentPiece;                                       
        std::vector<int> piecesWithAcces;

    public:
        Square(int currentPiece);
        
        int getCurrentPieceId() const;
        const std::vector<int>& getPiecesWithAcces() const;

        void setCurrentPiece(int newPieceId);
        void removeAttacker(int pieceToRemove);
        void addAttacker(int pieceToAdd);


};

#endif