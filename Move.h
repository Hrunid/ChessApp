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

        std::string convertPositionTo();
    public:
        Move(Position to, Position from, char movedPieceSymbol, std::string moveType);
        Position getPositionTo();
        Position getPositionFrom();
        char getPieceSymbol();
        std::string getMoveType();
        std::string convertToPgn();

};

#endif