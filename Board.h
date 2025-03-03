#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Square.h"
#include "Player.h"
#include "Position.h"
#include "Pin.h"
#include "Move.h"

#include <memory>
#include <vector>

class Board{
    private:
        std::unique_ptr<Piece> allPieces[32];
        std::unique_ptr<Square> squares[8][8];
        
        Player& whitePlayer;
        Player& blackPlayer;

        std::vector<Pin> pins;

        void setUpPieces();
        void createSquares();

        
        bool isPinCurrent(Pin pin);
        void updatePieces(std::vector<int>& pieceToUpdate);
        void removePieceFromSquares(int pieceId);
        void addPieceToSquares(int pieceId);
        void capture(Position pieceToCapturePosition);
        void createPiece(int id, char type, bool isWhite, Position pos, Board& board);
        
    public:
        Board(Player& whitePlayer, Player& blackPlayer);

        Piece& getPieceById(int id);
        int getPieceIdAtPosition(Position pos);
        Square& getSquareAtPosition(Position pos);
        bool isSquareEmpty(Position pos); 
        bool isOnBoard(Position pos);

        void promotion(int id, char pieceToPromoteTo); //TO DO
        void castle(); 
        void makeMove(const Move& move);
        void updatePins();
        void addPin(Pin newPin);
        
        
        
        

};

#endif