#include "Player.h"
#include "Board.h"
#include "King.h"

Player::Player(int kingId, bool isWhite)
    :   kingId(kingId),
        isWhite(isWhite),
        numOfChecks(0),
        board(nullptr),
        piecesId(),
        sCastleRights(true),
        lCastleRights(true)
        {}

Player::~Player(){
    board = nullptr;
}


bool Player::hasEnoughMaterial(){

    int bishopCount = 0;
    int knightCount = 0;

    for(int id = piecesId._Find_first(); id < Board::allPiecesMAX; id = piecesId._Find_next(id)){
        
        char pieceSymbol = board->getPieceById(id).getSymbol();

        if(pieceSymbol == 'P' || pieceSymbol == 'Q' || pieceSymbol == 'R'){
            return true;
        }
        else if(pieceSymbol == 'B'){
            bishopCount++;
        }
        else if(pieceSymbol == 'N'){
            knightCount++;
        }
        if(bishopCount >= 2 || (bishopCount >= 1 && knightCount >= 1)){
            return true;
        }

    }
    return false;

}

bool Player::isPlayerInCheck(){
    Position kingPosition = board->getPieceById(kingId).getPosition();
    std::bitset<32> piecesWithAcces = board->getSquareAtPosition(kingPosition).getPiecesWithAcces();

    numOfChecks = 0;
    for(int id = piecesWithAcces._Find_first(); id < Board::allPiecesMAX; id = piecesWithAcces._Find_next(id)){
        bool tempIsWhite = board->getPieceById(id).isPieceWhite();
        if(isWhite != tempIsWhite){
            numOfChecks++;
        }
    }
    if(numOfChecks > 0){
        return true;
    }
    else{
        return false;
    }
    
}

bool Player::hasPlayerMoves(){

    for(int id = piecesId._Find_first(); id < Board::allPiecesMAX; id = piecesId._Find_next(id)){
        const auto& moves = board->getPieceById(id).getAvailableMoves();
        if(!moves.empty()){
            return true;
        }
    }
    return false;
}

void Player::removePlayerPiece(int pieceToRemove){
    int begin = isWhite ? 0 : 16;
    if(pieceToRemove < begin || pieceToRemove > begin + 16) throw std::invalid_argument("Wrong piece ID for player " + std::to_string(isWhite) + " : " + std::to_string(pieceToRemove));
    piecesId[pieceToRemove] = 0;
}
//Napisać osobne metody do roszady królewskiej i długiej. Uprościć Szukanie wieży poprzez sprawdzanie sprawdzanie rogu planszy.

void Player::addPlayerPiece(int pieceToAdd){
    int begin = isWhite ? 0 : 16;
    if(pieceToAdd < begin || pieceToAdd >= begin + 16) throw std::invalid_argument("Wrong piece ID for player " + std::to_string(isWhite) + " : " + std::to_string(pieceToAdd));
    piecesId[pieceToAdd] = 1;
}

void Player::applyCheckRestrictions(){
    if(numOfChecks >= 2){
        for(size_t id = piecesId._Find_first(); id < Board::allPiecesMAX; id = piecesId._Find_next(id)){
            board->getPieceById(id).doubleCheck();
        }
        board->updatePiece(kingId);
    }
    else if(numOfChecks == 1){
        Position kingPosition = board->getPieceById(kingId).getPosition();
        int attackingPieceId;
        std::bitset<32> attackingPieces = board->getSquareAtPosition(kingPosition).getPiecesWithAcces();

        for(int id = attackingPieces._Find_first(); id < Board::allPiecesMAX; id = attackingPieces._Find_next(id)){
            if(isWhite != board->getPieceById(id).isPieceWhite()) attackingPieceId = id;
        }
        Piece& aPiece = board->getPieceById(attackingPieceId);
        char aSymbol = aPiece.getSymbol();
        Position aPos = aPiece.getPosition();
        std::vector<Position> checkLine = getCheckLine(aPos, kingPosition, aSymbol);
        for(int id = isWhite ? 1 : 17; id < Board::allPiecesMAX; id = piecesId._Find_next(id)){

            Piece& piece = board->getPieceById(id);
            std::vector<Position> pieceSquares = piece.getAvailableMoves();

            for(Position pos : pieceSquares){
                if(std::find(checkLine.begin(), checkLine.end(), pos) == checkLine.end()){
                    board->removePieceFromPosition(id, pos);
                    piece.removeMove(pos);
                }
            }
        }
        board->updatePiece(kingId);
    }
}

std::vector<Position> Player::getCheckLine(Position start, Position end, char aSymb) const{
    std::vector<Position> checkLine;
    if(aSymb == 'N'){
        checkLine = {start};
    }
    else{
        std::pair<int, int> direction = board->calculateDirection(start, end);
        while(start != end){
            checkLine.emplace_back(start);
            start += direction;
        }
    }
    return checkLine;
}

int Player::getKingId() const{
    return kingId;
}

bool Player::isPlayerWhite() const{
    return isWhite;
} 

void Player::setBoardPtr(Board* board){
    this->board = board;
}

std::bitset<32> Player::getPiecesId(){
    return piecesId;
}

std::optional<Pin> Player::pinnedOwn(const Move& lastMove){

    Position from = lastMove.getPositionFrom();
    Position to = lastMove.getPositionTo();
    Position kingPos = board->getPieceById(kingId).getPosition();

    if(!board->areAligned(kingPos, from)) return std::nullopt;
    auto dir = board->calculateDirection(kingPos, from);

    int first = board->nextInDirection(dir, kingPos);
    if(first == -1) return std::nullopt;

    Piece& piece = board->getPieceById(first);
    if(isWhite != piece.isPieceWhite()) return std::nullopt;

    Position newStart = piece.getPosition();
    int second = board->nextInDirection(dir, newStart);

    if(second == -1) return std::nullopt;
    Piece& piece2 = board->getPieceById(second);
    if(char symb = piece2.getSymbol(); piece2.isPieceWhite() == isWhite || symb == 'P' || symb == 'N' || symb == 'K') return std::nullopt;

    Pin newPin;
    newPin.pinnedPieceId = first;
    newPin.pinningPieceId = second;
    newPin.pinnedPieceDirection = dir;
    newPin.pinningPieceDirection = {-dir.first, -dir.second};

    piece.setPin(true);
    board->updatePiece(first);
    piece2.setPinningStatus(true);

    return newPin;

}

bool Player::hasCastleRights(CastleSide side){
    if(board->getPieceById(kingId).hasPieceMoved()){
        return sCastleRights = lCastleRights = false;
    }

    int row = board->getPieceById(kingId).getPosition().y;
    int x = (side == Short) ? 7 : 0;
    Position rookPos{x, row};

    if(side == Short){
        if(!sCastleRights) return false;

        auto rook = board->getPieceAtPosition(rookPos);
        if(!rook.has_value()) return sCastleRights = false;

        if(rook->get().hasPieceMoved()){
            return sCastleRights = false;
        }
        return true;

    }
    if(side == Long){
        if(!lCastleRights) return false;

        auto rook = board->getPieceAtPosition(rookPos);
        if(!rook.has_value()) return lCastleRights = false;

        if(rook->get().hasPieceMoved()){
            return lCastleRights = false;
        }
        return true;

    }
    return false;
}

bool Player::hasLongCastleRights(){
    return hasCastleRights(Long);
}

bool Player::hasShortCastleRights(){
    return hasCastleRights(Short);
}

bool Player::canCastleLong() const{
    King* king = dynamic_cast<King*>(&board->getPieceById(kingId));
    return king->canCastleL();
}

bool Player::canCastleShort() const{
    King* king = dynamic_cast<King*>(&board->getPieceById(kingId));
    return king->canCastleS();
}
