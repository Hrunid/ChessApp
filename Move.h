#ifndef MOVE_H
#define MOVE_H

#include "Position.h"

#include <string>

class Move{
    private:
        //int moveNumber;
        Position to;
        Position from;
        char movedPieceSymbol;
        std::string moveType; // c - castle, x - capture, = - promotion, + - check, # - mate

        std::string convertPositionTo(Position pos);
    public:
        Move(Position to, Position from, char movedPieceSymbol, std::string moveType);
        Position getPositionTo() const;
        Position getPositionFrom() const;
        char getPieceSymbol() const;
        std::string getMoveType() const;
        char getMovedPieceSymbol() const;
        
        std::string convertToPGN();
        

};

#endif