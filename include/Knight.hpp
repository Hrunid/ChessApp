#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.hpp"

class Knight : public Piece{
    public:
        Knight(int id, bool isWhite, Position currentPosition);

        void findMovesInDirection(std::pair<int, int> direction) override;
        void scanForPin(Position startPosition, std::pair<int, int> dir) override;
};

#endif