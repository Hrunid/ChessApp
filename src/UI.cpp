#include "UI.hpp"
#include "App.hpp"
#include "Game.hpp"

UI::UI(sf::RenderWindow& wnd, App& app)
: window(wnd),
  gui(window),
  boardView(),
  winSize(window.getSize()),
  app(app),

  menuButtonW_Pct(0.30f),
  menuButtonH_Pct(0.08f),
  menuStartY_Pct(0.25f),
  menuSpacingY_Pct(0.03f),
  menuTitleHeight_Pct(0.10f),
  menuTitleTopMargin_Pct(0.05f)
{
    
    const unsigned BASE_W = 1280, BASE_H = 720;
    scale = std::min(winSize.x / static_cast<float>(BASE_W),winSize.y / static_cast<float>(BASE_H));                     
    boardContainer = tgui::Panel::create();
    gui.add(boardContainer);
}

/*void UI::onResize(const sf::Vector2u& newSize)
{
    winSize = newSize;
    // Ustaw view na pełne okno
    sf::FloatRect area(0, 0, winSize.x, winSize.y);
    window.setView(sf::View(area));

    // Przelicz scale
    const unsigned BASE_W = 1280, BASE_H = 720;
    scale = std::min(winSize.x / static_cast<float>(BASE_W),
                     winSize.y / static_cast<float>(BASE_H));

    // Odtwórz layout
    drawMenu();
}*/

void UI::handleEvent(const sf::Event& evt)
{
    // Przekaż event do TGUI
    gui.handleEvent(evt);
    // TODO: dodatkowa obsługa kliknięć w planszę lub inne
}

void UI::drawMenu()
{
    // 1) clear out any old widgets
    gui.removeAllWidgets();

    // 2) title
    auto title = tgui::Label::create("Szachy");
    title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    unsigned titleH = static_cast<unsigned>(winSize.y * menuTitleHeight_Pct * scale);
    title->setTextSize(titleH);
    title->setPosition(0, winSize.y * menuTitleTopMargin_Pct * scale);
    title->setSize(winSize.x, titleH);
    gui.add(title);

    // 3) buttons
    unsigned btnW    = static_cast<unsigned>(winSize.x * menuButtonW_Pct * scale);
    unsigned btnH    = static_cast<unsigned>(winSize.y * menuButtonH_Pct * scale);
    int      posX    = static_cast<int>((winSize.x - btnW) / 2);
    unsigned posY    = static_cast<unsigned>(winSize.y * menuStartY_Pct * scale);
    unsigned spacing = static_cast<unsigned>(winSize.y * menuSpacingY_Pct * scale);

    const std::vector<std::string> labels = {"Nowa gra", "Wczytaj", "Ustawienia", "Wyjście"};
    for (size_t i = 0; i < labels.size(); ++i) {
        auto btn = tgui::Button::create(labels[i]);
        btn->setSize(btnW, btnH);
        btn->setPosition(posX, posY + static_cast<int>(i) * (btnH + spacing));
        gui.add(btn);
        if (i == 0) {
            btn->onPress([this]() {
                app.startNewGame(TwoPlayers);
            });
        }
    }


}

void UI::draw(){
    gui.draw();
}

void UI::drawGameSettings(){}
void UI::UI::drawEndGamePanel(){}

void UI::drawBoard()
{
    // 0) Usuń wszystkie widgety z root GUI (czyścimy menu itp.)
    gui.removeAllWidgets();

    // 1) Dodaj panel planszy z powrotem do GUI
    gui.add(boardContainer);

    // 2) Wyczyść jego wewnętrzne widgety (przyciski pól)
    boardContainer->removeAllWidgets();

    // 3) Oblicz rozmiar i pozycję planszy (70% okna, wyśrodkowana)
    const auto winSize = window.getSize();
    const float boardW = winSize.x * 0.7f;
    const float boardH = winSize.y * 0.7f;

    boardContainer->setSize({ boardW, boardH });
    boardContainer->setPosition({
        (winSize.x - boardW) / 2.f,
        (winSize.y - boardH) / 2.f
    });

    // 4) Rozmiar pojedynczego pola
    const float squareSize = boardW / 8.f;

    // 5) Tworzenie 8×8 przycisków
    for (unsigned row = 0; row < 8; ++row)
    {
        for (unsigned col = 0; col < 8; ++col)
        {
            auto button = tgui::Button::create();
            button->setSize({ squareSize, squareSize });
            // A1 (row=0,col=0) -> lewy dolny: y = (7-row)
            button->setPosition({
                col * squareSize,
                (7u - row) * squareSize
            });

            // kolor pola
            bool light = ((row + col) % 2 == 0);
            button->getRenderer()->setBackgroundColor(
                light ? sf::Color::White : sf::Color::Black
            );

            // wyświetl ID figury (0 = puste)
            int pieceId = boardView[row * 8 + col]->getCurrentPieceId();
            if (pieceId != 0)
                button->setText(std::to_string(pieceId));

            boardContainer->add(button);
        }
    }
}

void UI::drawSidePanel(){}
void UI::drawAccessibleSquares(std::vector<Position> squares){}
void UI::onResize(const sf::Vector2u& newSize){}

void UI::setBoardView(std::span<const std::unique_ptr<Square>> boardView){
    this->boardView = boardView;
}

