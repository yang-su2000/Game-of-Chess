#include "computerFour.h"
using namespace std;
ComputerLevelFour::ComputerLevelFour(Colour colour, ChessBoard &b):
	Player{colour,0,"computer[3]",false}, b{b}{}
void ComputerLevelFour::toggleDebug(){
	debug = !debug;
}
void ComputerLevelFour::printMoves(){
	cout << "MY VALID MOVES ARE:" << endl;
    for(auto &it: moveList){
        cout << "Valid Move: " << it.origin << " to " << it.dest << endl;
    }
    if(retreatedPieces.empty()){
        cout << "No retreatable moves" << endl;
    }
    if(checkableMoves.empty()){
        cout << "No checkable moves" << endl;
    }
    if(attackablePieces.empty()){
        cout << "No attackble moves" << endl;
    }
	for(auto &it: retreatedPieces){
        cout << "Retreat: " << it.second.second << " to ";
        cout << b.toString(it.second.first.coordC,it.second.first.coordR);
        cout << " " <<  b.toString(b.getInfoAt(it.second.first.coordR,it.second.first.coordC).pieceType);
        cout << ". Value: " << it.first << endl;
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
void ComputerLevelFour::checkAvoid(int r, int c, Info selectedPiece){
	//My selectedPiece is in danger of being captured
	if(b.checkforcapture(selectedPiece)){
		int value = getPieceValue(selectedPiece);
        //I check all possible movements where my piece can retreat
            for(int toR = 0; toR < 8; toR++){
                for(int toC = 0; toC < 8; toC++){
                    //I am able to move this piece to a different tile
                    if(b.move(r, c, toR, toC, colour,true)){
                        Info newInfo{toR, toC, selectedPiece.colour, selectedPiece.pieceType};
                        //I will move selectPiece to [toC,toR] from [c,r] as a potential move
					    pair<Info, string> moveInfo(newInfo, b.toString(c,r));
                        pair<int, pair<Info, string>> retreatPiece(value,moveInfo);
                        //This is a move I would want to consider
                        retreatedPieces.emplace_back(retreatPiece);
                        b.undo();
                    }
                }
            }
        
	}
}
void ComputerLevelFour::checkCheck(int r, int c, Info enemyKing){
	//atR and atC are the coordinates of the attacking piece
	//toR and toC are the coordinates where the attacking piece is attacking
	//I will find a piece that I own
	for(int atR = 0; atR < 8; atR++){
		for(int atC = 0; atC < 8; atC++){
			//I will find a coordinate to move my piece to
			for(int toR = 0; toR < 8; toR++){
				for(int toC = 0; toC < 8; toC++){
					Info toPiece = b.getInfoAt(toR,toC);
					//I can move my piece to this tile
					if(b.move(atR,atC,toR,toC,colour,true)){						
					
						//I might as well add this move to my list of valid movemovents
						Move m {b.toString(atC,atR), b.toString(toC,toR), colour};
						moveList.emplace_back(m);
						//The enemy king will be in check if I move my piece
						cout << b.toString(enemyKing.pieceType) << b.toString(enemyKing.colour) << endl;
						if(b.checkforcapture(enemyKing)){
							int value = getPieceValue(toPiece);
							pair<Info, string> moveInfo(toPiece, b.toString(atC,atR));
							pair<int, pair<Info, string>> checked(value,moveInfo);
							//This is a move I would want to consider
							checkableMoves.emplace_back(checked);
						}
						b.undo();
					}
				}
			}
		}
	}
}
void ComputerLevelFour::checkCapture(int r,int c, Info selectedPiece){
	//I will find one of my pieces
	for(int i = 0; i < 8; i++){
		for(int ii = 0; ii < 8; ii++){
			//I can move one of my pieces to a valid tile, capturing it
			if (b.move(i,ii,r,c,colour,true)) {
				b.undo();
				int value = getPieceValue(selectedPiece);
				//The tile I moved to is not an Empty tile
				if(value != 0){
					pair<Info, string> moveInfo(selectedPiece, b.toString(ii,i));
					pair<int, pair<Info, string>> enemyPiece(value,moveInfo);
					//This is a move I would want to consider
					attackablePieces.emplace_back(enemyPiece);	
				}
			}
		}
	}
}
Move ComputerLevelFour::nextMove(){
	//My previous move was not invalid, so it must be a new turn for me
	if(!isInvalid){
		//I will forget all possible moves done last turn
		retreatedPieces.clear();
		attackablePieces.clear();
		checkableMoves.clear();
		moveList.clear();
		Colour opponent = Colour::Black;
		if(colour == Colour::Black) opponent = Colour::White;
		//I will analyze the board for certain pieces
		for(int r = 0; r < 8; r++){
			for(int c = 0; c < 8; c++){
				Info selectedPiece = b.getInfoAt(r,c);
				
				//This piece I see is an enemy piece
				if(selectedPiece.colour == opponent){
					checkCapture(r,c,selectedPiece);
					
					if(selectedPiece.pieceType == PieceType::King){
						checkCheck(r,c,selectedPiece);
					}
				//This piece I see is a friendly piece
				}else if(selectedPiece.colour == colour){
					checkAvoid(r,c,selectedPiece);
				}
			}
		}
		/*I will sort my potential moves based on the value of the pieces*/
		//I will prioritize retreating my higher valued pieces
		sort(retreatedPieces.begin(), retreatedPieces.end(),
            [](pair<int,pair<Info,string>>&val1, pair<int,pair<Info,string>>&val2){
            return val1.first < val2.first;
        });
		//I will try to capture a piece to put the enemy King in check
        sort(checkableMoves.begin(), checkableMoves.end(),
            [](pair<int,pair<Info,string>>&val1, pair<int,pair<Info,string>>&val2){
            return val1.first < val2.first;
        });
		//I will prioritize capturing the enemy's higher valued pieces
		sort(attackablePieces.begin(), attackablePieces.end(),
			[](pair<int,pair<Info,string>>&val1, pair<int,pair<Info,string>>&val2){
			return val1.first < val2.first;
		});
		
	}
		if(debug) printMoves();
		//I do not need to retreat, I can't check the enemy, and I can't capture an enemy
		if(retreatedPieces.empty() && checkableMoves.empty() && attackablePieces.empty()){
			if(debug)cout << "Generating a random move" << endl;
			//I will use <random> to generate a random number within my possible moves
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<int> generate(0,moveList.size()-1);
            int r = generate(rng);
			Move m = moveList.at(r);
			moveList.pop_back();
			return m;
		//I will capture if I cannot retreat or check the enemy
		}else if(retreatedPieces.empty() && checkableMoves.empty()){
			if(debug)cout << "Capture available" << endl;
			//I will target the highest valued piece
			string origin = attackablePieces.back().second.second;
			string dest = b.toString(attackablePieces.back().second.first.coordC,
							attackablePieces.back().second.first.coordR);
			Move m = {origin, dest, colour};
			//If this move isn't available, I will discard it
			attackablePieces.pop_back();
			return m; 
		//I will check if I cannot retreat
		}else if(retreatedPieces.empty()){
            if(debug)cout << "Check available" << endl;
            string origin = checkableMoves.back().second.second;
            string dest = b.toString(checkableMoves.back().second.first.coordC,
                            checkableMoves.back().second.first.coordR);
            Move m = {origin, dest, colour};
            checkableMoves.pop_back();
            return m;
		//I will retreat
		}else{
			if(debug)cout << "Retreat available" << endl;
	        string origin = retreatedPieces.back().second.second;
            string dest = b.toString(retreatedPieces.back().second.first.coordC,
                            retreatedPieces.back().second.first.coordR);
            Move m = {origin, dest, colour};
			cout << m.origin << " to " << m.dest << endl;
            retreatedPieces.pop_back();
			return m; 
		}
		
	//My isRandom is on, I will start randomly moving my pieces
	return generateRandomMove();
}
