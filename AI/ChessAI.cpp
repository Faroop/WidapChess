
#include "ChessAI.h"

namespace chess
{

ChessAI::ChessAI()
{
	err.setPrefix("ChessAI error: ");
	reportErrors=false;
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
		
	copyInData(game);
	
	if (gameOver)
	{
		err << "nextMove() caalled after game is over" << err;
		return false;
	}
	
	return findBestMove(true, iterNum);
}

double ChessAI::findBestMove(bool playIt, int iter)
{
	int i;
	Square s, e;
	
	for (i=(color==WHITE)?0:16; i<(color==WHITE)?16:32; ++i)
	{
		if (pieces[i].alive)
		{
			s=pieces[i].square;
			
			for (e.y=0; e.y<8; ++e.y)
			{
				for (e.x=0; e.x<8; ++e.x)
				{
					if (checkMovePath(s, e) && checkCheck(colorToMove, s, e))
					{
						return game->playMove(s, e);
					}
				}
			}
		}
	}
	
	return 0;
}

double ChessAI::eval()
{
	return 0;
}

}

