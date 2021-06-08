#include "player.h"

using namespace std;

Player::Player(Colour colour, double score, string type, bool isInvalid):
	colour{colour}, score{score}, type{type}, isInvalid{isInvalid}{}
int Player::getPieceValue(Info selectedPiece){
	int value = 0;
    if(selectedPiece.pieceType == PieceType::Pawn) value = 1;
    else if(selectedPiece.pieceType == PieceType::Bishop) value = 3;
    else if(selectedPiece.pieceType == PieceType::Knight) value = 3;
    else if(selectedPiece.pieceType == PieceType::Rook) value = 5;
    else if(selectedPiece.pieceType == PieceType::Queen) value = 10;
    else if(selectedPiece.pieceType == PieceType::King) value = 100;
	return value;
}
double Player::getScore(){
	return score;
}
Colour Player::getColour(){
	return colour;
}

string Player::getType(){
	return type;
}
string Player::colourToString(){
	if(colour == Colour::White) return "White";
	else return "Black";
}
string Player::translateToCoord(int c, int r){
	string col;
	string row;
    if(c == 0) col = "a";
    else if(c == 1) col = "b";
    else if(c == 2) col = "c";
    else if(c == 3) col = "d";
    else if(c == 4) col = "e";
    else if(c == 5) col = "f";
    else if(c == 6) col = "g";
    else if(c == 7) col = "h";
	else return "Error";

    if(r == 0) row = "8";
    else if(r == 1) row = "7";
    else if(r == 2) row = "6";
    else if(r == 3) row = "5";
    else if(r == 4) row = "4";
    else if(r == 5) row = "3";
    else if(r == 6) row = "2";
    else if(r == 7) row = "1";
	else return "Error";

	return col + row;
    
}
Move Player::generateRandomMove(){
    //Random number generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> generate(0,7); // distribution in range [1, 6]

    string translatedOrigin = translateToCoord(generate(rng), generate(rng));
    string translatedDest = translateToCoord(generate(rng), generate(rng));


    Move m{translatedOrigin, translatedDest, colour};

    return m;
}
void Player::setValid(bool status){
	isInvalid = status;
}
void Player::addScore(double points){
	score += points;
}
