#include "App.h"

App::App()
    :   window(sf::VideoMode({800, 600}), "Chess"),
        game(nullptr),
        ui(std::make_unique<UI>(window))
    {
        window.setFramerateLimit(144);
    }

void App::run(){
    while(window.isOpen()){
        while(auto event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            else{
                ui->handleEvent(event);
            }
        }
    window.clear();
    ui->drawMenu();
    ui->draw();
    window.display();
    }

    
}