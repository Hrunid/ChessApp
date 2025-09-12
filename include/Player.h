#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"
#include "Pin.h"
#include "Move.h"
#include "King.h"

#include <bitset>
#include <vector>
#include <optional>

class Board;

class Player{

private: 
    const int kingId;
    const bool isWhite;
    int numOfChecks;
    std::bitset<32> piecesId;
    Board* board;
    std::vector<Position> getCheckLine(Position start, Position end, char aSymb) const;
    bool sCastleRights;
    bool lCastleRights;
    
    bool hasCastleRights(CastleSide side);

public:
    
    Player(int kingId, bool isWhite);
    ~Player();
    int getKingId() const;
    bool isPlayerWhite() const;
    bool hasEnoughMaterial();
    bool isPlayerInCheck();
    bool hasPlayerMoves();
    
    std::bitset<32> getPiecesId();
    void removePlayerPiece(int pieceToRemove);
    void addPlayerPiece(int pieceToAdd);
    void applyCheckRestrictions();
    void setBoardPtr(Board* board);
    void setKingPtr();
    std::optional<Pin> pinnedOwn(const Move& lastMove);

    bool hasShortCastleRights();
    bool hasLongCastleRights();
    bool canCastleLong() const;
    bool canCastleShort() const;
};

#endif