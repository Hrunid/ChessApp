#ifndef PIECE_H
#define PIECE_H

#include "Position.h"
#include "Pin.h"

#include <vector>
#include <iostream>
#include <utility>
#include <optional>
#include <array>
#include <span>

class Board;

class Piece{
    protected:
        inline static Board* board;
        const int id;
        const char symbol; 
        const bool isWhite;
        std::span<const std::pair<int, int>> moveDirections;
        Position currentPosition;
        bool isPinning;
        bool isPinned;
        bool hasMoved;
        std::vector<Position> availableMoves;
        std::vector<Position> seenBlockedSquares;
        
        virtual void findMovesInDirection(std::pair<int, int> direction);

    public:
        Piece(int id, char symbol, bool isWhite, Position currentPosition, std::span<const std::pair<int, int>> moveDirections);
        
        virtual ~Piece() = default;
        
        virtual void calculateAvailableMoves();                                 
        virtual void scanForPin(std::pair<int, int> dir, std::optional<Position> startPosition = std::nullopt);        
        virtual void updateMoves(Position unlockedSquare);                      


        int getId() const;
        char getSymbol() const;
        Position getPosition() const;
        bool isPieceWhite() const;
        const std::vector<Position>& getAvailableMoves() const;
        const std::vector<Position>& getSeenBlockedSquares() const;
        std::span<const std::pair<int, int>> getMoveDirections() const;
        bool isPiecePinned() const;
        bool hasPieceMoved() const;
        bool isMoveAvailable(Position pos) const;
        
        static void setBoardPtr(Board* b);
        void setPosition(Position newPosition);
        void setPinningStatus(bool pins);
        void setPin(bool pin);
        void pieceHasMoved();
        void clearMoves();
        void doubleCheck();
        void addMove(Position pos);
        void removeMove(Position pos);
};

#endif