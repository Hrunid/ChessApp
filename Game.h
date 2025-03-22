#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Move.h"
#include "Board.h"

#include <stack>
#include <memory>
#include <string>
#include <unordered_map>

enum Result{
    DRAW,
    WIN,
    LOSS
};

class Game{
    protected:
        std::unique_ptr<Player> players[2];
        std::unique_ptr<Board> board;
        std::stack<Move> moveHistory;
        std::unordered_map<int, int> positionHistory;
        int moveCount;
        int selectedPiece;
        int currentPlayer;
        int boringMoves;

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
};

#endif