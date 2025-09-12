#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece{

private:
    inline static constexpr std::array<std::pair<int, int>, 8> moveDirs = {{ {1, 0}, {-1 ,0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} }};

    bool sCastle;
    bool lCastle;    
    
    bool isSquareSafe(Position pos) const;
    void findCastleMove(CastleSide side, std::pair<int, int> dir, Position start);

public:
    King(int id, bool isWhite, Position currentPosition);
    void calculateAvailableMoves() override;
    void scanForPin(std::pair<int, int> dir, std::optional<Position> startPosition = std::nullopt) override;

    bool canCastleL() const;
    bool canCastleS() const;
};

#endif