#include "Board.hpp"
#include "King.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"

#include <random>
#include <limits>
#include <span>
#include <iostream>
#include <optional>
#include <exception>
#include <ranges>

Board::Board(Player* whitePlayer, Player* blackPlayer, const std::vector<Move>& moveHis)
    :   pins(),
        whitePlayer(whitePlayer),
        blackPlayer(blackPlayer),
        moveHistory(moveHis),
        squares{}

{
            
    Piece::setBoardPtr(this);

    setUpPieces();
    for(int i = 0; i < 32; i++){
        allPieces[i]->calculateAvailableMoves();
        addPieceToSquares(i);
    }
    generateRandNumbers();
}

Board::~Board(){
    whitePlayer = nullptr;
    blackPlayer = nullptr;
}

void Board::makeMove(const Move& move, int player){

    const Position from = move.getPositionFrom();
    Position to = move.getPositionTo();

    int movedPieceId = getPieceIdAtPosition(from);
    std::optional<Position> tempPositionToUpdate;                  
    if(move.capture()){
        capture(to);
    }
    else if(move.castle()){
        tempPositionToUpdate = castle(from, to);
    }
    else if(move.enPassant()){
        enPassant(from, to);
    }
    if(move.promotion()){
        promotion(movedPieceId, move.getPromotionPieceSymbol());
        tempPositionToUpdate = to;
    }
    std::vector<int> accessFrom = getSquareAtPosition(from).getPiecesWithAcces();
    std::vector<int> accessTo = getSquareAtPosition(to).getPiecesWithAcces();

    movePiece(from ,to);
    if (tempPositionToUpdate.has_value() && isOnBoard(*tempPositionToUpdate)) {
        updatePiecesAtPosition(*tempPositionToUpdate);
    }
    
    updatePieces(accessFrom);
    updatePieces(accessTo);
    Player* p = player == 0 ? whitePlayer : blackPlayer;
    updatePieces({p->getKingId()});
}

void Board::updatePiecesAtPosition(Position pos){
    const std::vector<int>& piecesWithAccess = getSquareAtPosition(pos).getPiecesWithAcces();
    Square sq = getSquareAtPosition(pos);
    int id = sq.getCurrentPieceId();
    if(id != -1) updatePieces({id});
    updatePieces(piecesWithAccess);
}

bool Board::isSquareEmpty(Position pos) const{
    return squares[pos.x][pos.y].getCurrentPieceId() == -1;
}

Piece& Board::getPieceById(int id){
    if(id < 0 || id > 31){
        throw std::out_of_range("Error Board::getPieceById() Piece index out of range!\n");
    }
    if(!allPieces[id]){
        throw std::invalid_argument("Error Board::getPieceById() Piece by index " + std::to_string(id) + " does not exist! (nullptr)");
    }
    return *allPieces[id];
}

int Board::getPieceIdAtPosition(Position pos) const{
    return squares[pos.x][pos.y].getCurrentPieceId();
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

    Position tempPosition = getPieceById(pinning).getPosition();
    const std::pair<int, int>& direction = pin.pinningPieceDirection;

    while(true){
        tempPosition += direction;
        if(!isOnBoard(tempPosition)){
            break;
        }
        else if(!isSquareEmpty(tempPosition)){
            
            if(int tempPieceId = getPieceIdAtPosition(tempPosition); tempPieceId == pinned){
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
    if(isOnBoard(pos)){
        int x = pos.x;
        int y = pos.y;
        return squares[x][y];
    }
    else{
        throw std::invalid_argument("Square array invalid index!");
    }
}

void Board::removePieceFromSquares(int pieceId){
    const std::vector<Position>& pieceSquares = allPieces[pieceId]->getAvailableMoves();
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
    getPieceById(pieceToMoveId).pieceHasMoved();
    getSquareAtPosition(from).setCurrentPiece(-1);
    getSquareAtPosition(to).setCurrentPiece(pieceToMoveId);
}

void Board::capture(Position pieceToCapturePosition){
    try{
        int pieceId = getPieceIdAtPosition(pieceToCapturePosition);
        bool isWhite = getPieceById(pieceId).isPieceWhite();
        if(allPieces[pieceId]->getSymbol() != 'K'){
            if(isWhite){
                whitePlayer->removePlayerPiece(pieceId);
            }
            else{
                blackPlayer->removePlayerPiece(pieceId);
            }
        }
        else{
            throw std::invalid_argument("Tried to capture King!");
        }
        getSquareAtPosition(pieceToCapturePosition).setCurrentPiece(-1);
        removePieceFromSquares(pieceId);
        allPieces[pieceId] = nullptr;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Board::capture(Position pieceToCapturePosition) Error: " << e.what() << std::endl;
    }
    

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

bool Board::canPlayerCastle(bool isWhite, int dx) const{
    if(isWhite){
        return whitePlayer->canPlayerCastle(dx);
    }
    else{
        return blackPlayer->canPlayerCastle(dx);
    }
}

void Board::updatePins(){
    for(int i = pins.size() - 1; i >= 0; i--){
        Pin tempPin = pins[i];
        if(!allPieces[tempPin.pinningPieceId]){
            pins.erase(pins.begin() + i);
            allPieces[tempPin.pinnedPieceId]->setPin(false);
        }
        else if(!isPinCurrent(tempPin)){
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

bool Board::isOnBoard(Position pos) const{
    int x = pos.x;
    int y = pos.y;
    if(x < 0 || x > 7 || y < 0 || y > 7){
        return false;
    }
    return true;
}

std::pair<int, int> Board::calculateDirection(Position from, Position to) const{
    int dx = to.x - from.x;
    int dy = to.y - from.y;
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
    for(int i = 0; i < 8; i++){
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
    // White player
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

    // Black player
    color = false;


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
            throw std::invalid_argument("Incorrect piece symbol!");
            break;
    }
    if(isWhite){
        whitePlayer->addPlayerPiece(id);

    }
    else{
        blackPlayer->addPlayerPiece(id);
    }
    squares[pos.x][pos.y].setCurrentPiece(id);
}

uint64_t Board::random64BitNum(){
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());
    uint64_t num64 = dis(gen);
    return num64;
}

int Board::getPieceIndexZ(int pieceId){
        int offset = getPieceById(pieceId).isPieceWhite() ? 0: 6;
        char pieceType = allPieces[pieceId]->getSymbol();

        switch (pieceType)
        {
            case 'P': return offset + 0;
            case 'N': return offset + 1;
            case 'B': return offset + 2;
            case 'R': return offset + 3;
            case 'Q': return offset + 4;
            case 'K': return offset + 5;
            default: return 0;
        }
        return offset;
}

int Board::getSquareIndexZ(Position pos){
    return (7 - pos.y) * 8 + pos.x;
}

int Board::getCastleKeyZ(bool isWhite, int dx){
    int color = isWhite ? 0 : 2;
    int offset;
    if(dx == 1) offset = 0;
    else offset = 1;
    return color + offset;
}

uint64_t Board::zobristHash(bool isBlackPlayer){
    uint64_t hash = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            Position tempPosition(i, j);
            if(!isSquareEmpty(tempPosition)){
                int tempPieceId = getPieceIdAtPosition(tempPosition);
                hash ^= zobristTable[getPieceIndexZ(tempPieceId)][getSquareIndexZ(tempPosition)];
            }
        }
    }
    if(isBlackPlayer){
        hash ^= blackPlayerKey;
        if(blackPlayer->canPlayerCastle(1)){
            hash ^= castleKeys[getCastleKeyZ(false, 1)];
        }
        if(blackPlayer->canPlayerCastle(-1)){
            hash ^= castleKeys[getCastleKeyZ(false, -1)];
        }
    }
    else{
        if(whitePlayer->canPlayerCastle(1)){
            hash ^= castleKeys[getCastleKeyZ(true, 1)];
        }
        if(whitePlayer->canPlayerCastle(-1)){
            hash ^= castleKeys[getCastleKeyZ(true, -1)];
        }
    }
    int offset = moveHistory.size() % 2 == 1 ? 1 : 16;
    for(auto it = allPieces.begin() + offset; it != allPieces.begin() + offset + 8; ++it){
        const std::unique_ptr<Piece>& piece = *it;
        if(!piece) continue;
        if(piece->getSymbol() != 'P') continue;
        Pawn* pawn = dynamic_cast<Pawn*>(piece.get());
        auto moveDir = std::ranges::subrange(pawn->getMoveDirections().begin() + 1, pawn->getMoveDirections().end());
        for(auto dir : moveDir){
            if(int x = pawn->getPosition().x + dir.first; pawn->canEnPassant(dir.first)){
                hash ^= enPassantKeys[x];
            }
        }
    }
    return hash;
}

void Board::setPlayerPtr(Player* player, bool isWhite){
    if(isWhite){
        this->whitePlayer = player;
    }
    else{
        this->blackPlayer = player;
    }
}

void Board::generateRandNumbers(){
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < 64; j++){
            zobristTable[i][j] = random64BitNum();
        }
    }

    for(int i = 0; i < 4; i++){
        castleKeys[i] = random64BitNum();
    }

    for(int i = 0; i < 8; i++){
        enPassantKeys[i] = random64BitNum();
    }

    blackPlayerKey = random64BitNum();

}

std::span<const Square> Board::getBoardView() const{
    return std::span<const Square>(&squares[0][0], 64);
}

std::span<const std::unique_ptr<Piece>> Board::getPieceView() const{
    return std::span<const std::unique_ptr<Piece>>(allPieces);
}
const Move* Board::getLastMove() const{
    if(!moveHistory.empty()){
        return &moveHistory.back();
    }
    else{
        return nullptr;
    }
}

void Board::removePieceFromPosition(int pieceId, Position pos){
    this->squares[pos.x][pos.y].removeAttacker(pieceId);
}


