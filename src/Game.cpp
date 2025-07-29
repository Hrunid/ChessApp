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
        ui(uiReff)
        
    {
        players[0] = std::make_unique<Player>(0, true);
        players[1] = std::make_unique<Player>(16, false);
        board = std::make_unique<Board>(players[0].get(), players[1].get(), moveHistory);
        
        
        players[0]->setBoardPtr(board.get());
        players[1]->setBoardPtr(board.get());
    }

void Game::processClick(Position click){
    int pieceAtClick = board->getPieceIdAtPosition(click);
    std::cout << " called procesClick() at x=" << click.x << " y=" << click.y << " selected piece:" << selectedPiece <<std::endl;
    if(selectedPiece != -1){
        if(board->getPieceById(selectedPiece).isMoveAvailable(click)){
            if(char selSymb = board->getPieceById(selectedPiece).getSymbol(); selSymb == 'P'){
                if(int promRow = getPieceById(selectedPiece).isPieceWhite() ? 0 : 7; click.y == promRow){
                    ui.drawPromotionOptions(click);
                    return;
                }
            }
            executeTurn(board->getPieceById(selectedPiece).getPosition(), click);
            return;
        }
        else if(!board->isSquareEmpty(click) && (board->getPieceById(pieceAtClick).isPieceWhite() == players[currentPlayer]->isPlayerWhite())){     //Check if clicked piece is same color as current player
            selectedPiece = pieceAtClick;
        }
        else{
            selectedPiece = -1;

        }
        return;
    }
    else if(board->isSquareEmpty(click)){
        selectedPiece = -1;
    }
    else if(players[currentPlayer]->isPlayerWhite() == board->getPieceById(pieceAtClick).isPieceWhite()){
        selectedPiece = pieceAtClick;
    }
    else{
        selectedPiece = -1;
    }
}
/*
void Game::processClick(Position click) {
    int pieceAtClick = board->getPieceIdAtPosition(click);
    bool squareEmpty = board->isSquareEmpty(click);
    bool clickedOwnPiece = !squareEmpty &&
        board->getPieceById(pieceAtClick).isPieceWhite() == players[currentPlayer]->isPlayerWhite();

    if (selectedPiece != -1) {
        const Piece& selected = board->getPieceById(selectedPiece);

        if (selected.isMoveAvailable(click)) {
            if (selected.getSymbol() == 'P') {
                int promotionRow = selected.isPieceWhite() ? 7 : 0;
                if (click.x == promotionRow) {
                    ui.drawPromotionOptions(click);
                    return;
                }
            }

            executeTurn(selected.getPosition(), click);
            return;
        }

        selectedPiece = clickedOwnPiece ? pieceAtClick : -1;
        return;
    }
    selectedPiece = clickedOwnPiece ? pieceAtClick : -1;
}*/

void Game::promote(Position from, Position to, char symb){
    executeTurn(from, to, true, symb);
}

void Game::executeTurn(Position from, Position to, bool promotion, char symb){
    Move newMove = createMove(from, to);
    moveHistory.push_back(newMove);
    moveCount++;
    currentPlayer = moveCount % 2;
    board->makeMove(newMove);
    checkGameState();
    selectedPiece = -1;
}

Move Game::createMove(Position from, Position to, bool promotion, char promPiece){
    int tempPieceId = board->getPieceIdAtPosition(from);
    char symb = board->getPieceById(tempPieceId).getSymbol();
    Move newMove(from, to, symb);
    if(!board->isSquareEmpty(to)){
        newMove.setCapture(true);
    }
    if(symb == 'K' && abs(from.x - to.x) > 1){
        newMove.setCastle(true);
    }
    else if(symb == 'P'){
        if(from.x != to.x && newMove.capture() && board->isSquareEmpty(to)){
            newMove.setEnPassant(true);
        }
    if(promotion){
        newMove.promotion();
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
        if(players[currentPlayer]->isPlayerInCheck()){
            moveHistory.back().setCheck(true);
            if(players[currentPlayer]->hasPlayerMoves()){
                players[currentPlayer]->applyCheckRestrictions();
            }
            else{
                moveHistory.back().setMate(true);
                endGame(Loss);
            }
        }
        else if(players[currentPlayer]->hasPlayerMoves()){
            board->updatePins();
            if(moveHistory.size() >= 2 && moveHistory[moveHistory.size() - 2].check()){
                const std::vector<int>& playersPieces = players[currentPlayer]->getPiecesId();
                board->updatePieces(playersPieces);
            }
            if(!players[currentPlayer]->hasEnoughMaterial()){
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
}

bool Game::threeTimeRepetition(){
    Move tempMove = moveHistory.back();
    uint64_t position = board->zobristHash(players[currentPlayer]->isPlayerWhite());

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
    return board->getPieceById(pieceId);
}

const Player& Game::getCurrentPlayer(){
    return *players[currentPlayer];
}

GameType Game::getGameType(){
    return type;
}

void Game::setGameState(GameState newState){
    state = newState;
}

std::span<const std::unique_ptr<Square>> Game::boardView() const{
    return board->getBoardView();
}

std::span<const std::unique_ptr<Piece>> Game::pieceView() const{
    return board->getPieceView();
}



