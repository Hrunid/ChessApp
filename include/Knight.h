#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece{

private:
    inline static constexpr std::array<std::pair<int, int>, 8> moveDirs = {{ {1, 2}, {1, -2}, {2, 1}, {2, -1}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1} }};

public:
    Knight(int id, bool isWhite, Position currentPosition);

    void findMovesInDirection(std::pair<int, int> direction) override;
    void scanForPin(std::pair<int, int> dir, std::optional<Position> startPosition = std::nullopt) override;
};

#endif