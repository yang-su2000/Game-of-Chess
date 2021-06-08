#include "record.h"

void Record::pop() {
        start.pop_back();
        end.pop_back();
}

void Record::push(Info origin, Info dest) {
        start.emplace_back(origin);
        end.emplace_back(dest);
}

bool Record::isMoved(int r, int c, PieceType pieceType) {
        for (int i = 0; i < start.size(); i++) {
                if (start[i].coordR == r && start[i].coordC == c
                        && start[i].pieceType == pieceType) {
                        return true;
                }
        }
        return false;
}

void Record::reset() {
	start.clear();
	end.clear();
	isAgain.clear();
}
