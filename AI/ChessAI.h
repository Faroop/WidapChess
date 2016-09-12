
#pragma once

#include "../ChessGame.h"
#include <string>
#include <list>
#include "../Error.h"

namespace chess
{

class ChessAI: protected Game
{
public:
	
	ChessAI();
	~ChessAI() {}
	
	void setup(Game * gameIn, PieceColor colorIn);
	void unsetup() {isSetUp=false;}
	bool nextMove();
	
	bool getIfSetUp() {return isSetUp;}
	
private:
	
	double findBestMove(bool playIt, int iter);
	double eval();
	
private:
	
	bool isSetUp=false;
	PieceColor color;
	Game * game;
	
	int iterNum=2;
	
	::Error err;
};

}
