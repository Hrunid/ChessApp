#include "Game.h"
#include "Pawn.h"

Game::Game(GameType type, UI& uiReff)
    :   moveHistory(),
        positionHistory(),
        moveCount(0),
        selectedPiece(-1),
        currentPlayer(0),
        boringMoves(0),
        state(Running),
        type(type),
        ui(uiReff),
        players{{{0, true}, {16, false}}},
        board(&players[0], &players[1], moveHistory)
    {
        players[0].setBoardPtr(&board);
        players[1].setBoardPtr(&board);

    }

void Game::processClick(Position click){
    int pieceAtClick = board.getPieceIdAtPosition(click);
    bool clickedEmpty = board.isSquareEmpty(click);
    bool clickedOwnPiece = !(clickedEmpty) &&  board.getPieceById(pieceAtClick).isPieceWhite() == players[currentPlayer].isPlayerWhite();
    if(selectedPiece != -1){
        Piece& selected = board.getPieceById(selectedPiece);
        bool tempIsWhite = selected.isPieceWhite();
        if(selected.isMoveAvailable(click)){
            Position from = selected.getPosition();
            if(selected.getSymbol() == 'P'){
                int promRow = selected.isPieceWhite() ? 0 : 7;
                if(int promRow = tempIsWhite ? 0 : 7; click.y == promRow){
                    ui.drawPromotionOptions(from, click);
                    return;
                }
            }
            executeTurn(from, click);
            return;
        }
        selectedPiece = clickedOwnPiece ? pieceAtClick : -1;
        return;
    }
    selectedPiece = clickedOwnPiece ? pieceAtClick : -1;
}

void Game::promote(Position from, Position to, char symb){
    executeTurn(from, to, std::make_optional(symb));
}

void Game::executeTurn(Position from, Position to, std::optional<char> promPiece){
    moveHistory.emplace_back(createMove(from, to, promPiece));
    moveCount++;
    currentPlayer = moveCount % 2;
    board.makeMove(moveHistory.back(), currentPlayer);
    checkGameState();
    selectedPiece = -1;
}

Move Game::createMove(Position from, Position to, std::optional<char> promPiece){
    int tempPieceId = board.getPieceIdAtPosition(from);
    char symb = board.getPieceById(tempPieceId).getSymbol();
    Move newMove(from, to, symb);
    if(!board.isSquareEmpty(to)){
        newMove.setCapture(true);
    }
    if(symb == 'K' && abs(from.x - to.x) > 1){
        newMove.setCastle(true);
    }
    else if(symb == 'P'){
        if(from.x != to.x && board.isSquareEmpty(to)){
            newMove.setEnPassant(true);
        }
    }
    if(promPiece.has_value()){
        newMove.setPromotion(true);
        newMove.setPromotionPiece(*promPiece);
    }

    return newMove;
}

void Game::checkGameState(){
    
    if(threeTimeRepetition() || fiftyMoveRule()){
        endGame(Draw);
    }
    else{
        if(players[currentPlayer].isPlayerInCheck()){
            moveHistory.back().setCheck(true);
            players[currentPlayer].applyCheckRestrictions();
            if(!players[currentPlayer].hasPlayerMoves()){
                moveHistory.back().setMate(true);
                moveHistory.back().setCheck(false);
                endGame(Loss);
            }
        }
        else if(players[currentPlayer].hasPlayerMoves()){
            if(moveHistory.size() >= 3 && moveHistory[moveHistory.size() - 3].check()){
                board.updatePieces(players[currentPlayer].getPiecesId());
            }
            if(!(players[currentPlayer].hasEnoughMaterial())){
                endGame(Draw);
            }
        }
        else{
            endGame(Draw);
        }
    }
}

void Game::endGame(Result res){

}

bool Game::threeTimeRepetition(){
    Move tempMove = moveHistory.back();
    uint64_t position = board.zobristHash(players[currentPlayer].isPlayerWhite());

    if(positionHistory.find(position) != positionHistory.end()){
        positionHistory[position]++;
    }
    else{
        positionHistory[position] = 1;
    }

    if(positionHistory[position] == 3){
        return true;
    }
    else{
        return false;
    }
}

bool Game::fiftyMoveRule(){
    const Move& tempMove = moveHistory.back();
    if(tempMove.capture()) boringMoves = 0;
    else if(tempMove.getPieceSymbol() == 'P') boringMoves = 0;
    else boringMoves++;

    if(boringMoves >= 50) return true;
    else return false;

}

void Game::runStockfish(){
    
}

GameState Game::getGameState() const{
    return state;
}

int Game::getSelectedPiece() const{
    return selectedPiece;
}

const Piece& Game::getPieceById(int pieceId){
    return board.getPieceById(pieceId);
}

const Player& Game::getCurrentPlayer(){
    return players[currentPlayer];
}

GameType Game::getGameType(){
    return type;
}

void Game::setGameState(GameState newState){
    state = newState;
}

std::span<const Square> Game::boardView() const{
    return board.getBoardView();
}

std::span<const std::unique_ptr<Piece>> Game::pieceView() const{
    return board.getPieceView();
}



