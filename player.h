#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <random>
#include <utility>
#include <algorithm>
#include "move.h"
#include "chessboard.h"

class Player{
	protected:
	Colour colour;
	double score;
	std::string type;
	bool isInvalid; //Turns on when the previous move is invalid

	public:
	Player(Colour, double, std::string, bool);
	virtual Move nextMove() = 0;
	void addScore(double);		//adds points to the score of Player
	Move generateRandomMove();	//generates a random Move

	std::string translateToCoord(int, int);	//translates ex.col:0 row:5 into a coordinate "a4"

	Colour getColour();
	int getPieceValue(Info);
	double getScore();
	std::string getType();
	std::string colourToString();
	void setValid(bool);

};

#endif
