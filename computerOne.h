#ifndef COMPUTER_ONE_H
#define COMPUTER_ONE_H
#include "player.h"
class ComputerLevelOne : public Player{
	public:
	ComputerLevelOne(Colour);
	Move nextMove() override;
};
#endif
