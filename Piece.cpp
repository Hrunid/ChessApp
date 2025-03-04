#include "Piece.h"
#include "Board.h"

Piece::Piece(int id, char symbol, bool isWhite, Position currentPosition, Board& board, const std::vector<std::pair<int, int>>& moveDirections)
    :   id(id),
        symbol(symbol), 
        isWhite(isWhite),
        currentPosition(currentPosition),
        board(board),
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
        for(auto pin : board.getPins()){
            if(pin.pinnedPieceId == id){
                numOfPins++;
            }
        }
        if(numOfPins >= 2){
            availableMoves.clear();
        }
        else if(numOfPins == 1){
            std::pair<int, int> moveDir;
            for(auto pin : board.getPins()){
                if(pin.pinnedPieceId == id){
                    moveDir = pin.pinnedPieceDirection;
                }
            }
            findMovesInDirection(moveDir);
        }

    }
    else{
        for(std::pair direction : moveDirections){
            findMovesInDirection(direction);
        }
    }
}

void Piece::findMovesInDirection(std::pair<int, int> direction){
    int x = currentPosition.x;
    int y = currentPosition.y;

    int dx = direction.first;
    int dy = direction.second;

    while(true){

        x += dx;
        y += dy;
        
        Position tempPosition(x, y);

        if(board.isOnBoard(tempPosition)){
            if(board.isSquareEmpty(tempPosition)){
                availableMoves.push_back(tempPosition);
            }
            else{
                int tempPieceId = board.getPieceIdAtPosition(tempPosition);
                bool tempIsWhite = board.getPieceById(tempPieceId).isPieceWhite();
                if(tempIsWhite == isWhite){
                    seenBlockedSquares.push_back(tempPosition);
                    break;
                }
                else{
                    char tempPieceSymbol = board.getPieceById(tempPieceId).getSymbol();
                    availableMoves.push_back(tempPosition);

                    if(tempPieceSymbol == 'K'){
                        break;
                    }
                    if(!isPinning){
                        scanForPin(tempPosition, dx, dy);
                    }
                    break;
                    

                }
            }
        }
        else{
            break;
        }
    }
}

void Piece::scanForPin(Position startPosition, int dx, int dy){

    int x = startPosition.x;
    int y = startPosition.y;

    while(true){

        x += dx;
        y += dy;

        Position tempPosition(x, y);

        if(!board.isOnBoard(tempPosition)){
           break;
        }
        if(!board.isSquareEmpty(tempPosition)){

            int tempPieceId = board.getPieceIdAtPosition(tempPosition);
            bool tempIsWhite = board.getPieceById(tempPieceId).isPieceWhite();

            if(tempIsWhite == isWhite){
                break;
            } 
            else{

                char tempPieceSymbol = board.getPieceById(tempPieceId).getSymbol();
                int pieceToPinId = board.getPieceIdAtPosition(startPosition);
                if(tempPieceSymbol == 'K'){
                    
                    board.getPieceById(pieceToPinId).setPin(true);
                    isPinning = true;
                    std::pair<int, int> pinningPieceDirection = {dx, dy};
                    std::pair<int, int> pinnedPieceDirection = {-dx, -dy};
                    Pin newPin = {this->id, pieceToPinId, pinningPieceDirection, pinnedPieceDirection};
                    board.addPin(newPin);
                    board.getPieceById(pieceToPinId).calculateAvailableMoves();
                    

                }               
                break;                

            }
        }



   }

}

void Piece::updateMoves(Position unlockedSquare){
    std::cout<<"Aktualizacja ruchuów"<<std::endl;
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
    availableMoves.push_back(pos);
}
