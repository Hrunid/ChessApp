#include "UI.h"
#include "App.h"
#include "Game.h"

UI::UI(tgui::Gui& guiReff, App& appReff, resources::ResourceManager& rmReff)
    : gui(guiReff),
    app(appReff),
    rm(rmReff),
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
    sidePanelW(43.75f),
    promotionPanelH(50.0f),
    promotionPanelW(12.5f),
    endGamePanelH(40.0f),
    endGamePanelW(60.0f)

    {
        menu = tgui::Group::create({percent(mainContainerSize), percent(mainContainerSize)});
        menu->setPosition("0%", "0%");

        tgui::Picture::Ptr background = tgui::Picture::create("C:/Users/wiktw/OneDrive/Dokumenty/c++/SzachyZId/ChessApp/res/tlo.jpg");
        background->setSize(percent(mainContainerSize), percent(mainContainerSize));
        menu->add(background);
        background->moveToBack();

        menuButtons = tgui::Panel::create({percent(menuButtonW), percent(menuButtonH)});
        menuButtons->setPosition("20%", "30%");
        std::function<void()> functions[4] = {
            [this](){ this->app.startNewGame(TwoPlayers);},
            [this](){ },
            [this](){ },
            [this](){ }
        };
        std::string labels[4] = {"Nowa gra", "Wczytaj", "Ustawienia", "Wyjscie"};

        for(int i = 0; i < 4; i++){
            auto btn = createTguiBtn(labels[i], {percent(0), percent(25*i)}, {"100%", percent(25)}, functions[i]);
            menuButtons->add(btn);
        }
        menuButtons->getRenderer()->setTransparentTexture(true);
        menu->add(menuButtons);
        gui.add(menu);


        boardContainer = createBoardContainer();
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
void UI::drawEndGamePanel(std::string res, int player){
    auto lb = endGamePanel->get<tgui::Label>("res");
    lb->setText(tgui::String(res));
    endGamePanel->moveToFront();
    endGamePanel->setVisible(true);
    gui.draw();
}
void UI::drawBoard()
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int index = getMappedIndex(i, j);
            int pieceId = boardView[index].getCurrentPieceId();
            Position mapped = mapPosition(Position(i, j));
            if(pieceId != -1){
                auto pieceRef = game->getPieceById(pieceId);
                auto pict = boardSquares[mapped.x][mapped.y]->get<tgui::Picture>("piece");
                resources::Piece piece = pieceSymbMap[pieceRef.getSymbol()];
                resources::Color color = pieceRef.isPieceWhite() ? resources::W : resources::B;
                pict->getRenderer()->setTexture(rm.getPieceTexture(resources::Sets::Basic, color, piece));
                pict->setVisible(true);
                
            }
            else{
                auto pict = boardSquares[mapped.x][mapped.y]->get<tgui::Picture>("piece");
                pict->setVisible(false);
            }

        }
    }
    this->draw();
}

void UI::drawSidePanel(){}
void UI::drawMoves(const std::vector<Position>& squares){

    for(Position pos : squares){
        int index = getMappedIndex(pos.x, pos.y);
        Position mapped = mapPosition(pos);
        if(boardView[index].empty()){
            auto move = boardSquares[mapped.x][mapped.y]->get<tgui::Picture>("dot");
            move->setVisible(true);
            move->moveToFront();
        }
        else{
            auto move = boardSquares[mapped.x][mapped.y]->get<tgui::Picture>("circle");
            move->setVisible(true);
            move->moveToFront();
        }
    }
    this->draw();
}
void UI::hideMoves(const std::vector<Position>& squares){
        for(Position pos : squares){
        int index = getMappedIndex(pos.x, pos.y);
        Position mapped = mapPosition(pos);

        auto move = boardSquares[mapped.x][mapped.y]->get<tgui::Picture>("dot");
        move->setVisible(false);
        move->moveToBack();

        auto move1 = boardSquares[mapped.x][mapped.y]->get<tgui::Picture>("circle");
        move1->setVisible(false);
        move1->moveToBack();
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

    Position mappedPos = mapPosition(to);
    promotionPanel->setPosition({percent(mappedPos.x * squareSize), percent((mappedPos.y == 0) ? 0 : 50)});
    std::vector<char> symbs = {'Q', 'R', 'B', 'N'};
    if(mappedPos.y == 7) std::reverse(symbs.begin(), symbs.end());
    for(int i = 0; i < symbs.size(); i++){
        char s = symbs[i];
        std::string btnName = std::string(1, s) + "btn";
        tgui::Button::Ptr btn = promotionPanel->get<tgui::Button>(btnName);
        btn->onClick([this, from, to, s](){
            game->promote(from, to, s);
            promotionPanel->setVisible(false);
        });
        btn->setPosition({percent(0), percent(i * 25)});

        btn->setVisible(true);

        std::string imgName = std::string(1, s) + "img";
        resources::Color color = (to.y == 0) ? resources::W : resources::B;
        tgui::Picture::Ptr img = promotionPanel->get<tgui::Picture>(imgName);
        img->getRenderer()->setTexture(rm.getPieceTexture(resources::Basic, color, pieceSymbMap[s]));
        img->setPosition(percent(10), percent(i * 25 + 2.5f));
        img->setSize({percent(80), percent(20)});

        img->setVisible(true);
        img->setIgnoreMouseEvents(true);
        img->moveToFront();

    }


    promotionPanel->moveToFront();
    promotionPanel->setVisible(true);
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

tgui::Container::Ptr UI::createBoardContainer(){

    tgui::Container::Ptr board = tgui::Panel::create({percent(boardContainerW), percent(boardContainerH)});
    board->setPosition(0, 0);

    for(int i = 0; i < 8; i++){
        for(int j = 7; j >= 0; j--){

            std::function func = [this, i, j]() {
                Position logicalPos = mapPosition(Position(i, j));
                promotionPanel->setVisible(false);
                game->processClick(logicalPos);
                
            };

            bool light = (i + j) % 2 == 0;
            sf::Color color = light ? lightSquare : darkSquare;

            tgui::Button::Ptr btn = createTguiBtn("", {percent(0), percent(0)}, {percent(100), percent(100)}, func, color);
            auto file = createSquareContainer({percent(squareSize * i), percent(squareSize * j)}, btn);

            boardSquares[i][j] = file;
            board->add(file);
       }
    } 

    promotionPanel = tgui::Group::create({percent(promotionPanelW), percent(promotionPanelH)});
    board->add(promotionPanel, "promPanel");

    char symbs[4] = {'Q', 'R', 'B', 'N'};
    for(int i = 0; i < 4; i++){
        auto btn = createTguiBtn("", {percent(0), percent(i * 25)}, {percent(100), percent(25)});
        std::string name = std::string(1, symbs[i]) + "btn";
        promotionPanel->add(btn, name);

        tgui::Picture::Ptr img = tgui::Picture::create();
        name = std::string(1, symbs[i]) + "img";
        promotionPanel->add(img, name);
    }    
    promotionPanel->setVisible(false);
    promotionPanel->moveToFront();

    endGamePanel = tgui::Panel::create({percent(endGamePanelW), percent(endGamePanelH)});
    endGamePanel->setVisible(false);
    endGamePanel->setPosition({percent(20), percent(30)});

    tgui::Label::Ptr res = tgui::Label::create();
    endGamePanel->add(res, "res");
    res->setPosition({percent(20), percent(10)});
    res->setSize({percent(60), percent(30)});

    tgui::Button::Ptr b2Menu = createTguiBtn(
        "Return to main menu", 
        {percent(20), percent(60)}, 
        {percent(60), percent(30)}, 
        [this](){
            boardContainer->setVisible(false);
            menu->setVisible(true);
            app.setAppState(MainMenu);
            endGamePanel->setVisible(false);
            gui.draw();
        }
    );
    endGamePanel->add(b2Menu);

    board->add(endGamePanel);
    
    board->setVisible(false);
    return board;
}

tgui::Container::Ptr UI::createSquareContainer(std::pair<tgui::String, tgui::String> position, tgui::Button::Ptr btn){
    tgui::Container::Ptr file = tgui::Group::create({percent(squareSize), percent(squareSize)});
    file->setPosition(position.first, position.second);
    file->add(btn, "btn");
    tgui::Picture::Ptr dot = tgui::Picture::create(rm.getMoveTexture(true));
    file->add(dot, "dot");
    dot->setPosition(percent(0), percent(0));
    dot->setSize(percent(100), percent(100));
    dot->setIgnoreMouseEvents(true);
    dot->setVisible(false);

    tgui::Picture::Ptr circle = tgui::Picture::create(rm.getMoveTexture(false));
    file->add(circle, "circle");
    circle->setPosition(percent(0), percent(0));
    circle->setSize(percent(100), percent(100));
    circle->setIgnoreMouseEvents(true);
    circle->setVisible(false);

    tgui::Picture::Ptr pieceImg = tgui::Picture::create();
    file->add(pieceImg, "piece");
    pieceImg->setPosition(percent(10), percent(10));
    pieceImg->setSize(percent(80), percent(80));
    pieceImg->setIgnoreMouseEvents(true);
    pieceImg->setVisible(false);
    
    pieceImg->moveToFront();

    
    return file;

}



