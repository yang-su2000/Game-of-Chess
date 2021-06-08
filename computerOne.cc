#include <time.h>
#include <stdlib.h>
#include "player.h"
#include "computerOne.h"
#include <random>
using namespace std;

ComputerLevelOne::ComputerLevelOne(Colour colour):
    Player{colour,0,"computer[1]",false}{}

Move ComputerLevelOne::nextMove(){
	//I am very random
	return generateRandomMove();
}
