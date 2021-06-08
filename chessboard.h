#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "piece.h"
#include "record.h"
#include "window.h"
class Observer;
enum class gameState {
    WhiteWin, BlackWin, Draw, Ingame
};
class ChessBoard {
        std::vector<std::vector<Piece>> ActualBoard;
        Record record;
        gameState game = gameState::Ingame;
	Xwindow x;
        bool whiteInCheck = false;
        bool blackInCheck = false;
        bool checking = false;
        int ctoint(char);
        bool noMoreLegalMove (Colour colour);
        bool isPromotion(int sR, int sC, int eR, int eC, bool computer = false);
        bool isImpasse(int sR, int sC, int eR, int eC);
	bool isCastling(int sR, int sC, int eR, int eC);
        void setObserver(Observer *ob);
        public:
	void newGraph();
	void drawGraph();
	void draw(int r, int c, PieceType pieceType, Colour colour);
	void History();
        gameState determinestate();
        bool checkforcapture(Info);
        bool isInCheck(Colour);
        gameState getGameState();
        bool getWhiteInCheck();
        bool getBlackInCheck();
		std::string toString(PieceType);
        Info getKingInfo(Colour);
		std::string toString(Colour);
		std::string toString(int,int);
        bool undo(); // call record, undo twice depends on record type
        bool move(int sR, int sC, int eR, int eC , Colour colour, bool simulate = false); // move piece
        void init(); // initialize chessboard
		/* SETUP */
		void clearBoard();
		void setPiece(std::string, Colour, PieceType);
		bool setupCheckForPawn();
		void setupCheckForKings(Colour, bool);
		void setupCheckForCheck(Colour);
		Colour getColourAt(int, int);
		PieceType getTypeAt(int,int);
		Info getInfoAt(int,int);
		/* SETUP */
        friend std::ostream &operator<<(std::ostream &out, const ChessBoard &cb);
};
#endif
