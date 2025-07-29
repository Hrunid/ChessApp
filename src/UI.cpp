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
    whiteSide(true),
    mainContainerSize("100%"),
    boardContainerH("100%"),
    boardContainerW("56.25%"),
    squareSize("12.5%"),
    menuButtonW("100%"),
    menuButtonH("20%"),
    sidePanelH("100%"),
    sidePanelW("43.75%")
{
    menu = tgui::Panel::create({"100%", "100%"});
    menu->setPosition("0%", "0%");

    tgui::Picture::Ptr background = tgui::Picture::create("C:/Users/user/Desktop/ChessApp/ChessApp/zdj/tlo.jpg");
    background->setSize("100%", "100%");
    menu->add(background);
    background->moveToBack();

    menuButtons = tgui::Panel::create();
    menuButtons->setSize("40%", "100");
    menuButtons->setPosition("30%", "0%");

    const std::vector<std::string> labels = {"Nowa gra", "Wczytaj", "Ustawienia", "Wyjscie"};
    float spacing = 0.02f;
    float btnHeight = 1.f / (labels.size() + spacing * (labels.size() - 1));
    float posY = 0.f;

    for (auto label : labels)
    {
        tgui::Button::Ptr btn = tgui::Button::create(label);
        btn->setSize("100%", tgui::String(std::to_string(btnHeight * 100.f) + "%"));
        btn->setPosition("0%", tgui::String(std::to_string(posY * 100.f) + "%"));

        
        if(label == "Wyjscie"){
            btn->onPress([this]() {
                window.close();
            });
        }
        else if(label == "Nowa gra"){
            btn->onPress([this](){
                this->app.startNewGame(TwoPlayers);
            });
        }
        else{
            btn->onPress([this]() {
                menuButtons->setVisible(false);
            });
        }
        menuButtons->add(btn);
        posY += btnHeight + spacing;
    }
    menu->add(menuButtons);
    gui.add(menu);

    boardContainer = tgui::Panel::create();
    boardContainer->setSize("56.25%", "100%");
    boardContainer->setPosition(0, 0);
    for(int i = 0; i < 8; i++){
        for(int j = 7; j >= 0; j--){
            tgui::Button::Ptr btn = tgui::Button::create();
            boardContainer->add(btn);
            boardButtons[i][j] = btn;
            btn->setSize("12.5%", "12.5%");
            
            //btn->setPosition(squareSize * i, squareSize * j);
            btn->setPosition(tgui::String(std::to_string(i * 12.5f) + "%"), tgui::String(std::to_string(j * 12.5f) + "%"));

            bool light = (i + j) % 2 == 0;
            sf::Color color = light ? sf::Color(233, 217, 209) : sf::Color(107, 70, 52);
            btn->getRenderer()->setBackgroundColor(color);
            btn->onClick([this, i, j]() {
                Position logicalPos = mapPosition(Position(i, j));
                game->processClick(logicalPos);
            });
        }
    }
    boardContainer->setVisible(false);
    gui.add(boardContainer);
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

