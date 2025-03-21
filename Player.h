#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"

#include <vector>

class Board;

class Player{
    private: 
        const int kingId;
        const bool isWhite;
        int numOfChecks;
        std::vector<int> piecesId;
        Board* board;

        std::vector<Position> getCheckLine(Position start, Position end);

    public:
        Player(int kingId, bool isWhite);
        ~Player();
        int getKingId() const;
        bool isPlayerWhite() const;
        bool hasEnoughMaterial();
        bool isPlayerInCheck();
        bool hasPlayerMoves();
        bool canPlayerCastle();
        const std::vector<int>& getPiecesId();
        void removePlayerPiece(int pieceToRemove);
        void addPlayerPiece(int pieceToAdd);
        void applyCheckRestrictions();

        void setBoardPtr(Board* board);

};

#endif