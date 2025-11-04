#ifndef RESOURCE_MANGER_H
#define RESOURCE_MANGER_H

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include <array>
#include <string>
#include <iostream>
#include <filesystem>

namespace resources{

enum Color {B, W};
enum Piece {King, Queen, Rook, Bishop, Knight, Pawn};
enum Sets {Basic, Count};
enum SquareColor{Dark, Light};

class ResourceManager{
private:
    std::string assetsPath;
    std::array<std::array<std::array<tgui::Texture, 6>, 2>, Sets::Count> pieceSets;
    std::array<std::array<sf::Color, 2>, 1> squareColors;

    tgui::Texture moveEmpty;
    tgui::Texture moveOccupied;

    void loadPieceSets();
    void loadColors();
    void loadMoveTextures();

public:
    ResourceManager(const std::filesystem::path& exePath);
    const tgui::Texture& getPieceTexture(Sets set, Color color, Piece piece);
    const tgui::Texture& getMoveTexture(bool empty);

};

}

#endif