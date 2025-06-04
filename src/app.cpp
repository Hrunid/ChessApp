#include "App.hpp"

App::App()
    :   window(sf::VideoMode({1280, 720}), "Chess"),
        game(nullptr),
        ui(std::make_unique<UI>(window, *this)),
        state(MainMenu)
    {
        window.setFramerateLimit(144);
    }

void App::run(){
    while(window.isOpen()){
        while(auto optEvent = window.pollEvent()){
            const sf::Event& event = *optEvent;
            if(event.is<sf::Event::Closed>()){
                window.close();
            }
            else if(event.is<sf::Event::Resized>()){
                ui->onResize(window.getSize());
            }
            else{
                ui->handleEvent(event);
            }
        }

        window.clear();
        drawUI();
        startNewGame(TwoPlayers);
        ui->draw();
        window.display();
    }

    
}

void App::startNewGame(GameType type){
    setAppState(InGame);
    if(type == TwoPlayers){
        game = std::make_unique<Game>();
        ui->setBoardView(game->boardView());
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
        ui->drawMenu();
    }
    else if(state == InGame){
        if(GameState gameState  = game->getGameState(); gameState == StartScreen){
            ui->drawGameSettings();
            game->setGameState(Running);
        }
        else if(gameState == Running){
            ui->drawBoard();
            ui->drawSidePanel();
            if(int pieceId = game->getSelectedPiece(); pieceId != -1){
                ui->drawAccessibleSquares((game->getPieceById(pieceId)).getAvailableMoves());
            }
            /*if(game->getCurrentPlayer().isPlayerInCheck()){

            }*/

        }
        else if (gameState == GameEnded)
        {
            ui->drawBoard();
            ui->drawEndGamePanel();
        }
        
    }
    else if(state == Settings){

    }
}

void App::setAppState(AppSate newState){
    state = newState;
}