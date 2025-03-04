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

        updatePieces(piecesWithAccesFrom);
        updatePieces(piecesWithAccesTo);

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

void Board::promotion(int id, char type){
   
    Position tempPosition(getPieceById(id).getPosition()); 
    bool tempIsWhie = getPieceById(id).
    allPieces[id] = nullptr;;
    capture(tempPosition);
    createPiece(id, type, tempPosition, *this);
}

void Board::castle(){
    
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
        dy = dy / std::abc(dy);
    }
    return std::make_pair(dx, dy);
    
}

const std::vector<Pin>& Board::getPins() const{
    return pins;
}

void Board::setUpPieces(){
    bool color = true;
    int id = 0;

    // Biały gracz

    // King

    createPiece(id, 'K', color, Position(4, 0), *this);
    id++;

    // Pawns

    for (int i = 0; i < 8; i++){
        createPiece(id, 'P', color, Position(i, 1), *this);
        id++;
    }

    // Bishops

    createPiece(id, 'B', color, Position(2, 0), *this);
    id++;
    createPiece(id, 'B', color, Position(5, 0), *this);
    id++;

    // Rooks

    createPiece(id, 'R', color, Position(0, 0), *this);
    id++;
    createPiece(id, 'R', color, Position(7, 0), *this);
    id++;

    // Knights

    createPiece(id, 'N', color, Position(1, 0), *this);
    id++;
    createPiece(id, 'N', color, Position(6, 0), *this);
    id++;

    // Queen

    createPiece(id, 'Q', color, Position(3, 0), *this);
    id++;

    // Czarny gracz
    color = false;

    // King

    createPiece(id, 'K', color, Position(4, 7), *this);
    id++;

    // Pawns

    for (int i = 0; i < 8; i++){
        createPiece(id, 'P', color, Position(i, 6), *this);
        id++;
    }

    // Bishops

    createPiece(id, 'B', color, Position(2, 7), *this);
    id++;
    createPiece(id, 'B', color, Position(5, 7), *this);
    id++;

    // Rooks

    createPiece(id, 'R', color, Position(0, 7), *this);
    id++;
    createPiece(id, 'R', color, Position(7, 7), *this);
    id++;

    // Knights

    createPiece(id, 'N', color, Position(1, 7), *this);
    id++;
    createPiece(id, 'N', color, Position(6, 7), *this);
    id++;

    // Queen

    createPiece(id, 'Q', color, Position(3, 7), *this);
    id++;
}

void Board::createPiece(int id, char type, bool isWhite, Position pos, Board& board){
    switch(type){
        case 'K':
            allPieces[id] = std::make_unique<King>(id, isWhite, pos, board);
            break;
        case 'Q':
            allPieces[id] = std::make_unique<Queen>(id, isWhite, pos, board);
            break;
        case 'R':
            allPieces[id] = std::make_unique<Rook>(id, isWhite, pos, board);
            break;
        case 'B':
            allPieces[id] = std::make_unique<Bishop>(id, isWhite, pos, board);
            break;
        case 'N':
            allPieces[id] = std::make_unique<Knight>(id, isWhite, pos, board);
            break;
        case 'P':
            allPieces[id] = std::make_unique<Pawn>(id, isWhite, pos, board);
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