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
        Board& board;

    public:
        Player(int kingId, bool isWhite, Board& board);
        
        int getKingId() const;
        bool isPlayerWhite() const;
        bool hasEnoughMaterial();
        bool isPlayerInCheck();
        bool hasPlayerMoves();
        //bool canPlayerCastle();

        void removePlayerPiece(int pieceToRemove);
        void addPlayerPiece(int pieceToAdd);

        //void createNewPieceForPlayer()
        //void applyCheckRestrictions();

};

#endif