#ifndef UI_H
#define UI_H

#include "Position.hpp"
#include "Square.hpp"
#include "Piece.hpp"


#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>
#include <span>
#include <memory>
#include <array>
#include <string>

class Game;
class App;

class UI{

    private:
        sf::RenderWindow& window;
        tgui::Gui gui;
        sf::Vector2u winSize;
        float scale;
        App& app;
        Game* game;

        tgui::String mainContainerSize;

        std::span<const std::unique_ptr<Square>> boardView;
        std::span<const std::unique_ptr<Piece>> pieceView;
        bool whiteSide;
        int getMappedIndex(int x, int y) const;
        Position mapPosition(Position pos) const;

        tgui::Group::Ptr boardContainer;
        std::array<std::array<tgui::Button::Ptr, 8>, 8> boardButtons;
        tgui::String boardContainerH;
        tgui::String boardContainerW;
        tgui::String squareSize;

        tgui::Container::Ptr menu;
        tgui::Container::Ptr menuButtons;
        tgui::String menuButtonW;
        tgui::String menuButtonH;

        tgui::Container::Ptr sidePanel;
        tgui::String sidePanelH;
        tgui::String sidePanelW;

        tgui::Container::Ptr promotionPanel;
        tgui::String promotionPanelH;
        tgui::String promotionPanelW;

        
    public:
        UI(sf::RenderWindow& window, App& app);
        void handleEvent(const sf::Event& evt);
        void drawMenu();
        void drawGameSettings();
        void draw();
        void drawEndGamePanel();
        void drawBoard();
        void showMenu(bool visible);
        void showBoard(bool visible);
        void drawSidePanel();
        void drawAccessibleSquares(const std::vector<Position>& squares);
        void drawPromotionOptions(Position pos);
        void setPerspective(bool whitePersp);

        void setGamePtr(Game* gamePtr);
        void setBoardView(std::span<const std::unique_ptr<Square>> boardView);
        void setPieceView(std::span<const std::unique_ptr<Piece>> pieceView);
    };

#endif