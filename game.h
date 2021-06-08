#ifndef GAME_H
#define GAME_H
#include <string>
#include <memory>
#include "chessboard.h"
#include "player.h"
#include "human.h"
#include "computerOne.h"
#include "computerTwo.h"
#include "computerThree.h"
#include "computerFour.h"

class Game {
	ChessBoard theBoard;
	Colour playerToMove = Colour::Black; //Next Player
	bool gameInitiated = false;

	std::shared_ptr<Player> currentPlayer; //CurrentPlayer
	std::shared_ptr<Player> whitePlayer;
	std::shared_ptr<Player> blackPlayer;

	double whiteScore = 0;
	double blackScore = 0;

	public:	
	void createPlayer(std::string player, Colour colour); //Initializes player
	bool playerMove(std::shared_ptr<Player>); //Player movement
	void undo();

	int ctoint(char);
	int rtoint(char);
	bool checkState();

	//Board
	void printBoard();
	void printScore(bool);
	void nextTurn();
	void clearBoard();
	void initializeBoard();
	void showHistory();

	//Setup
	void setupAdd(std::string piece, std::string coord);
	void setupRemove(std::string coord);
	void setupTurn(std::string colour);
	void setupExit();

	//Setters
	void setCurrentPlayer(Colour);
	void setGame(bool);
	void addScore(Colour);

	//Getters
	bool getGame();
	double getScore(Colour);
	std::shared_ptr<Player> getCurrentPlayer(); //Returns the current player
	std::shared_ptr<Player> getPlayer(Colour); //Returns the player of Colour;

};

#endif
