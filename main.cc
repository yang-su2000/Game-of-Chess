#include <iostream>
#include <sstream>
#include <string>
#include "game.h"
using namespace std;
//A Player can be one of typeOfPlayers, otherwise the input is invalid
bool checkValidPlayers(string playerOne, string playerTwo){
	string typeOfPlayers[5] = {"human", "computer[1]", "computer[2]", "computer[3]", "computer[4]"};
	bool checkPlayerOne = false;
	bool checkPlayerTwo = false;
	//Loops to check if each Player is one of typeOfPlayers
	for(int i = 0; i < 5; i++){
		if(playerOne == typeOfPlayers[i]){
			checkPlayerOne = true;
		}
		if(playerTwo == typeOfPlayers[i]){
			checkPlayerTwo = true;
		}
	}
	if(checkPlayerOne && checkPlayerTwo){
		return true;
	}
	//Error message
	cout << "Invalid player. Must be one of: ";
	for(int ii = 0; ii < 5; ii++){
		cout << typeOfPlayers[ii] << " ";
	}
	cout << endl;
	return false;
}
int main(){
	cout << " _____  _   _  _____ _____ _____ " << endl;
	cout << "/  __ \\| | | ||  ___/  ___/  ___|" << endl;
	cout << "| /  \\/| |_| || |__ \\ `--.\\ `--. " << endl;
	cout << "| |    |  _  ||  __| `--. \\`--. \\" << endl;
	cout << "| \\__/\\| | | || |___/\\__/ /\\__/ /" << endl;
	cout << " \\____/\\_| |_/\\____/\\____/\\____/" << endl;
	string whitePlayer;
	string blackPlayer;
	Game g;
	int rounds = 0;
	while(true){
		if(cin.eof()){
			cout << endl << "Session Finished" << endl;
			cin.ignore();
			break;
		}
		cout << endl;
		if(g.getGame()){
			cout << g.getCurrentPlayer()->colourToString() << "-Player's turn" << endl;
		}
		string command;
		cin >> command;
		if(command == "game"){
			//Game is running
			if(g.getGame()){
				cout << "A game is already running!" << endl;
				continue;
			}
			cin >> whitePlayer;	
			cin >> blackPlayer;	
			//Invalid input -- unknown Player Type
			if(!checkValidPlayers(whitePlayer, blackPlayer)){
				continue;
			}
			//Create the Players
			g.createPlayer(whitePlayer, Colour::White);
			g.createPlayer(blackPlayer, Colour::Black);
			cout << "A new game has been created!" << endl;
			cout << "White-Player is a " << g.getPlayer(Colour::White)->getType() << endl;
			cout << "Black-Player is a " << g.getPlayer(Colour::Black)->getType() << endl;
			g.setCurrentPlayer(Colour::White);
			g.setGame(true);
			rounds = 0;
			//Initialize a new ChessBoard
			g.initializeBoard();
			g.printBoard();
			continue;
		}else if(command == "print"){
			string printCommand;
			cin >> printCommand;
			//We can print the score, but we cannot print ChessBoard unless Game is initiated
			if(printCommand == "board" && g.getGame()) g.printBoard();
			else if(printCommand == "score") g.printScore(false);
			else cout << "Start game with: game white-player black-player" << endl;
			continue;
		}else if(!g.getGame()){
			cout << "Start game with: game white-player black-player" << endl;
			continue;
		}else if(command == "move"){
			//Player moves then board prints
			try{
				if(!g.playerMove(g.getCurrentPlayer())){
					g.printBoard();
					continue;
				}
			}catch(string e){
				cout << e << endl;
			}
			g.printBoard();
		}else if(command == "resign"){
			//Adds point to opponent and stops the game
			cout << g.getCurrentPlayer()->colourToString() << "-Player has resigned!" << endl;
			if(g.getCurrentPlayer()->getColour() == Colour::White){
				g.addScore(Colour::Black);
			}else{
				g.addScore(Colour::White);
			}
			g.setGame(false);
			
		}else if(command == "setup"){
			cout << "Setup has been issued. Enter \"clear\" to clear the board." << endl;
			g.printBoard();
			
			string setupCommand;
			while(true){
				cin >> setupCommand;
				//Player decides to stop
				if(cin.eof()){
					g.printScore(true);
					return 0;
				}
				if(setupCommand == "done"){
					try{
						//Checks if there is only 1 King of each colour
						//Checks if any Pawns are in the first or last row
						//Checks if any Kings are in check
						g.setupExit();
						cout << "Setup done" << endl;
						g.printBoard();
						break;
					}catch(string errorMessage){
						cout << errorMessage << endl;
					}
				}
				if(setupCommand == "clear"){
					cout << "The board has been cleared" << endl;
					g.clearBoard();
				}
				if(setupCommand == "+"){
					string piece;
					string coordinate;
					cin >> piece;
					cin >> coordinate;
					try{
						g.setupAdd(piece,coordinate);
					}catch(string errorMessage){
						cout << errorMessage << endl;
					}catch(...){
						cout << "Error: Bad input" << endl;
					}
				}else if(setupCommand == "-"){
					try{
					string coordinate;
					cin >> coordinate;
					
					g.setupRemove(coordinate);
					}catch(...){
						cout << "Error: Bad input" << endl;
						continue;
					}
				}else if(setupCommand == "="){
					string colour;
					cin >> colour;
					try{
						g.setupTurn(colour);
						cout << g.getCurrentPlayer()->colourToString() << "-Player will go next" << endl;
					}catch(string errorMessage){
						cout << errorMessage << endl;
					}
				}
				g.printBoard();
				continue;
			}
				continue;
		}else if(command == "undo"){
			g.undo();
			cout << "An undo has been called" << endl;
			g.printBoard();
		}else if(command == "history"){
			g.showHistory();
			g.nextTurn();
		}else{
			cout << command << " is an invalid input" << endl;
			continue;
		}
		g.nextTurn();
		rounds++;
	}
	g.printScore(true);
}
