#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "Square.h"
#include "UI.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <optional>

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
        std::unordered_map<uint64_t, int> positionHistory;
        int moveCount;
        int selectedPiece;
        int currentPlayer;
        int boringMoves;
        GameState state;
        GameType type;
        UI& ui;
        //Stockfish::UCIEngine cpu;
        virtual void runStockfish();
        Move createMove(Position from, Position to, std::optional<char> promPiece = std::nullopt);
        Move createMoveFromAiRespone(std::string response);
        void writePosition(std::string fen);
        void pgnToMoves(std::string pgn);      
        void executeTurn(Position from, Position to, std::optional<char> promPiece = std::nullopt);
        void checkGameState();
        bool threeTimeRepetition();
        bool fiftyMoveRule();

        void setSelected(int newSel);
        void makeMove();

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