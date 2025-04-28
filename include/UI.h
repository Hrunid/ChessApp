#ifndef UI_H
#define UI_H

#include "Position.h"

#include <vector>


#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

class UI{
    private:

    public:
        UI::UI();
        void drawBoard();
        void movePiece(Position from, Position to);
        void deletePiece(Position pos);
        void drawPromotionChoise();
        void drawAvailableMoves(const std::vector<Position>& moves);                   
        void drawMenu();
        void drawEndGame();
        void drawSidePanel();

};

#endif