#include "game.h"

using namespace std;

int Game::ctoint(char c) {
    if (c == 'a') return 0;
    if (c == 'b') return 1;
    if (c == 'c') return 2;
    if (c == 'd') return 3;
    if (c == 'e') return 4;
    if (c == 'f') return 5;
    if (c == 'g') return 6;
    if (c == 'h') return 7;
	return 8;
}
int Game::rtoint(char r){
    if (r == '8') return 0;
    if (r == '7') return 1;
    if (r == '6') return 2;
    if (r == '5') return 3;
    if (r == '4') return 4;
    if (r == '3') return 5;
    if (r == '2') return 6;
    if (r == '1') return 7;
	return 8;
}
void Game::undo(){
	theBoard.undo();
	theBoard.drawGraph();
}
bool Game::getGame(){
	return gameInitiated;
}
double Game::getScore(Colour colour){
	if(colour == Colour::White) return whiteScore;
	return blackScore;
}
void Game::addScore(Colour colour){
	if(colour == Colour::White) whiteScore++;
	else blackScore++;
}
void Game::setGame(bool on){
	gameInitiated = on;	
}
bool Game::checkState(){
    gameState state = theBoard.getGameState();

    state = theBoard.getGameState();
    if (state == gameState::BlackWin) {
        cout << "Checkmate! Black wins" << endl;
		gameInitiated = false;
		blackScore = blackScore+1;
        return true;
    }
    if (state == gameState::WhiteWin) {
        cout << "Checkmate! White wins" << endl;
		gameInitiated = false;
		whiteScore = whiteScore + 1;
        return true;
    }
    if (state == gameState::Draw) {
        cout << "Stalemate!" << endl;
		gameInitiated = false;
		blackScore = blackScore + 0.5;
		whiteScore = whiteScore + 0.5;
        return true;
    }
    if (theBoard.getBlackInCheck()) {
        cout << "Black is in Check!!!" << endl;
    }
    if (theBoard.getWhiteInCheck()) {
        cout << "White is in Check!!!" << endl;
    }
    return false;
}

void Game::printBoard(){	
	cout << endl << theBoard << endl;
}
void Game::printScore(bool done){
	if(done) cout << "Final Score:" << endl;
    else cout << "Score:" << endl;

	cout << "White: " << whiteScore << endl;;
	cout << "Black: " << blackScore << endl;;
}
void Game::clearBoard(){
	theBoard.clearBoard();
	theBoard.drawGraph();
}
void Game::initializeBoard(){
	theBoard.init();
}

/*	====PLAYERS====	   */
shared_ptr<Player> Game::getCurrentPlayer(){
	return currentPlayer;
}
shared_ptr<Player> Game::getPlayer(Colour colour){
	if(colour == Colour::White) return whitePlayer;
	return blackPlayer;
}
void Game::nextTurn(){
	if(getCurrentPlayer()->getColour() == Colour::White) setCurrentPlayer(Colour::Black);
	else setCurrentPlayer(Colour::White);
}
bool Game::playerMove(shared_ptr<Player> p){
	Move m = p->nextMove(); 	

	if(p->getType() == "human"){
		int sR = rtoint(m.origin[1]);
		int sC = ctoint(m.origin[0]);
		int eR = rtoint(m.dest[1]);
		int eC = ctoint(m.dest[0]);
		Info originalPiece = theBoard.getInfoAt(sR, sC);
		
		if(theBoard.move(sR, sC, eR, eC, m.colour)){
		cout << "Move: " << theBoard.toString(originalPiece.pieceType) << " " << m.origin << " to " << m.dest << endl;
		checkState();
		theBoard.drawGraph();
		return true;
		}
		cout << "Invalid move, try again." << endl;
		return false;	
	}else{
		while(true){
			if (m.origin == "checkmate") {
				checkState();
				return true;
			}
			int sR = rtoint(m.origin[1]);
			int sC = ctoint(m.origin[0]);
			int eR = rtoint(m.dest[1]);
			int eC = ctoint(m.dest[0]);
			Info originalPiece = theBoard.getInfoAt(sR, sC);
			
			if(theBoard.move(sR, sC, eR, eC, m.colour, true)){
				p->setValid(false);
				cout << "Move: " << theBoard.toString(originalPiece.pieceType) << " " << m.origin << " to " << m.dest << endl;
				checkState();
				theBoard.drawGraph();
				return true;
			}
			p->setValid(true);
			m = p->nextMove();
		}
	}
}
void Game::setCurrentPlayer(Colour colour){
	if(colour == Colour::White) currentPlayer = whitePlayer;
	else currentPlayer = blackPlayer;
}
void Game::createPlayer(string player, Colour colour){
	ChessBoard &b = theBoard;
	if(player == "human"){
		shared_ptr<Human> p = make_shared<Human>(colour);
		if(colour == Colour::White){
			whitePlayer = p;
		}else{
			blackPlayer = p;
		}
	}else if(player == "computer[1]"){
		shared_ptr<ComputerLevelOne> p = make_shared<ComputerLevelOne>(colour);
        if(colour == Colour::White){
            whitePlayer = p;
        }else{
            blackPlayer = p;
        }
	}else if(player == "computer[2]"){
		shared_ptr<ComputerLevelTwo> p = make_shared<ComputerLevelTwo>(colour,b);
		if(colour == Colour::White){
            whitePlayer = p;
        }else{
            blackPlayer = p;
        }
	}else if(player == "computer[3]"){
		shared_ptr<ComputerLevelThree> p = make_shared<ComputerLevelThree>(colour,b);
		if(colour == Colour::White){
            whitePlayer = p;
        }else{
            blackPlayer = p;
		}
	}else if(player == "computer[4]"){
		shared_ptr<ComputerLevelFour> p = make_shared<ComputerLevelFour>(colour,b);
		if(colour == Colour::White){
            whitePlayer = p;
        }else{
            blackPlayer = p;
		}
	}

}

/*	====SETUP====	*/
Colour determineColour(string piece){
	char pieceName = piece[0];
	if(97 < pieceName && pieceName < 122){
		return Colour::Black;
	}else{
		return Colour::White;
	}
}
PieceType determineChessType(string piece){
	string error = "Error: " + piece + " is an unknown piece type";
	if(piece[0] == 'k' || piece[0] == 'K') return PieceType::King;
	else if(piece[0] == 'q' || piece[0] == 'Q') return PieceType::Queen;
	else if(piece[0] == 'r' || piece[0] == 'R') return PieceType::Rook;
	else if(piece[0] == 'b' || piece[0] == 'B') return PieceType::Bishop;
	else if(piece[0] == 'n' || piece[0] == 'N') return PieceType::Knight;
	else if(piece[0] == 'p' || piece[0] == 'P') return PieceType::Pawn;
	else throw error;
}

void Game::setupAdd(string piece, string coord){
	Colour pieceColour = determineColour(piece);
	PieceType pieceType = determineChessType(piece);
	if(pieceType == PieceType::King){
		theBoard.setupCheckForKings(pieceColour, false);
	}
	theBoard.setPiece(coord, pieceColour, pieceType);	
}

void Game::setupRemove(string coord){
	Colour pieceColour = Colour::NoColour;
	PieceType pieceType = PieceType::Empty;
	theBoard.setPiece(coord, pieceColour, pieceType);
}

void Game::setupTurn(string colour){
	if(colour == "White" || colour == "white"){
		currentPlayer = whitePlayer;
	}else if(colour == "Black" || colour == "black"){
		currentPlayer = blackPlayer;
	}else{
		string error = "Error: Invalid colour";
		throw error;
	}
}


void Game::setupExit(){
	//Cannot exit if there isn't 1 King of each colour or if Kings are in check
	theBoard.setupCheckForKings(Colour::White,true);
	theBoard.setupCheckForKings(Colour::Black,true);
	if((theBoard.isInCheck(Colour::Black))){
		string error = "Error: Black King in Check";
		throw error;
	}
	if((theBoard.isInCheck(Colour::White))){ 
		string error = "Error: White King in Check";
		throw error;
	}
}

void Game::showHistory(){
	theBoard.History();
}
