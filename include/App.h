#ifndef APP_H
#define APP_H

#include "UI.h"
#include "Game.h"
#include "ResourceManager.h"

#include <filesystem>
#include <SFML/Graphics.hpp>
#include <memory>

enum AppState{
    MainMenu,
    InGame,
    Settings

};

class App{
private:
    int argc;
    char** argv;
    std::filesystem::path exePath;

    sf::RenderWindow window;
    tgui::Gui tgui;    
    resources::ResourceManager resManager;       
    UI ui;
 
    std::unique_ptr<Game> game;
    AppState state;
    bool menuDrawn;

    std::filesystem::path setPath();
    void drawUI();

public:
    App(int _argc, char* _argv[]);
    void run();
    void startNewGame(GameType type);
    void setAppState(AppState newState);

};

#endif