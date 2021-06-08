#ifndef RECORD_H
#define RECORD_H
#include <vector>
#include "info.h"

struct Record {
	std::vector<Info> start;
	std::vector<Info> end;
	std::vector<int> isAgain;
	void pop();
	void push(Info origin, Info dest);
	bool isMoved(int r, int c, PieceType pieceType);
	void reset();
};

#endif
