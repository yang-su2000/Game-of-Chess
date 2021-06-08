#ifndef MOVE_H
#define MOVE_H

#include "state.h"
#include "info.h"

struct Move {
	std::string origin;
	std::string dest;
	Colour colour;
};

#endif
