#include "piece.h"
#include <iostream>
using namespace std;
Piece::Piece(int r, int c) : r{r}, c{c} {}
void Piece::setPiece(Colour c, PieceType p) {
	colour = c;
	pieceType = p;
}
bool Piece::movePiece(int destR, int destC, Colour movedcolor) {
    if (pieceType == PieceType::Empty || colour != movedcolor || (r == destR && c == destC)) return false;
    setState(State{pieceType, MessageType::NewMove, colour, Direction::Other, c, r, destC, destR, false});
    //cout << "move starts." << endl;
    if (notify(*this)) return true;
    else {
        setState(State{pieceType, MessageType::Relay, colour, Direction::Other, c, r, destC, destR, false});
        return false;
    }
}
int abs(int x) {
    if (x > 0) return x;
    else return -x;
}
//the position of 2 relative to 1, or from 1's perspective. For example, if 1 is at left 2 is at right the function returns east 
Direction getdirect(int r1, int r2, int c1, int c2) {
    if (r1 == r2) {
        if (c2 > c1) return Direction::E;
        else return Direction::W;
    } else if (c1 == c2) {
        if (r2 > r1) return Direction::S;
        else return Direction::N;
    } else {
        if (abs(r2-r1) != abs(c2-c1)) return Direction::Other;
        if (r2 > r1) {
            if (c2 > c1) return Direction::SE;
            else return Direction::SW;
        } else {
            if (c2 > c1) return Direction::NE;
            else return Direction::NW;
        }
    }
}
Direction reverse(Direction direct) {
    if (direct == Direction::E) return Direction::W;
    else if (direct == Direction::W) return Direction::E;
    else if (direct == Direction::N) return Direction::S;
    else if (direct == Direction::S) return Direction::N;
    else if (direct == Direction::NW) return Direction::SE;
    else if (direct == Direction::SE) return Direction::NW;
    else if (direct == Direction::NE) return Direction::SW;
    else if (direct == Direction::SW) return Direction::NE;
    else return Direction::Other;
}
void Piece::replyMessage(State s, Direction direct) {
    //cout << "reply starts" << endl;
    //cout << "r: " << r << endl;
    //cout << "c: " << c << endl;
    this->setState(State{s.type, MessageType::Reply, colour, direct, 
                            s.originC, s.originR, s.destC, s.destR, false}); //success, return a Empty piece reply
                    notifyObservers();
                    setPiece(s.colour, s.type);
                    //cout << "reply finish" << endl;
}
bool Piece::notify(Subject &whoFrom) {
	State lastcarrierS = whoFrom.getState(); //state of the previous piece
        Info last = whoFrom.getInfo(); //Info of the previous piece
        // check for initial call;
        if (lastcarrierS.success) return false; // the original piece has already moved successfully, terminate
        if (last.coordC == c && last.coordR == r) {
		//cout << "Called self notify" << endl;
                //cout << "r: " << r << endl;
                //cout << "c: " << c << endl;
		notifyObservers();
                return this->getState().success; // If the process succeeded 
	}
        if (this->getState().message == MessageType::NewMove && lastcarrierS.message == MessageType::Reply) { //A successful move in which the message is sent out and received back
            setPiece(Colour::NoColour, PieceType::Empty);
            //cout << "Success" << endl;
            this->setState(State{lastcarrierS.type, MessageType::Reply, colour, Direction::Other, // garbage value
                            lastcarrierS.originC, lastcarrierS.originR, lastcarrierS.destC, lastcarrierS.destR, true}); //successful move, clear the original state.
            if (this->getState().success) //cout << "state set true" << endl;
            //cout << "r: " << r << endl;
            //cout << "c: " << c << endl;
            return false;
        }
        
        Direction mydirect = getdirect(last.coordR,r, last.coordC,c);
        
        ////////////////////////////////////////////////////////first notify//////////////////////////////////////////////////////////////////////////
        if (lastcarrierS.message == MessageType::NewMove) { 
            
            
            //cout << "First new NewMove" << endl;
            
            if (lastcarrierS.type == PieceType::Queen || lastcarrierS.type == PieceType::Bishop || lastcarrierS.type == PieceType::Rook || lastcarrierS.type == PieceType::King) {
                
                /////////////////////////////////////////////Checking for valid first move//////////////////////////////////////////////////////////
                
                
                if  (mydirect == Direction::Other) return false; // disable observers for knight
                
                if (this->colour == lastcarrierS.colour) return false; // blockage by friendly
                    
                if (lastcarrierS.type == PieceType::Bishop && mydirect != Direction::NW && mydirect != Direction::NE && 
                            mydirect != Direction::SW && mydirect != Direction::SE) return false; // Only transmit the message diagonally for bishop
                    
                if (lastcarrierS.type == PieceType::Rook && mydirect != Direction::N && mydirect != Direction::S && mydirect != Direction::W && 
                            mydirect != Direction::E) return false; // Only transmit the message  vertically/horizontally for rook
                
                /////////////////////////////////////////////Checking for destination//////////////////////////////////////////////////////////
                
                if (r == lastcarrierS.destR && c == lastcarrierS.destC) { //reached destination
                    replyMessage(lastcarrierS, reverse(mydirect)); //success, return a reply
                    
                    return false;
                } else { //not the destination
                    if (pieceType != PieceType::Empty) return false; // Blockage by enemy
                    if (lastcarrierS.type == PieceType::King) return false; // King has limited moving space, thus disable relaying beyond the initial piece's observers
                    this->setState( State{lastcarrierS.type, MessageType::Relay, lastcarrierS.colour, mydirect, 
                            lastcarrierS.originC, lastcarrierS.originR, lastcarrierS.destC, lastcarrierS.destR, false}); // Relay message
                            //cout << "relaying" << endl;
                            //cout << "r: " << r << endl;
                            //cout << "c: " << c << endl;
                }
             ////////////////////////////////////////////////////////////////Checking for knight////////////////////////////////////////////////////////////////
            
            } else if (lastcarrierS.type == PieceType::Knight) {
                
                /////////////////////////////////////////////Disable observers not for knight/////////////////////////////////////////////////////////////////
                
                if  (mydirect != Direction::Other) return false;
                
                /////////////////////////////////////////////Checking for destination/////////////////////////////////////////////////////////////////////////
                
                if (r == lastcarrierS.destR && c == lastcarrierS.destC) { //reached destination
                    if (this->colour == lastcarrierS.colour) return false; // Blockage by friendly
                    replyMessage(lastcarrierS, reverse(mydirect)); //success, return a reply
                    return false; 
                } else return false; //does not call notify(relay) on its observers because knight has limited moving space
                
            } else { //////////////////////////////////////////////////////// Checking for pawn /////////////////////////////////////////////////////////////////
                
                //cout << "Pawn notify called" << endl;
                
                //////////////////////////////////////////////////Limit observers to be notified////////////////////////////////////////////////////////////
                
                if  (mydirect == Direction::Other) return false; // disable observers for knight
                if (lastcarrierS.colour == Colour::White) {
                    if (r != (lastcarrierS.originR - 1)) return false; // only notify the forward row of observers
                } else {
                    if (r != (lastcarrierS.originR + 1)) return false; // only notify the forward row of observers
                }
                if (this->colour == lastcarrierS.colour) return false; // blockage by friendly
                ///////////////////////////////////////////////////////////////Checking for destination//////////////////////////////////////////////////////////////
                
                if (r == lastcarrierS.destR && c == lastcarrierS.destC) { //reached destination
                    //cout << "Reached1" << endl;
                    if (lastcarrierS.colour == Colour::White) {
                        if ((this->pieceType == PieceType::Empty && mydirect != Direction::N) || (this->pieceType != PieceType::Empty && mydirect == Direction::N))return false;
                    } else {
                        if ((this->pieceType == PieceType::Empty && mydirect != Direction::S) || (this->pieceType != PieceType::Empty && mydirect == Direction::S))return false;
                    }
                    
                    replyMessage(lastcarrierS, reverse(mydirect)); //success, return a reply in the opposite direction
                    return false; 
                } else {
                    if (lastcarrierS.colour == Colour::White) {
                        if (mydirect != Direction::N) return false; // out of the valid move area
                        if (lastcarrierS.originR != 6) return false; // check for first move
                    }
                    else {
                        if (mydirect != Direction::S) return false; // out of the valid move area
                        if (lastcarrierS.originR != 1) return false; // check for first move
                    }
                    //cout << "Pawn relay called" << endl;
                    //cout << "r: " << r << endl;
                            //cout << "c: " << c << endl;
                    this->setState( State{lastcarrierS.type, MessageType::Relay, lastcarrierS.colour, mydirect, 
                        lastcarrierS.originC, lastcarrierS.originR, lastcarrierS.destC, lastcarrierS.destR, false}); // Relay message
                    
                }; //does not call notify(relay) on its observers because knight has limited moving space
            }
            
        ////////////////////////////////////////////////////////Relaying (no relay for knight/king)//////////////////////////////////////////////////////////////////////////
            
        } else if (lastcarrierS.message == MessageType::Relay) {
            
            if  (mydirect != lastcarrierS.dir) return false; //check if it is on the same line as the previous piece.
            
            if (r == lastcarrierS.destR && c == lastcarrierS.destC) { //reached destination
                if (this->colour == lastcarrierS.colour) return false; // Blockage by friendly
                if (lastcarrierS.type == PieceType::Pawn && pieceType != PieceType::Empty) return false; // 
                replyMessage(lastcarrierS, reverse(mydirect)); //success, return a reply
                return false;
            } else {
                if (pieceType != PieceType::Empty) return false; // Blockage by enemy
                if (lastcarrierS.type == PieceType::Pawn) return false; // 
                this->setState( State{lastcarrierS.type, MessageType::Relay, lastcarrierS.colour, mydirect, 
                        lastcarrierS.originC, lastcarrierS.originR, lastcarrierS.destC, lastcarrierS.destR, false}); // Relay message
                        //cout << "relaying" << endl;
                            //cout << "r: " << r << endl;
                            //cout << "c: " << c << endl;
            }
            
        ////////////////////////////////////////////////////////Replying//////////////////////////////////////////////////////////////////////////    
            
        } else if (lastcarrierS.message == MessageType::Reply) { //Replying the message
            if (lastcarrierS.type == PieceType::Queen || lastcarrierS.type == PieceType::Bishop || lastcarrierS.type == PieceType::Rook || lastcarrierS.type == PieceType::Pawn ) {
                if  (mydirect != lastcarrierS.dir) return false; //check if it is on the same line (if it is on the correct path) as the previous piece.
                else {
                    if (pieceType != PieceType::Empty) return false; // Checking for blockage. THIS SHOULD NOT BE EXECUTED SINCE REPLY SHOULD ONLY HAPPEN ON A PIECELESS PATH
                    this->setState( State{lastcarrierS.type, MessageType::Reply, lastcarrierS.colour, mydirect, 
                            lastcarrierS.originC, lastcarrierS.originR, lastcarrierS.destC, lastcarrierS.destR, false}); //success, return a Empty piece reply
                            //cout << "replying" << endl;
                            //cout << "r: " << r << endl;
                            //cout << "c: " << c << endl;
                    notifyObservers();      
                    this->setState( State{lastcarrierS.type, MessageType::Reply, lastcarrierS.colour, reverse(mydirect), 
                    lastcarrierS.originC, lastcarrierS.originR, lastcarrierS.destC, lastcarrierS.destR, false}); // Set my direction back to original to prevent calling notify on other piece
                    return false;
                }
            } else return false; 
            // a piece should not receive a reply from knight or king unless the piece is the original piece, which is checked at the beginning
        }
        notifyObservers();
        return false;
}
Info Piece::getInfo() const {
	return Info{r,c,colour,pieceType};
}
