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
        
        // Parametry layoutu menu (procenty wymiarów okna)
        float menuButtonW_Pct;
        float menuButtonH_Pct;
        float menuStartY_Pct;
        float menuSpacingY_Pct;
        float menuTitleHeight_Pct;
        float menuTitleTopMargin_Pct;
    public:
        UI(sf::RenderWindow& window, App& app);

        /// Powinno być wywoływane przy zmianie rozdzielczości
        void onResize(const sf::Vector2u& newSize);

        /// Obsługa zdarzeń SFML (przekazanie do TGUI, custom klicki)
        void handleEvent(const sf::Event& evt);

        /// Rysuje główne menu z tytułem i przyciskami
        void drawMenu();
        void drawGameSettings();
        void draw();
        void drawEndGamePanel();
        void drawBoard();
        void drawSidePanel();
        void drawAccessibleSquares(std::vector<Position> squares);

        void setBoardView(std::span<const std::unique_ptr<Square>> boardView);
    };

#endif