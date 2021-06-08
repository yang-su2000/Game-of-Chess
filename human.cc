#include "human.h"

using namespace std;

Human::Human(Colour colour):
	Player{colour,0,"human",false} {}

Move Human::nextMove(){
	string origin;
	string dest;

	cin >> origin;
	cin >> dest;

	Move m{origin,dest,colour};	
	return m;
}
