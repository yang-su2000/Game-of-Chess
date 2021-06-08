#ifndef COMPUTER_TWO_H
#define COMPUTER_TWO_H
#include "player.h"
class ComputerLevelTwo : public Player{
	ChessBoard &b;
	void printMoves();
	std::vector<std::pair<int, std::pair<Info, std::string>>> checkableMoves;
	std::vector<std::pair<int, std::pair<Info, std::string>>> attackablePieces;
	std::vector<Move> moveList;
	bool isInvalid = false;
	bool debug = false;

        public:
        ComputerLevelTwo(Colour, ChessBoard&);
	bool checkCheck(int,int, Info);
	bool checkCapture(int, int, Info);	
        Move nextMove() override;
};
#endif
