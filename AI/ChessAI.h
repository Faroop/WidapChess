
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
	
	struct Settings
	{
		Settings();
		
		double pieceValues[6];
		int checkDepth;
		int checkWidth;
	};
	
	ChessAI();
	~ChessAI() {}
	
	Settings settings;
	
	void setup(Game * gameIn, PieceColor colorIn);
	void unsetup() {isSetUp=false;}
	bool nextMove();
	
	bool getIfSetUp() {return isSetUp;}
	
private:
	
	double findBestMove(bool playIt, int iter);
	inline double eval(PieceColor color);
	inline double evalForColor(PieceColor color);
	
private:
	
	bool isSetUp=false;
	PieceColor myColor;
	Game * game;
	
	::Error err;
};

}
