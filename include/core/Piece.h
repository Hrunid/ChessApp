#ifndef PIECE_H
#define PIECE_H

#include "Position.h"
#include "Pin.h"

#include <vector>
#include <iostream>
#include <utility>

class Board;

class Piece{
    protected:
        const int id;
        const char symbol; 
        const bool isWhite;
        const std::vector<std::pair<int, int>> moveDirections;
        static Board* board;
        Position currentPosition;
        bool isPinning;
        bool isPinned;
        bool hasMoved;
        std::vector<Position> availableMoves;
        std::vector<Position> seenBlockedSquares;
        
        virtual void findMovesInDirection(std::pair<int, int> direction);

    public:
        Piece(int id, char symbol, bool isWhite, Position currentPosition, const std::vector<std::pair<int, int>>& moveDirections);
        
        virtual ~Piece() = default;
        
        virtual void calculateAvailableMoves();                                 
        virtual void scanForPin(Position startPosition, int dx, int dy);        
        virtual void updateMoves(Position unlockedSquare);                      


        int getId() const;
        char getSymbol() const;
        Position getPosition() const;
        bool isPieceWhite() const;
        const std::vector<Position>& getAvailableMoves() const;
        const std::vector<Position>& getSeenBlockedSquares() const;
        bool isPiecePinned() const;
        bool hasPieceMoved() const;
        bool isMoveAvailable(Position pos);
        
        static void setBoardPtr(Board* b);
        void setPosition(Position newPosition);
        void setPinningStatus(bool pins);
        void setPin(bool pin);
        void pieceHasMoved();
        void clearMoves();
        void addMove(Position pos);

        

        
};

#endif