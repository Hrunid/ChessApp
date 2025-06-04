#ifndef GAME_H
#define GAME_H

#include "Player.hpp"
#include "Move.hpp"
#include "Board.hpp"
#include "Square.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

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
    GameEnded
};

class Game{
    protected:
        std::unique_ptr<Player> players[2];
        std::unique_ptr<Board> board;
        std::vector<Move> moveHistory;
        std::unordered_map<int, int> positionHistory;
        int moveCount;
        int selectedPiece;
        int currentPlayer;
        int boringMoves;
        GameState state;
        GameType type;

        void runStockfish();
        Move createMove(Position from, Position to);
        Move createMoveFromAiRespone(std::string response);
        void writePosition(std::string fen);
        void pgnToMoves(std::string pgn);      
        void executeTurn(Position from, Position to);
        void checkGameState();
        bool threeTimeRepetition();
        bool fiftyMoveRule();
    public:
        Game();
        virtual ~Game() = default;

        void startGame();
        void previousMove();
        void nextMove();
        void endGame(Result res);
        void processClick(Position click);
        GameState getGameState();
        int getSelectedPiece();
        const Piece& getPieceById(int pieceId);
        const Player& getCurrentPlayer();
        GameType getGameType();
        void setGameState(GameState newState);
        std::span<const std::unique_ptr<Square>> boardView() const;

};

#endif