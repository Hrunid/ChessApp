#include "Board.h"
#include "King.h"
#include "Bishop.h"
#include "Queen.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"

#include <random>
#include <limits>
#include <span>
#include <optional>
#include <exception>
#include <ranges>
#include <optional>
#include <cassert>

Board::Board(Player* whitePlayer, Player* blackPlayer, const std::vector<Move>& moveHis)
    :   pins(),
        whitePlayer(whitePlayer),
        blackPlayer(blackPlayer),
        moveHistory(moveHis),
        squares{}

{
            
    Piece::setBoardPtr(this);
    setUpPieces();
    for(int id = 0; id < allPiecesMAX; id++){
        allPieces[id]->calculateAvailableMoves();
        addPieceToSquares(id);
    }
    generateZobristKeys();
}

Board::~Board(){
    whitePlayer = nullptr;
    blackPlayer = nullptr;
}

void Board::makeMove(const Move& move, int player){

    Position from = move.getPositionFrom();
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
        tempPositionToUpdate = enPassant(from, to);
    }
    if(move.promotion()){
        promotion(movedPieceId, move.getPromotionPieceSymbol());
        tempPositionToUpdate = to;
    }
    std::bitset<32> accessFrom = getSquareAtPosition(from).getPiecesWithAcces();
    std::bitset<32> accessTo = getSquareAtPosition(to).getPiecesWithAcces();

    std::bitset<32> update = accessFrom | accessTo;

    movePiece(from ,to);
    if (tempPositionToUpdate.has_value() && isOnBoard(*tempPositionToUpdate)) {
        updatePiecesAtPosition(*tempPositionToUpdate);
    }

    Player* p = player == 0 ? whitePlayer : blackPlayer;
    int kingId = p->getKingId();

    updatePieces(update);
    updatePins();

    auto optPin = p->pinnedOwn(move);
    if(optPin.has_value()){
        pins.emplace_back(*optPin);
    }
    updatePiece(kingId);

}

void Board::updatePiecesAtPosition(Position pos){
    Square& sq = getSquareAtPosition(pos);
    std::bitset<32> piecesWithAccess = sq.getPiecesWithAcces();
    
    int id = sq.getCurrentPieceId();
    if(id != -1) piecesWithAccess[id] = 1;
    updatePieces(piecesWithAccess);
}

bool Board::isSquareEmpty(Position pos) const{
    return squares[pos.x][pos.y].getCurrentPieceId() == -1;
}

Piece& Board::getPieceById(int id){
    assert(id >= 0 && id <= 31);
    assert(allPieces[id]);
    return *allPieces[id];
}

int Board::getPieceIdAtPosition(Position pos) const{
    return squares[pos.x][pos.y].getCurrentPieceId();
}

bool Board::isPinCurrent(Pin pin){
    
    int pinningId = pin.pinningPieceId;
    int pinnedId = pin.pinnedPieceId;
    Piece& pinning = getPieceById(pinningId);
    Position tempPosition = pinning.getPosition();
    const std::pair<int, int>& direction = pin.pinningPieceDirection;

    int next = nextInDirection(direction, pinning.getPosition());
    if(next != pinnedId || next == -1) return false;
    Position newStart = getPieceById(next).getPosition();
    next = nextInDirection(direction, newStart);
    if(next == -1) return false;
    Piece& last = getPieceById(next);
    if((last.getSymbol() != 'K') || (pinning.isPieceWhite() == last.isPieceWhite())) return false;
    return true;

}

Square& Board::getSquareAtPosition(Position pos){
    assert(isOnBoard(pos));
        int x = pos.x;
        int y = pos.y;
        return squares[x][y];
    
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
        int pieceId = getPieceIdAtPosition(pieceToCapturePosition);
        bool isWhite = getPieceById(pieceId).isPieceWhite();
        assert(allPieces[pieceId]->getSymbol() != 'K');

        if(isWhite){
            whitePlayer->removePlayerPiece(pieceId);
        }
        else{
            blackPlayer->removePlayerPiece(pieceId);
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
    int dx = xDiff > 0 ? -1 : 1;
    Position tempPosition(from.x + dx, from.y);
    capture(tempPosition);
    return tempPosition;
}

void Board::updatePins(){
    std::erase_if(pins, [&](Pin pin){
        int pinningId = pin.pinningPieceId;
        int pinnedId = pin.pinnedPieceId;

        if(!(allPieces[pinningId]) && allPieces[pinnedId]){
            allPieces[pinnedId]->setPin(false);
            this->updatePiece(pinnedId);
            return true;
        }
        if(!(allPieces[pinnedId]) && allPieces[pinningId]){
            allPieces[pinningId]->setPinningStatus(false);
            return true;
        } 
        if(!(allPieces[pinningId]) && !(allPieces[pinnedId])){
            return true;
        }
        if(!isPinCurrent(pin)){
            allPieces[pinningId]->setPinningStatus(false);
            allPieces[pinnedId]->setPin(false);
            this->updatePiece(pinnedId);
            return true;
        }
        return false;

    });
}

void Board::addPin(Pin newPin){
    pins.emplace_back(newPin);
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

bool Board::areAligned(Position from, Position to) const{
    int xDiff = to.x - from.x;
    int yDiff = to.y - from.y;

    if(xDiff == 0 || yDiff == 0) return true;
    if(std::abs(xDiff) == std::abs(yDiff)) return true;

    return false;
}

const std::vector<Pin>& Board::getPins() const{
    return pins;
}

std::string Board::convertToFEN(){
    std::string FEN;
    FEN.reserve(30);
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

int Board::getCastleKeyZ(bool isWhite, CastleSide side){
    int color = isWhite ? 0 : 2;
    int offset = (side == CastleSide::Short) ? 0 : 1;
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
        if(blackPlayer->hasLongCastleRights()){
            hash ^= castleKeys[getCastleKeyZ(false, CastleSide::Long)];
        }
        if(blackPlayer->hasShortCastleRights()){
            hash ^= castleKeys[getCastleKeyZ(false, CastleSide::Short)];
        }
    }
    else{
        if(whitePlayer->hasLongCastleRights()){
            hash ^= castleKeys[getCastleKeyZ(true, CastleSide::Long)];
        }
        if(whitePlayer->hasShortCastleRights()){
            hash ^= castleKeys[getCastleKeyZ(true, CastleSide::Short)];
        }
    }
    int offset = moveHistory.size() % 2 == 1 ? 1 : 16;
    for(auto it = allPieces.begin() + offset; it != allPieces.begin() + offset + 8; ++it){
        const std::unique_ptr<Piece>& piece = *it;
        if(!piece) continue;
        if(piece->getSymbol() != 'P') continue;
        Pawn* pawn = dynamic_cast<Pawn*>(piece.get());
        if(pawn->enPassantCol.has_value()){
            hash ^= enPassantKeys[*(pawn->enPassantCol)];
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

void Board::generateZobristKeys(){
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
    squares[pos.x][pos.y].removeAttacker(pieceId);
}

std::optional<std::reference_wrapper<Piece>> Board::getPieceAtPosition(Position pos){
    if(isSquareEmpty(pos)) return std::nullopt;
    return *allPieces[getPieceIdAtPosition(pos)];
}

void Board::updatePiece(int id){
    removePieceFromSquares(id);
    allPieces[id]->calculateAvailableMoves();
    addPieceToSquares(id);
}

void Board::updatePieces(std::bitset<32> update){
    for(int id = update._Find_first(); id < update.size(); id = update._Find_next(id)){
        if(!allPieces[id]) continue;
        removePieceFromSquares(id);
        allPieces[id]->calculateAvailableMoves();
        addPieceToSquares(id);
    }
}

int Board::nextInDirection(std::pair<int, int> dir, Position start){
    while(true){
        start += dir;
        if(!isOnBoard(start)) break;
        if(isSquareEmpty(start)) continue;
        return getPieceIdAtPosition(start);
    }
    return -1;
}

bool Board::hasPlayerCastleRights(bool isWhite, CastleSide side){
    Player* p = isWhite ? whitePlayer : blackPlayer;
    if(side == CastleSide::Short) return p->hasShortCastleRights();
    if(side == CastleSide::Long) return p->hasLongCastleRights();
    
    return false;
}


