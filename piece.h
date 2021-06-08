#ifndef PIECE_H
#define PIECE_H
#include "state.h"
#include "info.h"
#include "observer.h"
#include "subject.h"

class Piece : public Subject, public Observer {
	const int r, c;
	Colour colour;
	PieceType pieceType;
    void replyMessage(State s, Direction reverse);

	public:
	Piece(int r, int c);
    void setPiece(Colour c, PieceType p);
	bool movePiece(int destR, int destC, Colour movedcolor); 
	bool notify(Subject &whoFrom) override;
	Info getInfo() const override;
};

#endif
