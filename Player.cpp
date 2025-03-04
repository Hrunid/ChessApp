#include "Player.h"
#include "Board.h"

Player::Player(int kingId, bool isWhite, Board& board)
    :   kingId(kingId),
        isWhite(isWhite),
        numOfChecks(0),
        board(board),
        piecesId()
        {}


bool Player::hasEnoughMaterial(){

    int bishopCount = 0;
    int knightCount = 0;

    for(int id : piecesId){
        
        char pieceSymbol = board.getPieceById(id).getSymbol();

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
    Position kingPosition = board.getPieceById(kingId).getPosition();
    const std::vector<int>& piecesWithAcces = board.getSquareAtPosition(kingPosition).getPiecesWithAcces();

    numOfChecks = 0;

    for(int tempPieceId : piecesWithAcces){
        bool tempIsWhite = board.getPieceById(tempPieceId).isPieceWhite();
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
        bool pieceHasNoMoves = board.getPieceById(id).getAvailableMoves().empty();
        if(!pieceHasNoMoves){
            return true;
        }
    }
    return false;
}

void Player::removePlayerPiece(int pieceToRemove){
    for(int i=0; i < piecesId.size(); i++){
        int pieceId = piecesId[i];
        if(pieceId == pieceToRemove){
            piecesId.erase(piecesId.begin() + i);
            break;
        } 
    }
}

bool Player::canPlayerCastle(){
    if(isPlayerInCheck()){
        return false;
    }
    else if(board.getPieceById(kingId).hasPieceMoved()){            //cheks if king has moved
        return false;
    }
    for(int id : piecesId){
        char tempSymbol = board.getPieceById(id).getSymbol();
        bool tempHasMoved = board.getPieceById(id).hasPieceMoved();
        if(tempSymbol == 'R' && !tempHasMoved){
            return true;
        }
    }
    return false;
}

void Player::addPlayerPiece(int pieceToAdd){
    piecesId.push_back(pieceToAdd);
}

void Player::applyCheckRestrictions(){
    if(numOfChecks >= 2){
        for(int pieceId : piecesId){
            board.getPieceById(pieceId).clearMoves();

        }
        board.getPieceById(kingId).calculateAvailableMoves();

    }
    else if(numOfChecks == 1){
        Position kingPosition = board.getPieceById(kingId).getPosition();
        int attackingPieceId;
        const std::vector<int>& attackingPiecesId = board.getSquareAtPosition(kingPosition).getPiecesWithAcces();

        for(int id : attackingPiecesId){
            if(isWhite != board.getPieceById(id).isPieceWhite()){
                attackingPieceId = id;
                break;
            }
        }

        char attackingPieceSymbol = board.getPieceById(attackingPieceId).getSymbol();
        Position attackingPiecePosition = board.getPieceById(attackingPieceId).getPosition();
        std::vector<Position> checkLine = getCheckLine(attackingPiecePosition, kingPosition);

        for(int id : piecesId){
            board.getPieceById(id).clearMoves();
        }

        for(Position pos : checkLine){
            const std::vector<int>& attackingPieceId = board.getSquareAtPosition(pos).getPiecesWithAcces();
            for(int id : attackingPieceId){
                if(isWhite == board.getPieceById(id).isPieceWhite()){
                    board.getPieceById(id).addMove(pos);
                }
            }

        }

    }
}

std::vector<Position> Player::getCheckLine(Position start, Position end){
    std::vector<Position> checkLine;
    std::pair<int, int> direction = board.calculateDirection(start, end);

    checkLine.push_back(start);

    int x = start.x;
    int y = start.y;
    int dx = direction.first;
    int dy = direction.second;
    int x2 = end.x;
    int y2 = end.y;
    while(x != x2 || y != y2){
        x += dx;
        y += dy;
        Position tempPosition(x, y);
        checkLine.push_back(tempPosition);
    }

    return checkLine;
}


int Player::getKingId() const{
    return kingId;
}

bool Player::isPlayerWhite() const{
    return isWhite;
}
