#include "Piece.h"
#include "Board.h"

#include <algorithm>

Piece::Piece(int id, char symbol, bool isWhite, Position currentPosition, std::span<const std::pair<int, int>> moveDirections)
    :   id(id),
        symbol(symbol), 
        isWhite(isWhite),
        currentPosition(currentPosition),
        moveDirections(moveDirections),
        isPinning(false),
        isPinned(false),
        hasMoved(false),
        availableMoves(),
        seenBlockedSquares()
        {}

        
void Piece::calculateAvailableMoves(){

    availableMoves.clear();
    seenBlockedSquares.clear();

    if(isPinned){
        int numOfPins = 0;
        for(const auto& pin : board->getPins()){
            if(pin.pinnedPieceId == id){
                numOfPins++;
            }
        }
        if(numOfPins >= 2){
            availableMoves.clear();
            for(auto dir : moveDirections){
                scanForPin(dir);
            }
            return;
        }
        else if(numOfPins == 1){
            std::pair<int, int> moveDir;
            for(auto pin : board->getPins()){
                if(pin.pinnedPieceId == id){
                    moveDir = pin.pinnedPieceDirection;
                }
            }
            for(auto dir : moveDirections){
                scanForPin(dir);
            }
            if(std::find(moveDirections.begin(), moveDirections.end(), moveDir) != moveDirections.end()){
                findMovesInDirection(moveDir);
            }

        }
    }
    else{
        for(const auto& direction : moveDirections){
            findMovesInDirection(direction);
        }
    }
}

void Piece::findMovesInDirection(std::pair<int, int> direction){
    Position tempPosition = currentPosition;

    while(true){
        tempPosition += direction;

        if(!board->isOnBoard(tempPosition)) break;

        auto foundPiece = board->getPieceAtPosition(tempPosition);

        if(!foundPiece.has_value()){
            availableMoves.emplace_back(tempPosition);
        }
        else{
            Piece& tempPiece = foundPiece->get();
            if(tempPiece.isPieceWhite() == isWhite){
                seenBlockedSquares.emplace_back(tempPosition);
                break;
            }
            else{
                availableMoves.emplace_back(tempPosition);
                if(!isPinning && tempPiece.getSymbol() != 'K'){
                    scanForPin(direction, tempPosition);
                }
                
            }
            break;
        }
    }
}

void Piece::scanForPin(std::pair<int, int> dir, std::optional<Position> startPosition){

    if(!startPosition.has_value() && !isPinning){
        Position tempPosition = currentPosition;
        while(true){
            tempPosition += dir;
            if(!board->isOnBoard(tempPosition)) break;

            auto foundPiece = board->getPieceAtPosition(tempPosition);
            if(!foundPiece.has_value()) continue;

            Piece& tempPiece = foundPiece->get();
            if(isWhite != tempPiece.isPieceWhite() && tempPiece.getSymbol() != 'K'){
                scanForPin(dir, tempPosition);
            }
            break;
        }
    }
    else{
        Position tempPosition = *startPosition;

        while(true){
            tempPosition += dir;

            if(!board->isOnBoard(tempPosition)) break;

            auto temp = board->getPieceAtPosition(tempPosition);
            if(!temp.has_value()) continue;

            Piece& foundPiece = temp->get();

            if(foundPiece.isPieceWhite() == isWhite) break;

            if(foundPiece.getSymbol() == 'K'){
                int pieceToPinId = board->getPieceIdAtPosition(*startPosition);
                board->getPieceById(pieceToPinId).setPin(true);
                isPinning = true;
                std::pair<int, int> pinningPieceDirection = dir;
                std::pair<int, int> pinnedPieceDirection = {-dir.first, -dir.second};
                Pin newPin = {this->id, pieceToPinId, pinningPieceDirection, pinnedPieceDirection};
                board->addPin(newPin);
                board->updatePiece(pieceToPinId);
            }
            break;
        }   
    }
}

void Piece::updateMoves(Position unlockedSquare){
}

int Piece::getId() const{
    return id;
}

char Piece::getSymbol() const{
    return symbol;
}

Position Piece::getPosition() const{
    return currentPosition;
}

bool Piece::isPieceWhite() const{
    return isWhite;
}

bool Piece::isPiecePinned() const{
    return isPinned;
}

bool Piece::hasPieceMoved() const{
    return hasMoved;
}

const std::vector<Position>& Piece::getAvailableMoves() const{
    return availableMoves;
}

const std::vector<Position>& Piece::getSeenBlockedSquares() const{
    return seenBlockedSquares;
}

bool Piece::isMoveAvailable(Position pos) const{
    return std::find(availableMoves.begin(), availableMoves.end(), pos) != availableMoves.end();
}
void Piece::setBoardPtr(Board* b){
    board = b;
}

void Piece::setPosition(Position newPosition){
    currentPosition = newPosition;
}

void Piece::setPinningStatus(bool pins){
    isPinning = pins;
}

void Piece::setPin(bool pin){
    isPinned = pin;
}

void Piece::pieceHasMoved(){
    hasMoved = true;
}

void Piece::clearMoves(){
    availableMoves.clear();
}

void Piece::addMove(Position pos){
    availableMoves.emplace_back(pos);
}
void Piece::removeMove(Position pos){
    availableMoves.erase(std::remove(availableMoves.begin(), availableMoves.end(), pos), availableMoves.end());
}

std::span<const std::pair<int, int>> Piece::getMoveDirections() const{
    return moveDirections;
}

void Piece::doubleCheck(){
    seenBlockedSquares.insert(seenBlockedSquares.end(), availableMoves.begin(), availableMoves.end());
    availableMoves.clear();
}
