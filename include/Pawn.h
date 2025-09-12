#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include "Move.h"


class Pawn : public Piece{
    private:
        const int promotionRow;
        const int enPassantRow;
        inline static constexpr std::array<std::pair<int, int>, 3> moveDirsW = {{ {0, -1}, {1, -1}, {-1, -1} }};
        inline static constexpr std::array<std::pair<int, int>, 3> moveDirsB = {{ {0, 1},  {1, 1},  {-1, 1} }};
    public:
        bool canEnPassant(int dx);
        Pawn(int id, bool isWhite, Position currentPosition);
        void scanForPin(std::pair<int, int> dir, std::optional<Position> startPosition = std::nullopt) override;
        void findMovesInDirection(std::pair<int, int> direction) override;
        int getPromoRow();
        std::optional<int> enPassantCol;
};

#endif