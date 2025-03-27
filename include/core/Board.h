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
#include <string>

class Board{
    private:
        std::unique_ptr<Piece> allPieces[32];
        std::unique_ptr<Square> squares[8][8];
        Player* whitePlayer;
        Player* blackPlayer;
        std::vector<Pin> pins;
        
        uint64_t zobristTable[12][64];                                          //12 for pieces types, 64 for squares
        uint64_t castleKeys[4];                                                 //4 castle posibilities
        uint64_t enPassantKeys[8];                                              //8 en passant files
        uint64_t blackPlayerKey;                                                //For black player move

        void setUpPieces();
        void createSquares();
        void createPiece(int id, char type, bool isWhite, Position pos);

        uint64_t random64BitNum();
        int getPieceIndexZ(int pieceId);
        int getSquareIndexZ(Position pos);
        int getCastleKeyZ(bool isWhite, int dx);                                //Player color & castle direction
        
        bool isPinCurrent(Pin pin);        
        void addPieceToSquares(int pieceId);        
        void promotion(int id, char type);
        Position castle(Position from, Position to);
        Position enPassant(Position from, Position to); 
        void capture(Position pieceToCapturePosition);
        void updatePiecesAtPosition(Position pos);
        void movePiece(Position from, Position to);
        
    public:
        Board();
        ~Board();
        void setPlayerPtr(Player* player, bool isWhite);
        Piece& getPieceById(int id);
        int getPieceIdAtPosition(Position pos);
        Square& getSquareAtPosition(Position pos);
        bool isSquareEmpty(Position pos); 
        bool isOnBoard(Position pos);
        bool canPlayerCastle(bool isWhite, int dx);
        const std::vector<Pin>& getPins() const;
        std::pair<int, int> calculateDirection(Position from, Position to);
        void updatePieces(const std::vector<int>& pieceToUpdate);       
        void makeMove(const Move& move);
        void updatePins();
        void addPin(Pin newPin);
        std::string convertToFEN();
        void removePieceFromSquares(int pieceId);
        uint64_t zobristHash(bool blackPlayer, std::pair<bool, int> enPassant);


        
        
        
        

};

#endif