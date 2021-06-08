#ifndef HUMAN_H
#define HUMAN_H
#include "player.h"

class Human : public Player{
	ChessBoard *b;
	public:
	Human(Colour);
	Move nextMove() override;	
};

#endif
