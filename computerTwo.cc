#include "computerTwo.h"
using namespace std;

ComputerLevelTwo::ComputerLevelTwo(Colour colour, ChessBoard &b):
	Player{colour,0,"computer[2]",false}, b{b}{}
void ComputerLevelTwo::printMoves(){
	for(auto &it: moveList){
		cout << "Valid Move: " << it.origin << " to " << it.dest << endl;
	}
	if(checkableMoves.empty()){
		cout << "No checkable moves" << endl;
	}
	if(attackablePieces.empty()){
		cout << "No attackble moves" << endl;
	}
	for(auto &it: checkableMoves){
        cout << "Check: " << it.second.second << " to "; 
		cout << b.toString(it.second.first.coordC,it.second.first.coordR);
		cout << " " <<  b.toString(b.getInfoAt(it.second.first.coordR,it.second.first.coordC).pieceType);
        cout << ". Value: " << it.first << endl;
	}
	cout << endl;
	for(auto &it: attackablePieces){
        cout << "Attack: " << it.second.second << " to "; 
		cout << b.toString(it.second.first.coordC,it.second.first.coordR);
		cout << " " << b.toString(b.getInfoAt(it.second.first.coordR,it.second.first.coordC).pieceType);
        cout << ". Value: " << it.first << endl;
    }
	cout << endl;
}
bool ComputerLevelTwo::checkCheck(int r, int c, Info enemyKing){
	//atR and atC are the coordinates of the attacking piece
	//toR and toC are the coordinates where the attacking piece is attacking
	for(int atR = 0; atR < 8; atR++){
		for(int atC = 0; atC < 8; atC++){
			for(int toR = 0; toR < 8; toR++){
				for(int toC = 0; toC < 8; toC++){
					//Current is able to move to destination
					if(b.move(atR,atC,toR,toC,colour, true)){						
						//Gets Info of destination
						Info toPiece = b.getInfoAt(toR,toC);
						//I might as well add this to my list of valid movemovents
		                Move m {b.toString(atC,atR), b.toString(toC,toR), colour};
			            moveList.emplace_back(m);
						//The move causes a check on the enemy
						if(b.checkforcapture(enemyKing)){
							int value = getPieceValue(toPiece);
							pair<Info, string> moveInfo(toPiece, b.toString(atC,atR));
							pair<int, pair<Info, string>> checked(value,moveInfo);
							checkableMoves.emplace_back(checked);
						}
						b.undo();
					}
				}
			}
		}
	}
	return true;
}
bool ComputerLevelTwo::checkCapture(int r,int c, Info selectedPiece){
	//Loops through each piece
	for(int i = 0; i < 8; i++){
		for(int ii = 0; ii < 8; ii++){
				//A piece is able to attack the selectedPiece
				if (b.move(i,ii,r,c,colour,true)) {
					b.undo();
					int value = getPieceValue(selectedPiece);
					if(value != 0){
						pair<Info, string> moveInfo(selectedPiece, b.toString(ii,i));
						pair<int, pair<Info, string>> enemyPiece(value,moveInfo);
						attackablePieces.emplace_back(enemyPiece);
					}
			}
		}
	}
	return false;
}
Move ComputerLevelTwo::nextMove(){
	//Invalid bit will be off when a new turn started
	if(!isInvalid){
		//Empties old set of movements
		attackablePieces.clear();
		checkableMoves.clear();
		moveList.clear();
		
		Colour opponent = Colour::Black;
		if(colour == Colour::Black) opponent = Colour::White;
		//Determines which piece is attackable
		for(int r = 0; r < 8; r++){
			for(int c = 0; c < 8; c++){
				Info selectedPiece = b.getInfoAt(r,c);
				if(selectedPiece.colour == opponent){
					checkCapture(r,c,selectedPiece);
					if(selectedPiece.pieceType == PieceType::King){
						checkCheck(r,c,selectedPiece);
					}
				}	
			}
		}
		//Sorts checkableMoves in terms of highest value captured
        sort(checkableMoves.begin(), checkableMoves.end(),
            [](pair<int,pair<Info,string>>&val1, pair<int,pair<Info,string>>&val2){
            return val1.first < val2.first;
        });
		//Sorts attackablePieces in terms of highest value captured
		sort(attackablePieces.begin(), attackablePieces.end(),
			[](pair<int,pair<Info,string>>&val1, pair<int,pair<Info,string>>&val2){
			return val1.first < val2.first;
		});
	}
		if(debug) printMoves();
		//moveList is empty, so it is either a checkmate or stalemate
		if(moveList.empty()){
			Move m = {"checkmate","stalemate",colour};
			return m;
		}
		//No moves that can check enemy king or attackable pieces
		if(checkableMoves.empty() && attackablePieces.empty()){
			cout << "Generating a random move" << endl;
		    std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<int> generate(0,moveList.size()-1);
			int r = generate(rng);
			Move m = moveList.at(r);
			moveList.pop_back();
			return m;
		//We prioritize checking over capturing
		}else if(checkableMoves.empty()){
			cout << "Capture available" << endl;
			string origin = attackablePieces.back().second.second;
			string dest = b.toString(attackablePieces.back().second.first.coordC,
							attackablePieces.back().second.first.coordR);
			Move m {origin, dest, colour};
			attackablePieces.pop_back();
			return m; 
		//A piece is able to check the enemy King
		}else{
			cout << "Check available" << endl;
	        string origin = checkableMoves.back().second.second;
            string dest = b.toString(checkableMoves.back().second.first.coordC,
                            checkableMoves.back().second.first.coordR);
            Move m {origin, dest, colour};
            checkableMoves.pop_back();
			return m; 
		}
		
	return generateRandomMove();
}
