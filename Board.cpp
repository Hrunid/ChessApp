#include "Board.h"
#include "King.h"
#include "Bishop.h"
#include "Queen.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"

Board::Board(Player& whitePlayer, Player& blackPlayer)
    :   whitePlayer(whitePlayer),
        blackPlayer(blackPlayer)
        {

            Piece::setBoardPtr(this);

            createSquares();
            setUpPieces();

            for(int i = 0; i < 32; i++){
                allPieces[i]->calculateAvailableMoves();
                int id = allPieces[i]->getId();
                addPieceToSquares(id);
            }
        }

void Board::makeMove(const Move& move){

    const Position from = move.getPositionFrom();
    Position to = move.getPositionTo();

    int movedPieceId = getPieceIdAtPosition(from);
    Position tempPositionToUpdate(-1, -1);                        //in case of need to remeber another position as in case with castle
    if(move.capture()){
        capture(to);
    }
    else if(move.castle()){
        tempPositionToUpdate = castle(from, to);
    }
    else if(move.enPassant()){
        enPassant(from, to);
    }
    else if(move.promotion()){
        promotion(movedPieceId, move.getPromotionPieceSymbol());
    }

    movePiece(from ,to);
    if(tempPositionToUpdate.x != -1 && tempPositionToUpdate.y != -1){
        updatePiecesAtPosition(tempPositionToUpdate);
    }
    updatePiecesAtPosition(from);
    updatePiecesAtPosition(to);
    

}

void Board::updatePiecesAtPosition(Position pos){
    const std::vector<int>& piecesWithAccess = getSquareAtPosition(pos).getPiecesWithAcces();
    updatePieces(piecesWithAccess);

}

bool Board::isSquareEmpty(Position pos){
    int pieceIdAtPosition = getSquareAtPosition(pos).getCurrentPieceId();
    if(pieceIdAtPosition == -1){
        return true;
    }
    else{
        return false;
    }
}

Piece& Board::getPieceById(int id){
    return *allPieces[id];
}

int Board::getPieceIdAtPosition(Position pos){
    return getSquareAtPosition(pos).getCurrentPieceId();
}



void Board::createSquares(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            squares[i][j] = std::make_unique<Square>(-1);
        }
    }
}

void Board::updatePieces(const std::vector<int>& piecesToUpdate){
    for(int pieceId : piecesToUpdate){
        removePieceFromSquares(pieceId);
        allPieces[pieceId]->calculateAvailableMoves();
        addPieceToSquares(pieceId);
    }
    
}

bool Board::isPinCurrent(Pin pin){
    int pinning = pin.pinningPieceId;
    int pinned = pin.pinnedPieceId;

    Position startPosition = getPieceById(pinning).getPosition();
    int x = startPosition.x;
    int y = startPosition.y;
    int dx = pin.pinningPieceDirection.first;
    int dy = pin.pinningPieceDirection.second;

    while(true){
        x += dx;
        y += dy;

        Position tempPosition(x, y);

        if(!isOnBoard(tempPosition)){
            break;
        }
        else if(!isSquareEmpty(tempPosition)){
            int tempPieceId = getPieceIdAtPosition(tempPosition);
            if(tempPieceId == pinned){
                return true;
            }
            else{
                return false;
            }
        }
           
    }
    return false;

}

Square& Board::getSquareAtPosition(Position pos){
    int x = pos.x;
    int y = pos.y;
    return *squares[x][y];
}

void Board::removePieceFromSquares(int pieceId){
    const std::vector<Position>& pieceSquares = getPieceById(pieceId).getAvailableMoves();
    for(auto position : pieceSquares){
        getSquareAtPosition(position).removeAttacker(pieceId);
    }
    const std::vector<Position>& seenBlockedSquares = getPieceById(pieceId).getSeenBlockedSquares();
    for(auto position : seenBlockedSquares){
        getSquareAtPosition(position).removeAttacker(pieceId);
    }
}

void Board::addPieceToSquares(int pieceId){
    const std::vector<Position>& pieceSquares = getPieceById(pieceId).getAvailableMoves();
    for(auto position : pieceSquares){
        getSquareAtPosition(position).addAttacker(pieceId);
    }
    const std::vector<Position>& seenBlockedSquares = getPieceById(pieceId).getSeenBlockedSquares();
    for(auto position : seenBlockedSquares){
        getSquareAtPosition(position).addAttacker(pieceId);
    }
}

void Board::movePiece(Position from, Position to){
    int pieceToMoveId = getPieceIdAtPosition(from);
    allPieces[pieceToMoveId]->setPosition(to);
    getSquareAtPosition(from).setCurrentPiece(-1);
}

void Board::capture(Position pieceToCapturePosition){

    int pieceId = getPieceIdAtPosition(pieceToCapturePosition);
    bool isWhite = getPieceById(pieceId).isPieceWhite();

    if(isWhite){
        whitePlayer.removePlayerPiece(pieceId);
    }
    else{
        blackPlayer.removePlayerPiece(pieceId);
    }

    getSquareAtPosition(pieceToCapturePosition).setCurrentPiece(-1);
    removePieceFromSquares(pieceId);
    allPieces[pieceId] = nullptr;

}

void Board::promotion(int id, char type){
   
    Position tempPosition(getPieceById(id).getPosition()); 
    bool tempIsWhie = getPieceById(id).isPieceWhite();
    capture(tempPosition);
    createPiece(id, type, tempIsWhie,tempPosition);
}

Position Board::castle(Position from, Position to){
    int xDiff = from.x - to.x;
    int dx;
    Position rookPosition(0, from.y);
    if(xDiff > 0){
        rookPosition.x = 0; 
        dx = -1;
    }
    else{
        rookPosition.x = 7;
        dx = 1;
    }
    Position newRookPosition(from.x + dx, from.y);
    movePiece(rookPosition, newRookPosition);
    return newRookPosition;

}

Position Board::enPassant(Position from, Position to){
    int xDiff = from.x - to.x;
    int dx;
    if(xDiff > 0){
        dx = -1;
    }
    else{
        dx = 1;
    }
    Position tempPosition(from.x + dx, from.y);
    capture(tempPosition);
    return tempPosition;
}

bool Board::canPlayerCastle(bool isWhite){
    if(isWhite){
        return whitePlayer.canPlayerCastle();
    }
    else{
        return blackPlayer.canPlayerCastle();
    }
}

void Board::updatePins(){
    for(int i = pins.size() - 1; i >= 0; i--){
        Pin tempPin = pins[i];
        if(!isPinCurrent(tempPin)){
            int pinningPieceId = tempPin.pinningPieceId;
            int pinnedPieceId = tempPin.pinnedPieceId;

            allPieces[pinningPieceId]->setPinningStatus(false);
            allPieces[pinnedPieceId]->setPin(false);
            allPieces[pinnedPieceId]->calculateAvailableMoves();

            pins.erase(pins.begin() + i);
            

            
        }
    }
}

void Board::addPin(Pin newPin){
    pins.push_back(newPin);
}

bool Board::isOnBoard(Position pos){
    int x = pos.x;
    int y = pos.y;

    if(x < 0 || x > 7 || y < 0 || y > 7){
        return false;
    }
    
    return true;
}

std::pair<int, int> Board::calculateDirection(Position from, Position to){
    int x1 = from.x;
    int y1 = from.y;
    int x2 = to.x;
    int y2 = to.y;

    int dx = x2 - x1;
    int dy = y2 - y1;
    if(dx != 0){
        dx = dx / std::abs(dx);
    }
    if(dy != 0){
        dy = dy / std::abs(dy);
    }
    return std::make_pair(dx, dy);
    
}

const std::vector<Pin>& Board::getPins() const{
    return pins;
}

std::string Board::convertToFEN(){
    std::string FEN = "";
    int emptySquaresCount;
    for(int i = 7; i >= 0; i--){
        emptySquaresCount = 0;
        for(int j = 0; j <= 7; j++){
            if(isSquareEmpty(Position(j, i))){
                emptySquaresCount++;
            }
            else if(emptySquaresCount != 0){
                FEN += std::to_string(emptySquaresCount);
                emptySquaresCount = 0;
            }
            else{
                emptySquaresCount = 0;
                int tempId = getPieceIdAtPosition(Position(j, i));
                bool tempIsWhite = allPieces[tempId]->isPieceWhite();
                char tempPieceSymbol = allPieces[tempId]->getSymbol();
                if(!tempIsWhite){
                    tempPieceSymbol = std::tolower(tempPieceSymbol);
                }
                FEN += tempPieceSymbol;
            }
            
        }
        if(emptySquaresCount != 0){
                FEN += std::to_string(emptySquaresCount);
            }
        FEN += '/';
    }
    FEN.pop_back();
    return  FEN;
}

void Board::setUpPieces(){
    bool color = true;
    int id = 0;

    // Biały gracz


    createPiece(id, 'K', color, Position(4, 0));
    id++;

    for (int i = 0; i < 8; i++){
        createPiece(id, 'P', color, Position(i, 1));
        id++;
    }

    createPiece(id, 'B', color, Position(2, 0));
    id++;
    createPiece(id, 'B', color, Position(5, 0));
    id++;

    createPiece(id, 'R', color, Position(0, 0));
    id++;
    createPiece(id, 'R', color, Position(7, 0));
    id++;

    createPiece(id, 'N', color, Position(1, 0));
    id++;
    createPiece(id, 'N', color, Position(6, 0));
    id++;

    createPiece(id, 'Q', color, Position(3, 0));
    id++;

    // Czarny gracz
    color = false;


    createPiece(id, 'K', color, Position(4, 7));
    id++;

    for (int i = 0; i < 8; i++){
        createPiece(id, 'P', color, Position(i, 6));
        id++;
    }

    createPiece(id, 'B', color, Position(2, 7));
    id++;
    createPiece(id, 'B', color, Position(5, 7));
    id++;

    createPiece(id, 'R', color, Position(0, 7));
    id++;
    createPiece(id, 'R', color, Position(7, 7));
    id++;


    createPiece(id, 'N', color, Position(1, 7));
    id++;
    createPiece(id, 'N', color, Position(6, 7));
    id++;

    createPiece(id, 'Q', color, Position(3, 7));
    id++;
}

void Board::createPiece(int id, char type, bool isWhite, Position pos){
    switch(type){
        case 'K':
            allPieces[id] = std::make_unique<King>(id, isWhite, pos);
            break;
        case 'Q':
            allPieces[id] = std::make_unique<Queen>(id, isWhite, pos);
            break;
        case 'R':
            allPieces[id] = std::make_unique<Rook>(id, isWhite, pos);
            break;
        case 'B':
            allPieces[id] = std::make_unique<Bishop>(id, isWhite, pos);
            break;
        case 'N':
            allPieces[id] = std::make_unique<Knight>(id, isWhite, pos);
            break;
        case 'P':
            allPieces[id] = std::make_unique<Pawn>(id, isWhite, pos);
            break;
        default:
            throw std::invalid_argument("Bledny symbol figury!");
            break;
    }

    if(isWhite){
        whitePlayer.addPlayerPiece(id);

    }
    else{
        blackPlayer.addPlayerPiece(id);
    }

    squares[pos.x][pos.y]->setCurrentPiece(id);
}