#include "App.h"

App::App()
    :   window(sf::VideoMode({1280, 720}), "Chess"),
        ui(std::make_unique<UI>(window, *this)),        
        game(),
        state(MainMenu),
        menuDrawn(false)
    {
        window.setFramerateLimit(60);
        ui->drawMenu();
    }

void App::run(){
    while(window.isOpen()){
        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else{
                ui->handleEvent(*event);
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
        game = std::make_unique<Game>(type, *ui);
        ui->setBoardView(game->boardView());
        ui->setPieceView(game->pieceView());
        ui->setGamePtr(game.get());
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
            ui->showMenu(false);
            ui->drawBoard();
            //ui->drawSidePanel();
            if(int pieceId = game->getSelectedPiece(); pieceId != -1){
                ui->drawAccessibleSquares((game->getPieceById(pieceId)).getAvailableMoves());
            }
            
        }
        else if (gameState == Ended)
        {
            ui->drawBoard();
            ui->drawEndGamePanel();
            ui->showMenu(true);
        }
        
    }
    else if(state == Settings){

    }
}

void App::setAppState(AppState newState){
    state = newState;
}