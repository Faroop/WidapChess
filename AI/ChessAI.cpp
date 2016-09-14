
#include "ChessAI.h"
#include <list>
using std::list;

namespace chess
{

struct MoveData
{
	MoveData()=default;
	
	MoveData(Square sIn, Square eIn, double scoreIn)
	{
		s=sIn;
		e=eIn;
		score=scoreIn;
	}
	
	Square s, e;
	double score;
};

class MoveHolder: public list<MoveData>
{
public:
	
	MoveHolder(int maxSizeIn)
	{
		maxSize=maxSizeIn;
	}
	
	inline void add(MoveData in)
	{
		auto i=begin();
		
		while (i!=end() && (*i).score>in.score)
		{
			i++;
		}
		
		if (size()>=maxSize)
		{
			if (i==end())
			{
				return;
			}
			else
			{
				insert(i, in);
				pop_back();
			}
		}
		else
		{
			insert(i, in);
		}
	}
	
private:
	unsigned maxSize;
};

ChessAI::Settings::Settings()
{
	pieceValues[KING]=1; //giving it a really high score would just mess up the rest of the calculations
	pieceValues[QUEEN]=10;
	pieceValues[ROOK]=5;
	pieceValues[KNIGHT]=3.6;
	pieceValues[BISHOP]=3.4;
	pieceValues[PAWN]=1;
	
	checkDepth=3;
	checkWidth=3;
}

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
		myColor=colorIn;
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
	
	if (colorToMove!=myColor)
	{
		err << "called chessAI with wrong color to move" << err;
		return false;
	}
	
	err << "\n\n" << err;
	
	bool success=findBestMove(true, settings.checkDepth);
	
	return success;
}

double ChessAI::findBestMove(bool playIt, int iter)
{
	PieceColor color=colorToMove;
	
	if (iter==0)
	{
		double score=eval(color);
		
		for (int j=settings.checkDepth; j>iter; --j)
			err << "    ";
		
		err << "score for " << pieceColor2Name(color) << ": " << score << err;
		return score;
	}
	
	Square s, e;
	MoveHolder moves(settings.checkWidth);
	
	for (int i=((color==WHITE)?0:16); i<((color==WHITE)?16:32); ++i)
	{
		if (pieces[i].alive)
		{
			s=pieces[i].square;
			
			for (e.y=0; e.y<8; ++e.y)
			{
				for (e.x=0; e.x<8; ++e.x)
				{
					bool path=checkMovePath(s, e);
					
					bool check=checkCheck(colorToMove, s, e);
					
					if (path && check)
					{
						forceMove(s, e);
						moves.add(MoveData(s, e, eval(color)));
						undo();
						
						checkBoard();
					}
				}
			}
		}
	}
	
	if (moves.size()<1)
	{
		err << "failed to find valid move" << err;
		return 0;
	}
	
	MoveData bestMove;
	
	bestMove.score=-1;
	
	for (auto i=moves.begin(); i!=moves.end(); ++i)
	{
		for (int j=settings.checkDepth; j>iter; --j)
			err << "    ";
		
		err << pieceColor2Name(color) << " " << pieceType2Name(board((*i).s)->type) << " from " << (*i).s.str() << " to " << (*i).e.str() << err;
		
		forceMove((*i).s, (*i).e);
		double score=1-findBestMove(false, iter-1);
		undo();
		
		if (score>bestMove.score)
		{
			bestMove.s=(*i).s;
			bestMove.e=(*i).e;
			bestMove.score=score;
		}
		
		//err << pieceType2Name(board((*i).s)->type) << " at " << (*i).s.str() << " to " << (*i).e.str() << " has a score of " << (*i).score << err;
	}
	
	for (int j=settings.checkDepth; j>iter; --j)
			err << "    ";
		
	err << "best score for " << pieceColor2Name(color) << ": " << bestMove.score << err;
	
	//MoveData mv=*(moves.begin());
	
	//err << pieceColor2Name(myColor) << " chosing move with a score of " << mv.score << err;
	
	if (playIt)
	{
		return game->playMove(bestMove.s, bestMove.e);
	}
	else
	{
		return bestMove.score;
	}
}

inline double ChessAI::eval(PieceColor color)
{
	double myScore=evalForColor(color);
	double otherScore=evalForColor(getOtherColor(color));
	
	return myScore/(myScore+otherScore);
}

inline double ChessAI::evalForColor(PieceColor color)
{
	double score=0;
	
	for (int i=((color==WHITE)?0:16); i<((color==WHITE)?16:32); ++i)
	{
		if (pieces[i].alive)
			score+=settings.pieceValues[pieces[i].type];
	}
	
	return score;
}

}

