#include "Game.hpp"
#include "Pawn.hpp"

Game::Game()
    :   moveHistory(),
        positionHistory(),
        moveCount(0),
        selectedPiece(-1),
        currentPlayer(0),
        boringMoves(0),
        state(Running)
    {
        players[0] = std::make_unique<Player>(0, true);
        players[1] = std::make_unique<Player>(16, false);
        board = std::make_unique<Board>(players[0].get(), players[1].get());
        
        
        players[0]->setBoardPtr(board.get());
        players[1]->setBoardPtr(board.get());
    }

void Game::processClick(Position click){
    int pieceAtClick = board->getPieceIdAtPosition(click);
    if(selectedPiece != -1){
        if(board->getPieceById(selectedPiece).isMoveAvailable(click)){
            char selSymb = board->getPieceById(selectedPiece).getSymbol();
            if(selSymb == 'P'){
                
            }
            executeTurn(board->getPieceById(selectedPiece).getPosition(), click);
        }
        else if(!board->isSquareEmpty(click) && (board->getPieceById(pieceAtClick).isPieceWhite() == players[currentPlayer]->isPlayerWhite())){     //Check if clicked piece is same color as current player
            selectedPiece = pieceAtClick;
        }
        else{
            selectedPiece = -1;
        }
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

void Game::executeTurn(Position from, Position to){
    Move newMove = createMove(from, to);
    moveHistory.push_back(newMove);
    moveCount++;
    currentPlayer = moveCount % 2;
    board->makeMove(newMove);
    checkGameState();
}

Move Game::createMove(Position from, Position to){
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
        /*if((board->getPieceById(tempPieceId).isPieceWhite() && to.y == 7) || (!board->getPieceById(tempPieceId).isPieceWhite() && to.y == 1)){
            newMove.setPromotion()
        }*/
    }
    return newMove;
}

void Game::checkGameState(){
    
    if(threeTimeRepetition() || fiftyMoveRule()){
        endGame(Draw);
    }
    else{
        if(players[currentPlayer]->isPlayerInCheck()){

            if(players[currentPlayer]->hasPlayerMoves()){
                players[currentPlayer]->applyCheckRestrictions();
                moveHistory.back().setCheck(true);
            }
            else{
                moveHistory.back().setMate(true);
                endGame(Loss);
            }
        }
        else if(players[currentPlayer]->hasPlayerMoves()){
            if(moveHistory[moveHistory.size() - 2].check()){
                std::vector<int> playersPieces = players[currentPlayer]->getPiecesId();
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
    char tempSymbol = tempMove.getPieceSymbol();
    int yDiff = tempMove.getPositionFrom().y - tempMove.getPositionTo().y;
    std::pair<bool, int> enPassant(false, 0);
    if(tempSymbol == 'P' && (yDiff == 2 || yDiff == -2)){
        enPassant.first = true;
        enPassant.second = tempMove.getPositionFrom().x;
    }

    uint64_t position = board->zobristHash(players[currentPlayer]->isPlayerWhite(), enPassant);

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

    if(boringMoves >= 100) return true;
    else return false;

}

void Game::runStockfish(){
    
}

GameState Game::getGameState(){
    return state;
}

int Game::getSelectedPiece(){
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


