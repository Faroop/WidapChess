
#include "ChessAI.h"

namespace chess
{

ChessAI::ChessAI()
{
	err.setPrefix("ChessAI error: ");
}

void ChessAI::setup(Game * gameIn, PieceColor colorIn)
{
	if (gameIn)
	{
		isSetUp=true;
		game=gameIn;
		color=colorIn;
	}
}

bool ChessAI::nextMove()
{
	Square s, e;
	
	if (!isSetUp)
	{
		err << "nextMove() called when ChessAI not yet set up" << err;
		return false;
	}
	
	s=Square(3, 1);
	e=Square(3, 3);
	
	return game->playMove(s, e);
}

double ChessAI::eval()
{
	//for (int i=0; i<)
	
	return 0;
}

}

