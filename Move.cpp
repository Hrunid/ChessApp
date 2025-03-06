#include "Move.h"

#include <string>

Move::Move(Position to, Position from, char movedPieceSymbol)
    :   to(to),
        from(from),
        pieceSymbol(movedPieceSymbol),
        wasCapture(false),
        wasCheck(false),
        wasCastle(false),
        wasEnPassant(false),
        wasPromotion(false),
        promotionPiece('Q'),
        wasMate(false)
        {}

Position Move::getPositionTo() const { return to; }        
Position Move::getPositionFrom() const { return from; }
char Move::getPieceSymbol() const { return pieceSymbol; }
bool Move::capture() const { return wasCapture; }
bool Move::check() const { return wasCheck; }
bool Move::castle() const { return wasCastle; }
bool Move::enPassant() const { return wasEnPassant; }
bool Move::promotion() const { return wasPromotion; }
char Move::getPromotionPieceSymbol() const { return promotionPiece; }
bool Move::mate() const { return wasMate; }
        
void Move::setCheck(bool check) { wasCheck = check; }
void Move::setCapture(bool capture) { wasCapture = capture; }
void Move::setCastle(bool castle) { wasCastle = castle; }
void Move::setEnPassant(bool enPassant) { wasEnPassant = enPassant; }
void Move::setPromotion(bool promotion) { wasPromotion = promotion; }
void Move::setPromotionPiece(char piece) { promotionPiece = piece; }
void Move::setMate(bool mate) { wasMate = mate; }        

std::string Move::convertToPGN(){
    std::string PGN = "";
    char piece;
    if(pieceSymbol = 'P'){
        piece = '\0';
    }
    else{
        piece = pieceSymbol;
    }


}

std::string convertPositionTo(Position pos){
    char x = 'a' + pos.x; 
    char y = '1' + pos.y;
    std::string s = "";
    s += x;
    s += y;
    return s;
}