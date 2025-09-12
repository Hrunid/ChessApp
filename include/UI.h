#ifndef UI_H
#define UI_H

#include "Position.h"
#include "Square.h"
#include "Piece.h"


#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <vector>
#include <span>
#include <memory>
#include <array>
#include <string>
#include <utility>
#include <optional>

class Game;
class App;

class UI{

    private:
        sf::RenderWindow& window;
        tgui::Gui gui;
        App& app;
        Game* game;
        std::string path;
        sf::Color lightSquare;
        sf::Color darkSquare;
        
        std::span<const Square> boardView;
        std::span<const std::unique_ptr<Piece>> pieceView;
        bool whiteSide;
        int getMappedIndex(int x, int y) const;
        Position mapPosition(Position pos) const;

        float mainContainerSize;
        tgui::Group::Ptr boardContainer;
        std::array<std::array<tgui::Button::Ptr, 8>, 8> boardButtons;
        float boardContainerH;
        float boardContainerW;
        float squareSize;

        tgui::Container::Ptr menu;
        tgui::Container::Ptr menuButtons;
        float menuButtonW;
        float menuButtonH;

        tgui::Container::Ptr sidePanel;
        float sidePanelH;
        float sidePanelW;
 
        
        float promotionPanelH;
        float promotionPanelW;

        tgui::String percent(float value) const;

        tgui::Button::Ptr createTguiBtn(
                std::string label, 
                std::pair<tgui::String, tgui::String> position, 
                std::pair<tgui::String, tgui::String> size, 
                std::optional<std::function<void()>> onClick = std::nullopt,
                std::optional<sf::Color> color = std::nullopt
            );
        
    public:
        tgui::Container::Ptr promotionPanel;
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
        void drawPromotionOptions(Position from, Position to);
        void setPerspective(bool whitePersp);

        void setGamePtr(Game* gamePtr);
        void setBoardView(std::span<const Square> boardView);
        void setPieceView(std::span<const std::unique_ptr<Piece>> pieceView);
    };

#endif