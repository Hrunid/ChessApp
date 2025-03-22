#ifndef UI_H
#define UI_H

#include "Position.h"

#include <vector>

class UI{
    private:

    public:
        void drawBoard();
        void movePiece(Position from, Position to);
        void deletePiece(Position pos);
        void drawPromotionChoise();
        void drawAvailableMoves(std::vector<Position> moves);                   
        void drawMenu();
        void drawEndGame();
        void drawSidePanel();

};

#endif