#ifndef UI_H
#define UI_H

#include "Position.h"
#include "Square.h"
#include "Piece.h"
#include "ResourceManager.h"


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
#include <unordered_map>

class Game;
class App;

class UI{

private:
    App& app;
    tgui::Gui& gui;
    resources::ResourceManager& rm;
    Game* game;

    static inline std::unordered_map<char, resources::Piece> pieceSymbMap{
            {'K', resources::King}, 
            {'Q', resources::Queen}, 
            {'R', resources::Rook}, 
            {'B', resources::Bishop},
            {'N', resources::Knight},
            {'P', resources::Pawn} };

    sf::Color lightSquare;
    sf::Color darkSquare;
    
    std::span<const Square> boardView;
    std::span<const std::unique_ptr<Piece>> pieceView;
    bool whiteSide;
    int getMappedIndex(int x, int y) const;
    Position mapPosition(Position pos) const;

    float mainContainerSize;
    tgui::Container::Ptr boardContainer;    
    float boardContainerH;
    float boardContainerW;
    std::array<std::array<tgui::Container::Ptr, 8>, 8> boardSquares;
    float squareSize;
    tgui::Container::Ptr promotionPanel;
    tgui::Container::Ptr endGamePanel;
    float endGamePanelW;
    float endGamePanelH;

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

    tgui::Container::Ptr createBoardContainer();
    tgui::Container::Ptr createSquareContainer(
        std::pair<tgui::String, tgui::String> position, 
        tgui::Button::Ptr btn
    );
    
public:
    UI(tgui::Gui& guiRef, App& appReff, resources::ResourceManager& rmReff);
    void handleEvent(const sf::Event& evt);
    void drawMenu();
    void drawGameSettings();
    void draw();
    void drawEndGamePanel(std::string res, int player);
    void drawBoard();
    void showMenu(bool visible);
    void showBoard(bool visible);
    void drawSidePanel();
    void drawMoves(const std::vector<Position>& squares);
    void hideMoves(const std::vector<Position>& squares);
    void drawPromotionOptions(Position from, Position to);
    void setPerspective(bool whitePersp);

    void setGamePtr(Game* gamePtr);
    void setBoardView(std::span<const Square> boardView);
    void setPieceView(std::span<const std::unique_ptr<Piece>> pieceView);
};

#endif