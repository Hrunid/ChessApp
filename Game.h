#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Move.h"
#include "Board.h"

#include <stack>
#include <memory>
#include <string>
#include <unordered_map>

class Game{
    protected:
        std::unique_ptr<Player> players;
        std::unique_ptr<Board> board;
        std::stack<Move> moveHistory;
        std::unordered_map<std::string, int> positionHistory;
        int moveCount;
        int selectedPiece;
        int currentPlayer;

        void runStockfish();
        void createMove(int x, int y);
        void createMoveFromAiRespone(std::string response);
        void writePosition(std::string fen);
        void pgnToMoves(std::string pgn);      
        std::string gameResult();
        void executeTurn();
    public:
        Game();
        virtual ~Game() = default;

        void startGame();
        void previousMove();
        void nextMove();
        void endGame();
        void processClick(int x, int y);
};

#endif