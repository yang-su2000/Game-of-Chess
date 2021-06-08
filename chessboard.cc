#include "chessboard.h"
using namespace std;
string ChessBoard::toString(Colour c){
	if(c == Colour::White) return "White";
	if(c == Colour::Black) return "Black";
	return "NoColour";
}
string ChessBoard::toString(PieceType p){
	if(p == PieceType::Pawn) return "Pawn";
	if(p == PieceType::Rook) return "Rook";
	if(p == PieceType::Queen) return "Queen";
	if(p == PieceType::Knight) return "Knight";
	if(p == PieceType::Bishop) return "Bishop";
	if(p == PieceType::King) return "King";
	if(p == PieceType::Empty) return "Empty";
}
string ChessBoard::toString(int c, int r){
    string col;
    string row;
    if(c == 0) col = "a";
    else if(c == 1) col = "b";
    else if(c == 2) col = "c";
    else if(c == 3) col = "d";
    else if(c == 4) col = "e";
    else if(c == 5) col = "f";
    else if(c == 6) col = "g";
    else if(c == 7) col = "h";
    else return "Error";
    if(r == 0) row = "8";
    else if(r == 1) row = "7";
    else if(r == 2) row = "6";
    else if(r == 3) row = "5";
    else if(r == 4) row = "4";
    else if(r == 5) row = "3";
    else if(r == 6) row = "2";
    else if(r == 7) row = "1";
    else return "Error";
    return col + row;
}
Info ChessBoard::getKingInfo(Colour kingColour) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Info currentpiece = ActualBoard[r][c].getInfo();
            if (currentpiece.pieceType == PieceType::King && currentpiece.colour == kingColour) return currentpiece;
        }
     }
    throw "no King";
    // this should never happen since there must be a king in the board
}
gameState ChessBoard::getGameState() {
    return game;
}
bool ChessBoard::getWhiteInCheck() {return whiteInCheck;}
bool ChessBoard::getBlackInCheck() {return blackInCheck;}
bool ChessBoard::noMoreLegalMove(Colour colour) {
    checking = true;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            for (int tor = 0; tor < 8; tor++) {
                for (int toc = 0; toc < 8; toc++) {
                    if (move(r,c,tor,toc,colour, true)) {
                        undo();
                        checking = false;
                        return false;
                    }
                }
            }
        }
     }
    checking = false;
    return true;
}
gameState ChessBoard::determinestate() {
	whiteInCheck = checkforcapture(getKingInfo(Colour::White));
	blackInCheck = checkforcapture(getKingInfo(Colour::Black));
    bool whiteNoMoreLegalMove = noMoreLegalMove(Colour::White);
    bool blackNoMoreLegalMove = noMoreLegalMove(Colour::Black);
    if ((!whiteNoMoreLegalMove) && (!blackNoMoreLegalMove)) return gameState::Ingame;
    if (whiteNoMoreLegalMove && whiteInCheck) return gameState::BlackWin;
    else if (whiteNoMoreLegalMove && !whiteInCheck) return gameState::Draw;
    else if (blackNoMoreLegalMove && blackInCheck) return gameState::WhiteWin;
    else if (blackNoMoreLegalMove && !blackInCheck) return gameState::Draw;
    else return gameState::Ingame;
    
}
// Takes the info of a piece as parameter, the method will check if the piece can be attacked by any opposite piece.
bool ChessBoard::checkforcapture(Info pieceToBeChecked) {
    Colour enemy = (pieceToBeChecked.colour == Colour::White)? Colour::Black : Colour::White;
     for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            record.push(ActualBoard[r][c].getInfo(), pieceToBeChecked);
            if (ActualBoard[r][c].movePiece(pieceToBeChecked.coordR,pieceToBeChecked.coordC, enemy)) {
                undo();
                return true;
            }
            record.pop();
        }
     }
    return false;
}
bool ChessBoard::undo() {
        if (record.start.size() == 0) {
                cout << "No earlier moves are avaliable to undo." << endl;
                return false;
        }
		game = gameState::Ingame;
        Info pS = record.start.back();
        Info pE = record.end.back();
        record.pop();
        ActualBoard[pS.coordR][pS.coordC].setPiece(pS.colour, pS.pieceType);
        ActualBoard[pE.coordR][pE.coordC].setPiece(pE.colour, pE.pieceType);
        if (record.isAgain.size() && record.isAgain.back() == record.start.size() + 1) {
                record.isAgain.pop_back();
                undo();
        }
        return true;
}
bool ChessBoard::isPromotion(int sR, int sC, int eR, int eC, bool simulate) {
        Info meS = ActualBoard[eR][eC].getInfo();
        if ((meS.pieceType == PieceType::Pawn && meS.colour == Colour::Black && eR == 7) ||
            (meS.pieceType == PieceType::Pawn && meS.colour == Colour::White && eR == 0)) {
			if(!simulate){
                cout << "Choose one chess to promote from: R(rook), K(knight), B(bishop) or Q(queen)." << endl;
                string promote;
                cin >> promote;
                if (promote == "R") {
                        ActualBoard[eR][eC].setPiece(meS.colour, PieceType::Rook);
                } else if (promote == "K") {
                        ActualBoard[eR][eC].setPiece(meS.colour, PieceType::Knight);
                } else if (promote == "B") {
                        ActualBoard[eR][eC].setPiece(meS.colour, PieceType::Bishop);
                } else if (promote == "Q") {
                        ActualBoard[eR][eC].setPiece(meS.colour, PieceType::Queen);
                } else {
                        cout << "Need to choose from R, K, B or Q." << endl;
                        isPromotion(sR, sC, eR, eC, simulate);
                }
			} else {
                ActualBoard[eR][eC].setPiece(meS.colour, PieceType::Queen);	
			}
                return true;
        }  return false;
}
bool ChessBoard::isImpasse(int sR, int sC, int eR, int eC) {
        Info meS = ActualBoard[sR][sC].getInfo();
        Info meE = ActualBoard[eR][eC].getInfo();
        if ((meS.pieceType != PieceType::Pawn || record.start.size() == 0)) { return false; }
        Info lastS = record.start.back();
        Info lastE = record.end.back();
        if (meS.colour == Colour::Black && meS.coordR==lastE.coordR) {
                if ((sR + 1 == eR) && ((sC + 1 == eC) || (sC - 1 == eC)) &&
                                lastS.pieceType == PieceType::Pawn && meE.colour == Colour::NoColour
                                && lastS.pieceType == PieceType::Pawn && lastS.colour == Colour::White
                                && lastS.coordR - lastE.coordR == 2 && eC == lastS.coordC) {
                        Info tmp1 = lastS;
                        tmp1.coordR = lastE.coordR;
                        record.push(meS, tmp1);
                        Info tmp2 = meS;
                        tmp2.coordC = lastS.coordC;
                        record.push(tmp2, meE);
                        record.isAgain.emplace_back(record.start.size());
                        ActualBoard[sR][sC].setPiece(Colour::NoColour, PieceType::Empty);
                        ActualBoard[eR][eC].setPiece(Colour::Black, PieceType::Pawn);
                        ActualBoard[lastE.coordR][lastE.coordC].setPiece(Colour::NoColour, PieceType::Empty);
                        return true;
                } return false;
        } else if (meS.colour == Colour::White && meS.coordR==lastE.coordR) {
                if ((sR - 1 == eR) && ((sC + 1 == eC) || (sC - 1 == eC)) &&
                                lastS.pieceType == PieceType::Pawn && meE.colour == Colour::NoColour
                                && lastS.pieceType == PieceType::Pawn && lastS.colour == Colour::Black
                                && lastE.coordR - lastS.coordR == 2 && eC == lastS.coordC) {
                        Info tmp1 = lastS;
                        tmp1.coordR = lastE.coordR;
                        record.push(meS, tmp1);
                        Info tmp2 = meS;
                        tmp2.coordC = lastS.coordC;
                        record.push(tmp2, meE);
                        record.isAgain.emplace_back(record.start.size());
                        ActualBoard[sR][sC].setPiece(Colour::NoColour, PieceType::Empty);
                        ActualBoard[eR][eC].setPiece(Colour::White, PieceType::Pawn);
                        ActualBoard[lastE.coordR][lastE.coordC].setPiece(Colour::NoColour, PieceType::Empty);
                        return true;
                } return false;
        } return false;
}
bool ChessBoard::isCastling(int sR, int sC, int eR, int eC) {
        Info meS = ActualBoard[sR][sC].getInfo();
        if (meS.pieceType != PieceType::King || sC != 4 || record.isMoved(sR, sC, PieceType::King)) return false;
        if ((sR == 0 && eR == 0 && meS.colour == Colour::Black) ||
            (sR == 7 && eR == 7 && meS.colour == Colour::White)) {
                if (eC == 2) { // castling to "left"
                        for (int i = 1; i <= 3; i++) {
                                if (ActualBoard[sR][i].getInfo().colour != Colour::NoColour) return false;
                        }
                        Info meE = ActualBoard[sR][0].getInfo();
                        if (record.isMoved(sR, 0, PieceType::Rook)) return false;
                        if (meE.pieceType == PieceType::Rook && meE.colour == meS.colour) {
                                int count = 0;
                                if (sR == 0) {
                                        count = move(0, 4, 0, 3, Colour::Black) + move(0, 3, 0, 2, Colour::Black)
                                                + move(0, 2, 0, 1, Colour::Black);
                                } else if (sR == 7) {
                                        count = move(7, 4, 7, 3, Colour::White) + move(7, 3, 7, 2, Colour::White)
                                                + move(7, 2, 7, 1, Colour::White);
                                }
                                for (int i = 0; i < count; i++) { undo(); }
                                if (count != 3) { drawGraph(); return false; }
                                Info tmpK = ActualBoard[eR][eC].getInfo();
                                record.push(meS, tmpK);
                                ActualBoard[sR][sC].setPiece(Colour::NoColour, PieceType::Empty);
                                ActualBoard[eR][eC].setPiece(meS.colour, PieceType::King);
                                Info tmpR = ActualBoard[sR][sC-1].getInfo();
                                record.push(meE, tmpR);
                                ActualBoard[sR][sC-1].setPiece(meS.colour, PieceType::Rook);
                                ActualBoard[sR][0].setPiece(Colour::NoColour, PieceType::Empty);
                                record.isAgain.emplace_back(record.start.size());
                                return true;
                        }
                } else if (eC == 6) { // castling to "right"
                        for (int i = 5; i <= 6; i++) {
                                if (ActualBoard[sR][i].getInfo().colour != Colour::NoColour) return false;
                        }
                        Info meE = ActualBoard[sR][7].getInfo();
                        if (record.isMoved(sR, 0, PieceType::Rook)) return false;
                        if (meE.pieceType == PieceType::Rook && meE.colour == meS.colour) {
                                if ((sR == 0 && move(0 ,4, 0, 5, Colour::Black)) ||
                                    (sR == 7 && move(7, 4, 7, 5, Colour::White))) {
                                        undo();
                                        Info tmpK = ActualBoard[eR][eC].getInfo();
                                        record.push(meS, tmpK);
                                        ActualBoard[sR][sC].setPiece(Colour::NoColour, PieceType::Empty);
                                        ActualBoard[eR][eC].setPiece(meS.colour, PieceType::King);
                                        Info tmpR = ActualBoard[sR][sC+1].getInfo();
                                        record.push(meE, tmpR);
                                        ActualBoard[sR][sC+1].setPiece(meS.colour, PieceType::Rook);
                                        ActualBoard[sR][7].setPiece(Colour::NoColour, PieceType::Empty);
                                        record.isAgain.emplace_back(record.start.size());
                                        return true;
                                }
                        }
                }
        } return false;
}
int ChessBoard::ctoint(char c) {
    if (c == 'a') return 0;
    if (c == 'b') return 1;
    if (c == 'c') return 2;
    if (c == 'd') return 3;
    if (c == 'e') return 4;
    if (c == 'f') return 5;
    if (c == 'g') return 6;
    if (c == 'h') return 7;
    else return 8;
}
bool ChessBoard::move(int sR, int sC, int eR, int eC, Colour colour, bool simulate) {
    
    if (game != gameState::Ingame) return false;
    
    if (isImpasse(sR, sC, eR, eC)) { // check for en passant
            
        if (!checkforcapture(getKingInfo(colour))) {
                
            if (!checking) {
                game = determinestate();
            }
            return true;
        } else {
            undo();
            return false;
        }
    }
    if (isCastling(sR, sC, eR, eC)) { // check for castling
	     if (!checkforcapture(getKingInfo(colour))) {

            if (!checking) {
                game = determinestate();
            }
            return true;
        } else {
            undo();
	    undo();
            return false;
        }
    }
    record.push(ActualBoard[sR][sC].getInfo(), ActualBoard[eR][eC].getInfo());
    if (ActualBoard[sR][sC].movePiece(eR,eC, colour)) {

        if (!checkforcapture(getKingInfo(colour))) {
            
            isPromotion(sR, sC, eR, eC, simulate);
            
            if (!checking) {
                
                game = determinestate();
            }
            return true;
        } else {
            undo();
            return false;
        }
    }
    record.pop();
    return false;
}
void ChessBoard::init() {
        game = gameState::Ingame;
        checking = false;
        ActualBoard.clear();
	record.reset();
	newGraph();
        for (int r = 0; r < 8; r++) {
                vector<Piece> subboard;
                for (int c = 0; c < 8; c++) {
                        subboard.emplace_back(Piece{r, c});
                }
                if (r == 0) {
                        subboard[0].setPiece(Colour::Black, PieceType::Rook);
                        subboard[1].setPiece(Colour::Black, PieceType::Knight);
                        subboard[2].setPiece(Colour::Black, PieceType::Bishop);
                        subboard[3].setPiece(Colour::Black, PieceType::Queen);
                        subboard[4].setPiece(Colour::Black, PieceType::King);
                        subboard[5].setPiece(Colour::Black, PieceType::Bishop);
                        subboard[6].setPiece(Colour::Black, PieceType::Knight);
                        subboard[7].setPiece(Colour::Black, PieceType::Rook);
                }
                else if (r < 7) {
                        for (int c = 0; c < 8; c++) {
                                if (r == 1) {
                                        subboard[c].setPiece(Colour::Black, PieceType::Pawn);
                                } else if (r == 6) {
                                        subboard[c].setPiece(Colour::White, PieceType::Pawn);
                                } else {
                                        subboard[c].setPiece(Colour::NoColour, PieceType::Empty);
                                }
                        }
                } else {
                        subboard[0].setPiece(Colour::White, PieceType::Rook);
                        subboard[1].setPiece(Colour::White, PieceType::Knight);
                        subboard[2].setPiece(Colour::White, PieceType::Bishop);
                        subboard[3].setPiece(Colour::White, PieceType::Queen);
                        subboard[4].setPiece(Colour::White, PieceType::King);
                        subboard[5].setPiece(Colour::White, PieceType::Bishop);
                        subboard[6].setPiece(Colour::White, PieceType::Knight);
                        subboard[7].setPiece(Colour::White, PieceType::Rook);
                }
                ActualBoard.emplace_back(subboard);
        }
        for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                        if (r) {
                                ActualBoard[r][c].attach(&ActualBoard[r - 1][c]);
                        } // attach N
                        if (r && c) {
                                ActualBoard[r][c].attach(&ActualBoard[r - 1][c - 1]);
                        } // attach NW
                        if (r && (c < 7)) {
                                ActualBoard[r][c].attach(&ActualBoard[r - 1][c + 1]);
                        } // attach NE
                        if (r < 7) {
                                ActualBoard[r][c].attach(&ActualBoard[r + 1][c]);
                        } // attach S
                        if ((r < 7) && c) {
                                ActualBoard[r][c].attach(&ActualBoard[r + 1][c - 1]);
                        } // attach SW
                        if ((r < 7) && (c < 7)) {
                                ActualBoard[r][c].attach(&ActualBoard[r + 1][c + 1]);
                        } // attach SE
                        if (c) {
                                ActualBoard[r][c].attach(&ActualBoard[r][c - 1]);
                        } // attach W
                        if (c < 7) {
                                ActualBoard[r][c].attach(&ActualBoard[r][c + 1]);
                        } // attach E
                        if ((r > 1) && c) {
                                ActualBoard[r][c].attach(&ActualBoard.at(r - 2).at(c - 1));
                        } // attach NNW
                        if ((r > 1) && (c < 7)) {
                                ActualBoard[r][c].attach(&ActualBoard.at(r - 2).at(c + 1));
                        } // attach NNE
                        if ((r < 6) && c) {
                                ActualBoard[r][c].attach(&ActualBoard.at(r + 2).at(c - 1));
                        } // attach SSW
                        if ((r < 6) && (c < 7)) {
                                ActualBoard[r][c].attach(&ActualBoard.at(r + 2).at(c + 1));
                        } // attach SSE
                        if (r && (c > 1)) {
                                ActualBoard[r][c].attach(&ActualBoard.at(r - 1).at(c - 2));
                        } // attach WWN
                        if ((r < 7) && (c > 1)) {
                                ActualBoard[r][c].attach(&ActualBoard.at(r + 1).at(c - 2));
                        } // attach WWS
                        if (r && (c < 6)) {
                                ActualBoard[r][c].attach(&ActualBoard.at(r - 1).at(c + 2));
                        } // attach EEN
                        if ((r < 7) && (c < 6)) {
                                ActualBoard[r][c].attach(&ActualBoard.at(r + 1).at(c + 2));
                        } // attach EES
                }
        }
}
std::ostream &operator<<(std::ostream &out, const ChessBoard &cb) {
        bool BoardColour = true; // false -> black, true -> white
        for (int r = 0; r < cb.ActualBoard.size(); r++) {
			out << 8-r << "  ";
                for (int c = 0; c < cb.ActualBoard[r].size(); c++) {
                        Colour C = cb.ActualBoard[r][c].getInfo().colour;
                        PieceType CT = cb.ActualBoard[r][c].getInfo().pieceType;
                        if (C == Colour::NoColour) {
                                if (BoardColour) { out << " "; }
                                else { out << "-"; }
                        } else if (C == Colour::Black) {
                                if      (CT == PieceType::Pawn) { out << "p"; }
                                else if (CT == PieceType::Knight) { out << "n"; }
                                else if (CT == PieceType::Rook) { out << "r"; }
                                else if (CT == PieceType::Bishop) { out << "b"; }
                                else if (CT == PieceType::Queen) { out << "q"; }
                                else if (CT == PieceType::King) { out << "k"; }
                                else {}
                        } else if (C == Colour::White) {
                                if      (CT == PieceType::Pawn) { out << "P"; }
                                else if (CT == PieceType::Knight) { out << "N"; }
                                else if (CT == PieceType::Rook) { out << "R"; }
                                else if (CT == PieceType::Bishop) { out << "B"; }
                                else if (CT == PieceType::Queen) { out << "Q"; }
                                else if (CT == PieceType::King) { out << "K"; }
                                else {}
                        } else {}
                        if (!(c + 1 == cb.ActualBoard[r].size())) {
                                BoardColour = !BoardColour;
                        }
                }
                out << std::endl;
				if(r == cb.ActualBoard.size()-1) out << endl << "   abcdefgh" << std::endl;
        }
        return out;
}
//Checks if a pawn is at Row 1 or Row 8
bool ChessBoard::setupCheckForPawn(){
    for(int c = 0; c < 8; c++){
        if(ActualBoard.at(0).at(c).getInfo().pieceType == PieceType::Pawn){
            ActualBoard.at(0).at(c).setPiece(Colour::NoColour, PieceType::Empty);
            return false;
        }
        if(ActualBoard.at(7).at(c).getInfo().pieceType == PieceType::Pawn){
            ActualBoard.at(7).at(c).setPiece(Colour::NoColour, PieceType::Empty);
            return false;
        }
    }
    return true;
}
void ChessBoard::clearBoard() {
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            ActualBoard[r][c].setPiece(Colour::NoColour, PieceType::Empty);
        }
    }
    record.reset();
}
void ChessBoard::setPiece(string coord, Colour colour, PieceType piece){
    int r;
	string rs{coord[1]};
	istringstream iss{rs};
	iss >> r;
    int c = ctoint(coord[0]);
    ActualBoard.at(8-r).at(c).setPiece(colour,piece);
    bool pawnCheck = this->setupCheckForPawn();
    if(!pawnCheck){
        string error = "Error: cannot place Pawn at the first or last row of the board";
        throw error;
    }
    draw(8-r, c, piece, colour);
}
void ChessBoard::setupCheckForKings(Colour colour, bool done){
    int kings = 0;
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            if(this->getTypeAt(r,c) == PieceType::King){
                if(this->getColourAt(r,c) == colour){
                    kings++;
                }
            }
        }
    }
    string errorMessage;
    if((kings >= 1 && !done) || (kings != 1 && done)){
        errorMessage = "Error: There must only be one King";
        throw errorMessage;
    }
}
Colour ChessBoard::getColourAt(int r, int c){
    if((r >= 0) && (r < 8) && (c >= 0) && (c < 8)){
        return ActualBoard.at(r).at(c).getInfo().colour;
    }else{
        return Colour::NoColour;
    }
}
PieceType ChessBoard::getTypeAt(int r, int c){
    if((r >= 0) && (r < 8) && (c >= 0) && (c < 8)){
        return ActualBoard.at(r).at(c).getInfo().pieceType;
    }else{
        return PieceType::Empty;
    }
}
bool ChessBoard::isInCheck(Colour colour){
	return checkforcapture(getKingInfo(colour));
}
Info ChessBoard::getInfoAt(int r,int c){
	return ActualBoard[r][c].getInfo();
}
void ChessBoard::newGraph() {
        x.fillRectangle(0, 0, 600, 600, Xwindow::White);
        for (int r = 0; r < 8; r++) {
                if (r == 0) {
                        draw(r, 0, PieceType::Rook,   Colour::Black);
                        draw(r, 1, PieceType::Knight, Colour::Black);
                        draw(r, 2, PieceType::Bishop, Colour::Black);
                        draw(r, 3, PieceType::Queen,  Colour::Black);
                        draw(r, 4, PieceType::King,   Colour::Black);
                        draw(r, 5, PieceType::Bishop, Colour::Black);
                        draw(r, 6, PieceType::Knight, Colour::Black);
                        draw(r, 7, PieceType::Rook,   Colour::Black);
                } else if (r < 7) {
                        for (int c = 0; c < 8; c++) {
                                if (r == 1) {
                                draw(r, c, PieceType::Pawn, Colour::Black);
                                } else if (r == 6) {
                                draw(r, c, PieceType::Pawn, Colour::White);
                                } else {
                                draw(r, c, PieceType::Empty, Colour::NoColour);
                                }
                        }
                } else {
                        draw(r, 0, PieceType::Rook,   Colour::White);
                        draw(r, 1, PieceType::Knight, Colour::White);
                        draw(r, 2, PieceType::Bishop, Colour::White);
                        draw(r, 3, PieceType::Queen,  Colour::White);
                        draw(r, 4, PieceType::King,   Colour::White);
                        draw(r, 5, PieceType::Bishop, Colour::White);
                        draw(r, 6, PieceType::Knight, Colour::White);
                        draw(r, 7, PieceType::Rook,   Colour::White);
                }
        }
}
void ChessBoard::draw(int r, int c, PieceType chess, Colour colour) {
        int boardcolor;
        int chesscolor;
        if ((r + c) / 2 * 2 == (r + c)) { boardcolor = 6; }
        else { boardcolor = 3; }
        x.fillRectangle(75 * c, 75 * r, 75, 75, boardcolor);
        std::string CT;
        if (colour == Colour::NoColour || chess == PieceType::Empty) {
                return;
        }
        else if (colour == Colour::Black) {
                chesscolor = 0;
                x.fillCircle(75 * c + 37, 75 * r + 37, 40, 1);
        if (chess == PieceType::Pawn)        { CT = "p"; }
        else if (chess == PieceType::Knight) { CT = "n"; }
        else if (chess == PieceType::Rook)   { CT = "r"; }
        else if (chess == PieceType::Bishop) { CT = "b"; }
        else if (chess == PieceType::Queen)  { CT = "q"; }
        else if (chess == PieceType::King)   { CT = "k"; }
        } else if (colour == Colour::White) {
                chesscolor = 1;
                x.fillCircle(75 * c + 37, 75 * r + 37, 40, 0);
        if (chess == PieceType::Pawn)        { CT = "P"; }
        else if (chess == PieceType::Knight) { CT = "N"; }
        else if (chess == PieceType::Rook)   { CT = "R"; }
        else if (chess == PieceType::Bishop) { CT = "B"; }
        else if (chess == PieceType::Queen)  { CT = "Q"; }
        else if (chess == PieceType::King)   { CT = "K"; }
        }
        x.drawString(75 * c + 37, 75 * r + 42, CT, chesscolor);
}
void ChessBoard::drawGraph() {
        for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                        Info info = ActualBoard[r][c].getInfo();
                        PieceType chess = info.pieceType;
                        Colour colour = info.colour;
                        draw(r, c, chess, colour);
                }
        }
}
void ChessBoard::History() {
        cout << "History is printed out from the first move to the most recent move." << endl;
        cout << "Moves that are 'undo'ed are not counted." << endl;
        cout << "There are currently " << record.start.size() - record.isAgain.size() << " move(s)." << endl;
        cout << "----------Start----------" << endl;
        int j = 0;
        for (int i = 0; i < record.start.size(); i++) {
                cout << i+1-j << ". ";
                if (j < record.isAgain.size() && i + 2 == record.isAgain[j]) {
                        if (record.start[i].pieceType == PieceType::Pawn) {
                                cout << toString(record.start[i].colour) << " " << toString(record.start[i].pieceType) << " did [en passant] and moved from " << toString(record.start[i].coordC, record.start[i].coordR) << " to " << toString(record.end[i+1].coordC, record.end[i+1].coordR) << " and captured the " << toString(record.end[i].colour) << " " << toString(record.end[i].pieceType) << " at " << toString(record.end[i].coordC, record.end[i].coordR) << endl;
                        i++; j++;
                        continue;
                        } else if (record.start[i].pieceType == PieceType::King) {
                                cout << toString(record.start[i].colour) << " " << toString(record.start[i].pieceType) << " did [castling] with " << toString(record.start[i+1].colour) << " " << toString(record.start[i+1].pieceType) << " and moved from " << toString(record.start[i].coordC, record.start[i].coordR) << " to " << toString(record.end[i].coordC, record.end[i].coordR) << endl;
                        i++; j++;
                        continue;
                        }
                } else {
                        cout << toString(record.start[i].colour) << " " << toString(record.start[i].pieceType) << " moved from " << toString(record.start[i].coordC, record.start[i].coordR) << " to " << toString(record.end[i].coordC, record.end[i].coordR);
                        if (record.end[i].pieceType != PieceType::Empty) {
                                cout << " and captured the " << toString(record.end[i].colour) << " " << toString(record.end[i].pieceType) << " at " << toString(record.end[i].coordC, record.end[i].coordR);
                        }
                        cout << endl;
                }
        }
        cout << "-----------End-----------" << endl;
}
