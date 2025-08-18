#ifndef APP_H
#define APP_H

#include "UI.hpp"
#include "Game.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

enum AppState{
    MainMenu,
    InGame,
    Settings

};

class App{
    private:
        sf::RenderWindow window;
        std::unique_ptr<UI> ui;
        std::unique_ptr<Game> game;
        AppState state;
        bool menuDrawn;

        void drawUI();

    public:
        App();
        void run();
        void startNewGame(GameType type);
        void setAppState(AppState newState);

};

#endif