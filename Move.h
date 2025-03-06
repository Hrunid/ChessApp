#ifndef MOVE_H
#define MOVE_H

#include "Position.h"

#include <string>

class Move{
    private:
        Position to;
        Position from;
        char pieceSymbol;
        bool wasCapture;
        bool wasCheck;
        bool wasCastle;
        bool wasEnPassant;
        bool wasPromotion;
        char promotionPiece;
        bool wasMate;

        std::string convertPositionTo(Position pos);
    public:
        Move(Position to, Position from, char movedPieceSymbol);
        Position getPositionTo() const;
        Position getPositionFrom() const;
        char getPieceSymbol() const;
        bool capture() const;
        bool check() const;
        bool castle() const;
        bool enPassant() const;
        bool promotion() const;
        char getPromotionPieceSymbol() const;
        bool mate() const;

        void setCheck(bool check);
        void setCapture(bool capture);
        void setCastle(bool castle);
        void setEnPassant(bool enPassant);
        void setPromotion(bool promotion);
        void setPromotionPiece(char piece);
        void setMate(bool mate);

        std::string convertToPGN();
        std::string convertToUCI();
        

};

#endif