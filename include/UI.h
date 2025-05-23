#ifndef UI_H
#define UI_H

#include "Position.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

class UI{


    private:
        sf::RenderWindow& window;
        tgui::Gui gui;
        sf::Vector2u winSize;
        float scale;

        // Parametry layoutu menu (procenty wymiarów okna)
        float menuButtonW_Pct;
        float menuButtonH_Pct;
        float menuStartY_Pct;
        float menuSpacingY_Pct;
        float menuTitleHeight_Pct;
        float menuTitleTopMargin_Pct;
    public:
        UI(sf::RenderWindow& window);

        /// Powinno być wywoływane przy zmianie rozdzielczości
        void onResize(const sf::Vector2u& newSize);

        /// Obsługa zdarzeń SFML (przekazanie do TGUI, custom klicki)
        void handleEvent(const sf::Event& evt);

        /// Rysuje główne menu z tytułem i przyciskami
        void drawMenu();

        void draw();
    };

#endif