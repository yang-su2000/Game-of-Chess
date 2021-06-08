#ifndef COMPUTER_THREE_H
#define COMPUTER_THREE_H
#include "player.h"
class ComputerLevelThree : public Player{
	ChessBoard &b;
	void printMoves();
	bool isInvalid = false;
	bool debug = false;
	std::vector<std::pair<int, std::pair<Info, std::string>>> retreatedPieces;
	std::vector<std::pair<int, std::pair<Info, std::string>>> checkableMoves;
	std::vector<std::pair<int, std::pair<Info, std::string>>> attackablePieces;
	std::vector<Move> moveList;
        public:
        ComputerLevelThree(Colour, ChessBoard&);
	void checkAvoid(int, int, Info);
	void checkCheck(int, int, Info);
	void checkCapture(int, int, Info);	
	void getMoveList();
        Move nextMove() override;
	void toggleDebug();
};
#endif
