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
#include <cstdlib>

class Board{
    private:
        std::unique_ptr<Piece> allPieces[32];
        std::unique_ptr<Square> squares[8][8];
        
        Player& whitePlayer;
        Player& blackPlayer;

        std::vector<Pin> pins;

        void setUpPieces();
        void createSquares();
        void createPiece(int id, char type, bool isWhite, Position pos, Board& board);
        
        bool isPinCurrent(Pin pin);
        void updatePieces(const std::vector<int>& pieceToUpdate);
        void removePieceFromSquares(int pieceId);
        void addPieceToSquares(int pieceId);
        
        
        
        void promotion(int id, char type); //TO DO
        void castle(Position from, Position to);
        void enPassant(Position from, Position to); 
        void capture(Position pieceToCapturePosition);
        void updatePiecesAtPosition(Position pos);
        
    public:
        Board(Player& whitePlayer, Player& blackPlayer);

        Piece& getPieceById(int id);
        int getPieceIdAtPosition(Position pos);
        Square& getSquareAtPosition(Position pos);
        bool isSquareEmpty(Position pos); 
        bool isOnBoard(Position pos);
        bool canPlayerCastle(bool isWhite);
        const std::vector<Pin>& getPins() const;
        std::pair<int, int> calculateDirection(Position from, Position to);
               
        void makeMove(const Move& move);
        void updatePins();
        void addPin(Pin newPin);

        
        
        
        

};

#endif