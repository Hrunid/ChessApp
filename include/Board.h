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
#include <cstdint>
#include <string>
#include <span>
#include <array>
#include <bitset>
#include <optional>

class Board{
    private:
        std::array<std::unique_ptr<Piece>, 32> allPieces;
        std::array<std::array<Square, 8>, 8> squares;
        Player* whitePlayer;
        Player* blackPlayer;
        std::vector<Pin> pins;
        const std::vector<Move>& moveHistory;
        
        std::array<std::array<uint64_t, 64>, 12> zobristTable;                  //12 for pieces types, 64 for squares
        std::array<uint64_t, 4> castleKeys;                                     //4 castle posibilities
        std::array<uint64_t, 8> enPassantKeys;                                  //8 en passant files
        uint64_t blackPlayerKey;                                                //For black player move

        void setUpPieces();
        void createPiece(int id, char type, bool isWhite, Position pos);

        uint64_t random64BitNum();
        int getPieceIndexZ(int pieceId);
        int getSquareIndexZ(Position pos);
        int getCastleKeyZ(bool isWhite, CastleSide side);                        //Player color & castle direction
        
        bool isPinCurrent(Pin pin);        
        void addPieceToSquares(int pieceId);        
        void promotion(int id, char type);
        Position castle(Position from, Position to);
        Position enPassant(Position from, Position to); 
        void capture(Position pieceToCapturePosition);
        void updatePiecesAtPosition(Position pos);
        void movePiece(Position from, Position to);
        void generateZobristKeys();
        void updatePins();
        
    public:        
        inline static constexpr int allPiecesMAX = 32;

        Board(Player* whitePlayer, Player* blackPlayer, const std::vector<Move>& moveHis);
        ~Board();
        void setPlayerPtr(Player* player, bool isWhite);
        Piece& getPieceById(int id);
        std::optional<std::reference_wrapper<Piece>> getPieceAtPosition(Position pos);
        int getPieceIdAtPosition(Position pos) const;
        Square& getSquareAtPosition(Position pos);
        bool isSquareEmpty(Position pos) const; 
        bool isOnBoard(Position pos) const;
        const std::vector<Pin>& getPins() const;
        std::pair<int, int> calculateDirection(Position from, Position to) const;
        bool areAligned(Position from, Position to) const;

        void makeMove(const Move& move, int player);
        
        void addPin(Pin newPin);
        std::string convertToFEN();
        void removePieceFromSquares(int pieceId);
        void removePieceFromPosition(int pieceId, Position pos);
        uint64_t zobristHash(bool blackPlayer);
        std::span<const Square> getBoardView() const;
        std::span<const std::unique_ptr<Piece>> getPieceView() const;
        const Move* getLastMove() const;   


        void updatePiece(int id);
        void updatePieces(std::bitset<32> piecesToUpdate);
        int nextInDirection(std::pair<int, int> dir, Position start);
        bool hasPlayerCastleRights(bool isWhite, CastleSide side);
    };

#endif