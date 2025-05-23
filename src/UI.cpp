#include "UI.h"

UI::UI(sf::RenderWindow& wnd)
: window(wnd)
, gui(window)
, winSize(window.getSize())
, // ustawienia layoutu menu
  menuButtonW_Pct(0.30f),
  menuButtonH_Pct(0.08f),
  menuStartY_Pct(0.25f),
  menuSpacingY_Pct(0.03f),
  menuTitleHeight_Pct(0.10f),
  menuTitleTopMargin_Pct(0.05f)
{
    // Oblicz początkowy mnożnik skalowania (opcjonalnie)
    const unsigned BASE_W = 1280, BASE_H = 720;
    scale = std::min(winSize.x / static_cast<float>(BASE_W),
                     winSize.y / static_cast<float>(BASE_H));
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
    gui.removeAllWidgets();

    // Tytuł aplikacji
    auto title = tgui::Label::create("Szachy");
    title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    unsigned titleH = static_cast<unsigned>(winSize.y * menuTitleHeight_Pct * scale);
    title->setTextSize(titleH);
    title->setPosition(0, winSize.y * menuTitleTopMargin_Pct * scale);
    title->setSize(winSize.x, titleH);
    gui.add(title);

    // Przyciski menu
    unsigned btnW   = static_cast<unsigned>(winSize.x * menuButtonW_Pct * scale);
    unsigned btnH   = static_cast<unsigned>(winSize.y * menuButtonH_Pct * scale);
    int      posX   = static_cast<int>((winSize.x - btnW) / 2);
    unsigned posY   = static_cast<unsigned>(winSize.y * menuStartY_Pct * scale);
    unsigned spacing= static_cast<unsigned>(winSize.y * menuSpacingY_Pct * scale);

    const std::vector<std::string> labels = {"Nowa gra", "Wczytaj", "Ustawienia", "Wyjście"};
    for (size_t i = 0; i < labels.size(); ++i) {
        auto btn = tgui::Button::create(labels[i]);
        btn->setSize(btnW, btnH);
        btn->setPosition(posX, posY + static_cast<int>(i) * (btnH + spacing));
        gui.add(btn);
    }
}

void UI::draw(){
    gui.draw();
}