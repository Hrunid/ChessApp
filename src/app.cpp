#include "App.h"

#include <filesystem>
#include <windows.h>
#include <system_error>

App::App(int _argc, char* _argv[])
    :   argc(_argc),
        argv(_argv),
        exePath(setPath()),
        window(sf::VideoMode({1280, 720}), "Chess"),
        tgui{window},
        resManager(exePath),
        ui(tgui, *this, resManager),        
        game(),
        state(MainMenu),
        menuDrawn(false)
    {
        window.setFramerateLimit(60);
        ui.drawMenu();
    }

void App::run(){
    while(window.isOpen()){
        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else{
                ui.handleEvent(*event);
            }
        }
        window.clear();
        drawUI();
        window.display();
    }
}

void App::startNewGame(GameType type){
    setAppState(InGame);
    if(type == TwoPlayers){
        game = std::make_unique<Game>(type, ui);
        ui.setBoardView(game->boardView());
        ui.setPieceView(game->pieceView());
        ui.setGamePtr(game.get());
        ui.drawBoard();
    }
    else if(type == AnalisysMode){
        // new
    }
    else if(type == PlayerVsCPU){

    }
    else{
        
    }
}


void App::drawUI(){
    if(state == MainMenu){
        ui.drawMenu();
    }
    else if(state == InGame){
        if(GameState gameState  = game->getGameState(); gameState == StartScreen){
            ui.drawGameSettings();
            game->setGameState(Running);
        }
        else if(gameState == Running){
            ui.showBoard(true);
            ui.showMenu(false);
            //ui.drawSidePanel();
            ui.draw();
            
        }
        else if (gameState == Ended)
        {
            ui.drawBoard();
            
            ui.showMenu(true);
        }
        
    }
    else if(state == Settings){

    }
}

void App::setAppState(AppState newState){
    state = newState;
}

std::filesystem::path App::setPath(){
    for(int i = 0; i < argc; ++i){
        std::filesystem::path path{ argv[i] };
        if(std::filesystem::exists(path) && std::filesystem::exists(path.parent_path())){
            return path;
        }
    }
    DWORD err = ERROR_PATH_NOT_FOUND;
    throw std::system_error(
        static_cast<int>(err),
        std::system_category(),
        "Nie znaleziono katalogu z zasobami!"
    );
}