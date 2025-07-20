#ifndef UI_H
#define UI_H

#include "Position.hpp"
#include "Square.hpp"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>
#include <span>
#include <memory>

class Game;
class App;

class UI{


    private:
        sf::RenderWindow& window;
        tgui::Gui gui;
        sf::Vector2u winSize;
        float scale;
        App& app;

        std::span<const std::unique_ptr<Square>> boardView;
        tgui::Group::Ptr boardContainer;
        tgui::Button::Ptr boardButtons[8][8];
        
        tgui::Container::Ptr menu;
        tgui::Container::Ptr menuButtons;
        
        float menuButtonW_Pct;
        float menuButtonH_Pct;
        float menuStartY_Pct;
        float menuSpacingY_Pct;
        float menuTitleHeight_Pct;
        float menuTitleTopMargin_Pct;
    public:
        UI(sf::RenderWindow& window, App& app);
        void handleEvent(const sf::Event& evt);
        void drawMenu();
        void drawGameSettings();
        void draw();
        void drawEndGamePanel();
        void drawBoard(bool whiteSide);
        void showMenu(bool visible);
        void showBoard(bool visible);
        void drawSidePanel();
        void drawAccessibleSquares(std::vector<Position> squares);

        void setBoardView(std::span<const std::unique_ptr<Square>> boardView);
    };

#endif