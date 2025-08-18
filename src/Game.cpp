#include "Game.hpp"
#include "Pawn.hpp"

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
        //Stockfish::Bitboard::init();
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
    executeTurn(from, to, true, symb);
}

void Game::executeTurn(Position from, Position to, bool promotion, char symb){
    Move newMove = createMove(from, to, promotion, symb);
    moveHistory.push_back(newMove);
    moveCount++;
    currentPlayer = moveCount % 2;
    board.makeMove(newMove, currentPlayer);
    checkGameState();
    selectedPiece = -1;
}

Move Game::createMove(Position from, Position to, bool promotion, char promPiece){
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
        if(from.x != to.x && newMove.capture() && board.isSquareEmpty(to)){
            newMove.setEnPassant(true);
        }
    if(promotion){
        newMove.setPromotion(promotion);
        newMove.setPromotionPiece(promPiece);
    }

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
                endGame(Loss);
            }
        }
        else if(players[currentPlayer].hasPlayerMoves()){
            board.updatePins();
            if(moveHistory.size() >= 2 && moveHistory[moveHistory.size() - 2].check()){
                const std::vector<int>& playersPieces = players[currentPlayer].getPiecesId();
                board.updatePieces(playersPieces);
            }
            if(!players[currentPlayer].hasEnoughMaterial()){
                endGame(Draw);
            }

        }
        else{
            endGame(Draw);
        }
    }
}

void Game::endGame(Result res){
    std::cout<<"Game ended"<<res <<std::endl;
    ui.setGamePtr(nullptr);
    ui.showBoard(false);
    ui.showMenu(true);
    ui.setPieceView({});
    ui.setBoardView({});
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
    Move tempMove = moveHistory.back();
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



