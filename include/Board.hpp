#ifndef BOARD_H
#define BOARD_H

#include "Piece.hpp"
#include "Square.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "Pin.hpp"
#include "Move.hpp"

#include <memory>
#include <vector>
#include <cstdlib>
#include <string>
#include <span>
#include <array>

class Board{
    private:
        std::array<std::unique_ptr<Piece>, 32> allPieces;
        std::array<std::array<std::unique_ptr<Square>, 8>, 8> squares;
        Player* whitePlayer;
        Player* blackPlayer;
        std::vector<Pin> pins;
        const std::vector<Move>& moveHistory;
        
        std::array<std::array<uint64_t, 64>, 12> zobristTable;                  //12 for pieces types, 64 for squares
        std::array<uint64_t, 4> castleKeys;                                     //4 castle posibilities
        std::array<uint64_t, 8> enPassantKeys;                                  //8 en passant files
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
        void generateRandNumbers();
        
    public:
        Board(Player* whitePlayer, Player* blackPlayer, const std::vector<Move> moveHis);
        ~Board();
        void setPlayerPtr(Player* player, bool isWhite);
        Piece& getPieceById(int id);
        int getPieceIdAtPosition(Position pos) const;
        Square& getSquareAtPosition(Position pos);
        bool isSquareEmpty(Position pos) const; 
        bool isOnBoard(Position pos) const;
        bool canPlayerCastle(bool isWhite, int dx) const;
        const std::vector<Pin>& getPins() const;
        std::pair<int, int> calculateDirection(Position from, Position to) const;
        void updatePieces(const std::vector<int>& pieceToUpdate);       
        void makeMove(const Move& move);
        void updatePins();
        void addPin(Pin newPin);
        std::string convertToFEN();
        void removePieceFromSquares(int pieceId);
        void removePieceFromPosition(int pieceId, Position pos);
        uint64_t zobristHash(bool blackPlayer);
        std::span<const std::unique_ptr<Square>> getBoardView() const;
        std::span<const std::unique_ptr<Piece>> getPieceView() const;
        const Move* getLastMove() const;   
 };

#endif