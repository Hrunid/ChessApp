#include "Player.hpp"
#include "Board.hpp"

Player::Player(int kingId, bool isWhite)
    :   kingId(kingId),
        isWhite(isWhite),
        numOfChecks(0),
        board(nullptr),
        piecesId()
        {}

Player::~Player(){
    board = nullptr;
}


bool Player::hasEnoughMaterial(){

    int bishopCount = 0;
    int knightCount = 0;

    for(int id : piecesId){
        
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
    const std::vector<int>& piecesWithAcces = board->getSquareAtPosition(kingPosition).getPiecesWithAcces();

    numOfChecks = 0;

    for(int tempPieceId : piecesWithAcces){
        bool tempIsWhite = board->getPieceById(tempPieceId).isPieceWhite();
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

    for(int id : piecesId){
        bool pieceHasNoMoves = board->getPieceById(id).getAvailableMoves().empty();
        if(!pieceHasNoMoves){
            return true;
        }
    }
    return false;
}

void Player::removePlayerPiece(int pieceToRemove){
    piecesId.erase(std::remove(piecesId.begin(), piecesId.end(), pieceToRemove), piecesId.end());
}

bool Player::canPlayerCastle(int dx){
    if(board != nullptr){
        if(isPlayerInCheck()){
            return false;
        }
        else if(board->getPieceById(kingId).hasPieceMoved()){            
            return false;
        }
        int rookX;
        if(dx > 0) rookX = 7;
        else{
            rookX = 0;
        }
        Position tempPosition(rookX, board->getPieceById(kingId).getPosition().y);
        if(!board->isSquareEmpty(tempPosition)){
            int tempPieceId = board->getPieceIdAtPosition(tempPosition);
            char tempSymb = board->getPieceById(tempPieceId).getSymbol();
            bool tempHasMoved = board->getPieceById(tempPieceId).hasPieceMoved();
            if(tempSymb == 'R' && !tempHasMoved){
                return true;
            }
        }
        return false;
    }
    else{
        return false;
    }
}


//Napisać osobne metody do roszady królewskiej i długiej. Uprościć Szukanie wieży poprzez sprawdzanie sprawdzanie rogu planszy.

void Player::addPlayerPiece(int pieceToAdd){
    piecesId.push_back(pieceToAdd);
}

void Player::applyCheckRestrictions(){
    if(numOfChecks >= 2){
        for(int pieceId : piecesId){
            board->getPieceById(pieceId).clearMoves();
        }
        board->updatePieces({kingId});
    }
    else if(numOfChecks == 1){
        Position kingPosition = board->getPieceById(kingId).getPosition();
        int attackingPieceId;
        const std::vector<int>& attackingPiecesId = board->getSquareAtPosition(kingPosition).getPiecesWithAcces();
        std::vector<int>::const_iterator it = std::find_if(
            attackingPiecesId.begin(),
            attackingPiecesId.end(),
            [&](int id) {
                return isWhite != board->getPieceById(id).isPieceWhite();
            }
        );
        attackingPieceId = *it;
        char attackingPieceSymbol = board->getPieceById(attackingPieceId).getSymbol();
        Position attackingPiecePosition = board->getPieceById(attackingPieceId).getPosition();
        std::vector<Position> checkLine = getCheckLine(attackingPiecePosition, kingPosition, attackingPieceSymbol);
        for(auto it = piecesId.begin() + 1; it != piecesId.end(); ++it){
            int id = *it;
            Piece& piece = board->getPieceById(id);
            std::vector<Position> pieceSquares = piece.getAvailableMoves();
            for(Position pos : pieceSquares){
                if(std::find(checkLine.begin(), checkLine.end(), pos) == checkLine.end()){
                    board->removePieceFromPosition(id, pos);
                    piece.removeMove(pos);
                }
            }
        }
        board->updatePieces({kingId});
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
            checkLine.push_back(start);
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

const std::vector<int>& Player::getPiecesId(){
    return piecesId;
}
