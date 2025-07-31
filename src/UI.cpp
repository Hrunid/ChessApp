#include "UI.hpp"
#include "App.hpp"
#include "Game.hpp"

#include <iostream>

UI::UI(sf::RenderWindow& wnd, App& app)
    : window(wnd),
    gui(window),
    boardView(),
    winSize(window.getSize()),
    app(app),
    game(),
    whiteSide(false),
    mainContainerSize("100%"),
    boardContainerH("100%"),
    boardContainerW("56.25%"),
    squareSize("12.5%"),
    menuButtonW("60%"),
    menuButtonH("40%"),
    sidePanelH("100%"),
    sidePanelW("43.75%")
    {
        menu = tgui::Panel::create({mainContainerSize, mainContainerSize});
        menu->setPosition("0%", "0%");

        tgui::Picture::Ptr background = tgui::Picture::create("C:/Users/user/Desktop/ChessApp/ChessApp/zdj/tlo.jpg");
        background->setSize(mainContainerSize, mainContainerSize);
        menu->add(background);
        background->moveToBack();

        menuButtons = tgui::Panel::create({menuButtonW, menuButtonH});
        menuButtons->setPosition("20%", "30%");

        const std::vector<tgui::String> labels = {"Nowa gra", "Wczytaj", "Ustawienia", "Wyjscie"};
        for(int i = 0; i < labels.size(); i++){
            tgui::Button::Ptr btn = tgui::Button::create(labels[i]);
            btn->setSize("100%", "22.5%");
            btn->setPosition("0%", tgui::String(i * 25) + "%");
            if(i == 0){
                btn->onPress([this]() {
                    this->app.startNewGame(TwoPlayers);
                });
            }
            else if(i == 1){
                btn->onPress([this]() {
                    std::cout << "Load game functionality not implemented yet." << std::endl;
                });
            }
            else if(i == 2){
                btn->onPress([this]() {
                    std::cout << "Settings functionality not implemented yet." << std::endl;
                });

            }
            else if(i == 3){
                btn->onPress([this]() {
                    window.close();
                });
            }   
            menuButtons->add(btn);
            menuButtons->getRenderer()->setTransparentTexture(false);
        }
        menu->add(menuButtons);
        gui.add(menu);

        boardContainer = tgui::Panel::create({boardContainerW, boardContainerH});
        boardContainer->setPosition(0, 0);
        for(int i = 0; i < 8; i++){
            for(int j = 7; j >= 0; j--){
                tgui::Button::Ptr btn = tgui::Button::create();
                boardContainer->add(btn);
                boardButtons[i][j] = btn;
                btn->setSize(squareSize, squareSize);
                btn->setPosition(tgui::String(std::to_string(i * 12.5f) + "%"), tgui::String(std::to_string(j * 12.5f) + "%"));

                bool light = (i + j) % 2 == 0;
                sf::Color color = light ? sf::Color(233, 217, 209) : sf::Color(107, 70, 52);
                btn->getRenderer()->setBackgroundColor(color);
                btn->onClick([this, i, j]() {
                    Position logicalPos = mapPosition(Position(i, j));
                    game->processClick(logicalPos);
                    promotionPanel->setVisible(false);
                });
            }
        }
        boardContainer->setVisible(false);
        gui.add(boardContainer);

        promotionPanel = tgui::Panel::create({squareSize, "50%"});
        promotionPanel->setVisible(false);
        boardContainer->add(promotionPanel);
        for(int i = 0; i < 4; i++){
            tgui::Button::Ptr btn = tgui::Button::create();
            btn->setSize("100%", "25%");
            btn->setPosition("0%", tgui::String(25 * i) + "%");
            promotionPanel->add(btn);
        }
    }

void UI::handleEvent(const sf::Event& evt){
    gui.handleEvent(evt);
}

void UI::drawMenu()
{
    this->draw();
}

void UI::draw(){
    gui.draw();
}

void UI::drawGameSettings(){}
void UI::UI::drawEndGamePanel(){
    
}
void UI::drawBoard()
{
    boardContainer->setVisible(true);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int index = getMappedIndex(i, j);
            int pieceId = boardView[index]->getCurrentPieceId();
            std::string text = "";
            if(pieceId != -1){
                char symbol = game->getPieceById(pieceId).getSymbol();
                text += symbol;
                text += std::to_string(pieceId);
            }
            boardButtons[i][j]->setText(text);
        }
    }
    this->draw();
}

void UI::drawSidePanel(){}
void UI::drawAccessibleSquares(const std::vector<Position>& squares){

    for(Position pos : squares){
        auto mappedPos = mapPosition(pos);
        boardButtons[mappedPos.x][mappedPos.y]->setText("Accessible");
    }
    this->draw();
}
void UI::setBoardView(std::span<const std::unique_ptr<Square>> boardVieww){
    this->boardView = boardVieww;
}
void UI::setPieceView(std::span<const std::unique_ptr<Piece>> pieceView){
    this->pieceView = pieceView;
}

void UI::showMenu(bool visible){
    menu->setVisible(visible);
}
void UI::showBoard(bool visible){
    boardContainer->setVisible(visible);
}

void UI::setGamePtr(Game* gamePtr){
    this->game = gamePtr;
}

void UI::drawPromotionOptions(Position pos){
    std::cout << "called method drawPromotionOptions\n";
    
    if(whiteSide){
        float offset = pos.y == 7 ? 3 : 0;
        promotionPanel->setPosition(tgui::String(12.5 * pos.x) + "%", tgui::String(12.5 * pos.y - (12.5f * offset)) + "%");
        
    }
    else{
        float offset = pos.y == 7 ? 0 : 3;
        promotionPanel->setPosition(tgui::String(12.5 * (8 - pos.x)) + "%", tgui::String(12.5 * (8 - pos.y) - (12.5 * offset)) + "%");
    }
    promotionPanel->setVisible(true);
}

void UI::setPerspective(bool whitePersp){
    whiteSide = whitePersp;
}

int UI::getMappedIndex(int x, int y) const{
    return whiteSide ? x * 8 + y : (7 - x) * 8 + (7 - y);
}
Position UI::mapPosition(Position pos) const{
    return whiteSide ? pos : Position(7 - pos.x, 7 - pos.y);
}

