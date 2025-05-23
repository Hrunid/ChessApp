#ifndef APP_H
#define APP_H

#include "UI.h"
#include "Game.h"

#include <SFML/Graphics.hpp>
#include <memory>

enum AppSate{
    mainMenu,

};

class App{
    private:
        sf::RenderWindow window;
        std::unique_ptr<UI> ui;
        std::unique_ptr<Game> game;

    public:
        App();
        void run();
        void startNewGame();
        void endGame();

};

#endif