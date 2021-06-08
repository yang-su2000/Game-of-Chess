#ifndef STATE_H
#define STATE_H

enum class MessageType { NewMove, Relay, Reply };
enum class Colour { NoColour, Black, White };
enum class Direction { NW, N, NE, SW, S, SE, W, E, Other };
enum class PieceType {Pawn, King, Queen, Bishop, Rook, Knight, Empty};

struct State {
        PieceType type;
	MessageType message; // what state is "me"
	Colour colour; // what colour is the "origin"
	Direction dir; // what direction from me is the "origin"
	int originC;
	int originR;
	int destC;
	int destR;
        bool success;
};

#endif
