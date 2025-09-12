#include "UI.h"
#include "App.h"
#include "Game.h"

#include <iostream>

UI::UI(sf::RenderWindow& wnd, App& app)
    : window(wnd),    
    app(app),
    gui(window),
    path(__FILE__),
    lightSquare(233, 217, 209),
    darkSquare(107, 70, 52),
    boardView(),
    game(),
    whiteSide(true),
    mainContainerSize(100.0f),
    boardContainerH(100.0f),
    boardContainerW(56.25f),
    squareSize(12.5f), 
    menuButtonW(60.0f),
    menuButtonH(40.0f),
    sidePanelH(100.0f),
    sidePanelW(43.75f)
    {
        menu = tgui::Panel::create({percent(mainContainerSize), percent(mainContainerSize)});
        menu->setPosition("0%", "0%");

        tgui::Picture::Ptr background = tgui::Picture::create("C:/Users/wiktw/OneDrive/Dokumenty/c++/SzachyZId/ChessApp/res/tlo.jpg");
        background->setSize(percent(mainContainerSize), percent(mainContainerSize));
        menu->add(background);
        background->moveToBack();

        menuButtons = tgui::Panel::create({percent(menuButtonW), percent(menuButtonH)});
        menuButtons->setPosition("20%", "30%");
        std::function<void()> functions[4] = {
            [this](){ this->app.startNewGame(TwoPlayers);},
            [this](){ std::cout << "Load game functionality not implemented yet." << std::endl;},
            [this](){ std::cout << "Settings functionality not implemented yet." << std::endl;},
            [this](){ this->window.close();}
        };
        std::string labels[4] = {"Nowa gra", "Wczytaj", "Ustawienia", "Wyjscie"};

        for(int i = 0; i < 4; i++){
            auto btn = createTguiBtn(labels[i], {percent(0), percent(25*i)}, {"100%", percent(25)}, functions[i]);
            menuButtons->add(btn);
        }
        menuButtons->getRenderer()->setTransparentTexture(true);
        menu->add(menuButtons);
        gui.add(menu);

        boardContainer = tgui::Panel::create({percent(boardContainerW), percent(boardContainerH)});
        boardContainer->setPosition(0, 0);
        for(int i = 0; i < 8; i++){
            for(int j = 7; j >= 0; j--){
                std::function fun = [this, i, j]() {
                    Position logicalPos = mapPosition(Position(i, j));
                    game->processClick(logicalPos);
                    promotionPanel->setVisible(false);
                };
                bool light = (i + j) % 2 == 0;
                sf::Color color = light ? lightSquare : darkSquare;
                tgui::Button::Ptr btn = createTguiBtn
                    (
                        "", 
                        {percent(i * squareSize), percent(j * squareSize)},
                        {percent(squareSize), percent(squareSize)},
                        fun,
                        color
                    );
                boardContainer->add(btn);
                boardButtons[i][j] = btn;
                
            }
        }
        boardContainer->setVisible(false);
        gui.add(boardContainer);

        promotionPanel = tgui::Panel::create({percent(squareSize), percent(squareSize * 4)});
        promotionPanel->setVisible(false);
        boardContainer->add(promotionPanel);
        for(int i = 0; i < 4; i++){
            tgui::Button::Ptr btn = createTguiBtn
                (
                    "", 
                    {percent(0), percent(squareSize * 2 * i)}, 
                    {percent(100), percent(25)}
                );
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
            int pieceId = boardView[index].getCurrentPieceId();
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
void UI::setBoardView(std::span<const Square> boardVieww){
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

void UI::drawPromotionOptions(Position from, Position to){
    std::cout << "called method drawPromotionOptions at position: x=" << to.x << " y="<< to.y <<std::endl;;
    game->promote(from, to, 'Q');

    std::vector<tgui::Widget::Ptr> widgets = promotionPanel->getWidgets();
    if(to.y == (whiteSide ? 0 : 7)){
        tgui::String x = whiteSide ? percent(to.x * squareSize) : percent((8 - to.x)*squareSize);
        std::cout<<"Calculeted x position:" << x << std::endl;
        promotionPanel->setPosition(x, "0%");
        char symb[4] = {'Q', 'R', 'B', 'N'};
        for(int i = 0; i < 4; i++){
            char s = symb[i];
            auto btn = std::static_pointer_cast<tgui::Button>(widgets[i]);
            btn->setPosition("0%", percent(squareSize * 2 * i));
            btn->setText(tgui::String(s));
            btn->getRenderer()->setBackgroundColor(sf::Color::Red);
            btn->onClick([this, from, to, s]() {
                game->promote(from, to, s);
                promotionPanel->setVisible(false);
            });
        }

    }
    else if((whiteSide && to.y == 7) || (!(whiteSide) && to.y == 0)){
       
    }
    promotionPanel->setVisible(true);
    promotionPanel->moveToFront();
    this->draw();
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

tgui::String UI::percent(float value) const{
    return tgui::String(value) + '%';
}

tgui::Button::Ptr UI::createTguiBtn(std::string label, std::pair<tgui::String, tgui::String> position, std::pair<tgui::String, tgui::String> size, std::optional<std::function<void()>> onClick, std::optional<sf::Color> color){
    tgui::Button::Ptr btn = tgui::Button::create(label);
    btn->setPosition(position.first, position.second);
    btn->setSize(size.first, size.second);
    if(onClick) btn->onClick(*onClick);
    if(color) btn->getRenderer()->setBackgroundColor(*color);
    return btn;
}



