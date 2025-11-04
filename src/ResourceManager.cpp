#include "ResourceManager.h"

using namespace resources;

ResourceManager::ResourceManager(const std::filesystem::path& exeDir)
    :   assetsPath{exeDir.parent_path().string() + "\\res"}
    {
        loadPieceSets();
        moveEmpty.load(assetsPath + "\\kropka.png");
        moveOccupied.load(assetsPath + "\\okrag.png");
    }

void ResourceManager::loadPieceSets(){
    for(int set = 0; set < Sets::Count; set++){
        for(int color = 0; color < 2; color++){
            for(int piece = 0; piece < 6; piece++){

                std::string col = color ? "\\W" : "\\B";
                std::string path{assetsPath};
                std::string name;
                switch (piece)
                {
                case King:
                    name = "\\king";
                    break;

                case Queen:
                    name = "\\queen";
                    break;

                case Rook:
                    name = "\\rook";
                    break;

                case Bishop:
                    name = "\\bishop";
                    break;

                case Knight:
                    name = "\\knight";
                    break;

                case Pawn:
                    name = "\\pawn";
                    break;

                default:
                    break;
                }
                path += "\\sets\\" + std::to_string(set) + col + name + ".png";
                
                pieceSets[set][color][piece].load(path);
            }
        }
    }
}

const tgui::Texture& ResourceManager::getPieceTexture(Sets set, Color color, Piece piece){
    return pieceSets[set][color][piece];
}
const tgui::Texture& ResourceManager::getMoveTexture(bool empty){
    if(empty) return moveEmpty;
    else return moveOccupied;
}
