#ifndef GAME_H
#define GAME_H

#include "Player.hpp"
#include "Move.hpp"
#include "Board.hpp"
#include "Square.hpp"
#include "UI.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

#include "uci.h"


enum Result{
    Draw,
    Win,
    Loss
};

enum GameType{
    TwoPlayers,
    AnalisysMode,
    PlayerVsCPU
};

enum GameState{
    StartScreen,
    Running,
    Ended
};

class Game{
    protected:
        std::array<Player, 2> players;
        Board board;
        std::vector<Move> moveHistory;
        std::unordered_map<int, int> positionHistory;
        int moveCount;
        int selectedPiece;
        int currentPlayer;
        int boringMoves;
        GameState state;
        GameType type;
        UI& ui;
        //Stockfish::UCIEngine cpu;
        virtual void runStockfish();
        Move createMove(Position from, Position to, bool promotion = false, char promPiece = '\0');
        Move createMoveFromAiRespone(std::string response);
        void writePosition(std::string fen);
        void pgnToMoves(std::string pgn);      
        void executeTurn(Position from, Position to, bool promotion = false, char symb = '\0');
        void checkGameState();
        bool threeTimeRepetition();
        bool fiftyMoveRule();
    public:
        Game(GameType type, UI& uiReff);
        virtual ~Game() = default;

        bool prom = false;

        void startGame();
        void previousMove();
        void nextMove();
        void endGame(Result res);
        void processClick(Position click);
        void promote(Position from, Position to, char symb);
        GameState getGameState() const;
        int getSelectedPiece() const;
        const Piece& getPieceById(int pieceId);
        const Player& getCurrentPlayer();
        GameType getGameType();
        void setGameState(GameState newState);
        std::span<const Square> boardView() const;
        std::span<const std::unique_ptr<Piece>> pieceView() const;
};

#endif