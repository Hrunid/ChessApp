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
    scale(1.0f),
    menuButtonW_Pct(0.30f),
    menuButtonH_Pct(0.08f),
    menuStartY_Pct(0.25f),
    menuSpacingY_Pct(0.03f),
    menuTitleHeight_Pct(0.10f),
    menuTitleTopMargin_Pct(0.05f)
{
    menu = tgui::Panel::create({"100%", "100%"});
    menu->setPosition("0%", "0%");

    tgui::Picture::Ptr background = tgui::Picture::create("C:/Users/wiktw/OneDrive/Dokumenty/c++/SzachyZId/ChessApp/zdj/tlo.jpg");
    background->setSize("100%", "100%");
    
    menu->add(background);
    background->moveToBack();

    



    menuButtons = tgui::Panel::create();
    menuButtons->setSize("40%", "100%");
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
                std::cout << "Kliknieto\n";
            });
        }
        menuButtons->add(btn);
        posY += btnHeight + spacing;
    }
    menu->add(menuButtons);
    gui.add(menu);


    
    boardContainer = tgui::Panel::create();
    //auto boardSize = window.getSize().y * 0.7;
    boardContainer->setSize("56.25%", "100%");
    boardContainer->setPosition(0, 0);
    //float squareSize = boardSize / 8;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            tgui::Button::Ptr btn = tgui::Button::create();
            boardContainer->add(btn);
            btn->setSize("12.5%", "12.5%");
            
            //btn->setPosition(squareSize * i, squareSize * j);
            btn->setPosition(tgui::String(std::to_string(i * 12.5f) + "%"), tgui::String(std::to_string(j * 12.5f) + "%"));

            bool light = (i + j) % 2 == 0;
            sf::Color color = light ? sf::Color(233, 217, 209) : sf::Color(107, 70, 52);
            btn->getRenderer()->setBackgroundColor(color);
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
void UI::UI::drawEndGamePanel(){}

void UI::drawBoard(bool whiteSide)
{
    std::cout << "Called UI::drawBorad()\n";
    boardContainer->setVisible(true);
    this->draw();
}

void UI::drawSidePanel(){}
void UI::drawAccessibleSquares(std::vector<Position> squares){}
void UI::setBoardView(std::span<const std::unique_ptr<Square>> boardView){
    this->boardView = boardView;
}

void UI::showMenu(bool visible){
    menu->setVisible(visible);
}
void UI::showBoard(bool visible){
    boardContainer->setVisible(visible);
}

