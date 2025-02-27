#include "Board.h"

Board::Board(Player& whitePlayer, Player& blackPlayer)
    :   whitePlayer(whitePlayer),
        blackPlayer(blackPlayer)
        {
            createSquares();
            setUpPieces();

            for(int i = 0; i < 32; i++){
                allPieces[i]->calculateAvailableMoves();
                int id = allPieces[i]->getId();
                addPieceToSquares(id);
            }
        }

void Board::makeMove(const Move& move){

    Position from = move.from;
    Position to = move.to;

    if(!isSquareEmpty(to)){
        capture(to);
        }
    
        const std::vector<int>& piecesWithAccesFrom = getSquareAtPosition(from).getPiecesWithAcces();
        const std::vector<int>& piecesWithAccesTo = getSquareAtPosition(to).getPiecesWithAcces();

        int pieceId = getPieceIdAtPosition(from);

        getPieceById(pieceId).setPosition(to);

        getSquareAtPosition(from).setCurrentPiece(-1);
        getSquareAtPosition(to).setCurrentPiece(pieceId);
        

        for(int pieceId : piecesWithAccesFrom){
            removePieceFromSquares(pieceId);
        }
        for(int pieceId : piecesWithAccesTo){
            removePieceFromSquares(pieceId);
        }

        updatePieces(piecesWithAccesFrom);
        updatePieces(piecesWithAccesTo);

        
        for(int pieceId : piecesWithAccesFrom){
            addPieceToSquares(pieceId);
        }
        for(int pieceId : piecesWithAccesTo){
            addPieceToSquares(pieceId);
        }
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

void Board::updatePieces(std::vector<int>& piecesToUpdate){
    for(int pieceId : piecesToUpdate){
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

void Board::capture(Position pieceToCapturePosition){

    int pieceId = getPieceIdAtPosition(pieceToCapturePosition);
    bool isWhite = getPieceById(pieceId).isPieceWhite();

    if(isWhite){
        whitePlayer.removePlayerPiece(pieceId);
    }
    else{
        blackPlayer.removePlayerPiece(pieceId);
    }

    removePieceFromSquares(pieceId);
}

void Board::promotion(int id, char piece){

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

void Board::setUpPieces(){
    bool color = true;
    int id = 0;

    // Biały gracz

    // King

    allPieces[id] = std::make_unique<King>(id, color, Position(4, 0), *this);
    whitePlayer.addPlayerPiece(id);
    squares[4][0].setCurrentPiece(id);
    id++;

    // Pawns

    for (int i = 0; i < 8; i++){
        allPieces[id] = std::make_unique<Pawn>(id, color, Position(i, 1), *this);
        whitePlayer.addPlayerPiece(id);
        squares[i][1].setCurrentPiece(id);
        id++;
    }

    // Bishops

    allPieces[id] = std::make_unique<Bishop>(id, color, Position(2, 0), *this);
    whitePlayer.addPlayerPiece(id);
    squares[2][0].setCurrentPiece(id);
    id++;
    allPieces[id] = std::make_unique<Bishop>(id, color, Position(5, 0), *this);
    whitePlayer.addPlayerPiece(id);
    squares[5][0].setCurrentPiece(id);
    id++;

    // Rooks

    allPieces[id] = std::make_unique<Rook>(id, color, Position(0, 0), *this);
    whitePlayer.addPlayerPiece(id);
    squares[0][0].setCurrentPiece(id);
    id++;
    allPieces[id] = std::make_unique<Rook>(id, color, Position(7, 0), *this);
    whitePlayer.addPlayerPiece(id);
    squares[7][0].setCurrentPiece(id);
    id++;

    // Knights

    allPieces[id] = std::make_unique<Knight>(id, color, Position(1, 0), *this);
    whitePlayer.addPlayerPiece(id);
    squares[1][0].setCurrentPiece(id);
    id++;
    allPieces[id] = std::make_unique<Knight>(id, color, Position(6, 0), *this);
    whitePlayer.addPlayerPiece(id);
    squares[6][0].setCurrentPiece(id);
    id++;

    // Queen

    allPieces[id] = std::make_unique<Queen>(id, color, Position(3, 0), *this);
    whitePlayer.addPlayerPiece(id);
    squares[3][0].setCurrentPiece(id);
    id++;

    // Czarny gracz
    color = false;

    // King

    allPieces[id] = std::make_unique<King>(id, color, Position(4, 7), *this);
    blackPlayer.addPlayerPiece(id);
    squares[4][7].setCurrentPiece(id);
    id++;

    // Pawns

    for (int i = 0; i < 8; i++){
        allPieces[id] = std::make_unique<Pawn>(id, color, Position(i, 6), *this);
        blackPlayer.addPlayerPiece(id);
        squares[i][6].setCurrentPiece(id);
        id++;
    }

    // Bishops

    allPieces[id] = std::make_unique<Bishop>(id, color, Position(2, 7), *this);
    blackPlayer.addPlayerPiece(id);
    squares[2][7].setCurrentPiece(id);
    id++;
    allPieces[id] = std::make_unique<Bishop>(id, color, Position(5, 7), *this);
    blackPlayer.addPlayerPiece(id);
    squares[5][7].setCurrentPiece(id);
    id++;

    // Rooks

    allPieces[id] = std::make_unique<Rook>(id, color, Position(0, 7), *this);
    blackPlayer.addPlayerPiece(id);
    squares[0][7].setCurrentPiece(id);
    id++;
    allPieces[id] = std::make_unique<Rook>(id, color, Position(7, 7), *this);
    blackPlayer.addPlayerPiece(id);
    squares[7][7].setCurrentPiece(id);
    id++;

    // Knights

    allPieces[id] = std::make_unique<Knight>(id, color, Position(1, 7), *this);
    blackPlayer.addPlayerPiece(id);
    squares[1][7].setCurrentPiece(id);
    id++;
    allPieces[id] = std::make_unique<Knight>(id, color, Position(6, 7), *this);
    blackPlayer.addPlayerPiece(id);
    squares[6][7].setCurrentPiece(id);
    id++;

    // Queen

    allPieces[id] = std::make_unique<Queen>(id, color, Position(3, 7), *this);
    blackPlayer.addPlayerPiece(id);
    squares[3][7].setCurrentPiece(id);
}