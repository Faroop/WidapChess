#include "ChessGame.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using widap::err;

namespace chess
{

Square::Square(int xIn, int yIn)
{
	x=xIn;
	y=yIn;
	clamp();
}

Square::Square(string str)
{
	if (str.length()!=2)
	{
		err << "Square sent string \"" << str << "\" with wrong length" << err;
		x=0;
		y=0;
	}
	
	if (str[0]>='A' && str[0]<='H')
		x=str[0]-'A';
	else if (str[0]>='a' && str[0]<='h')
		x=str[0]-'a';
	else
	{
		err << "Square sent string \"" << str << "\" with bad first character" << err;
		x=0;
	}
		
	if (str[1]>='1' && str[1]<='8')
		y=str[1]-'1';
	else
	{
		err << "Square sent string \"" << str << "\" with bad second character" << err;
		y=0;
	}
}

string Square::str()
{
	string out;
	
	out+=(char)(x+'A');
	out+=(char)(y+'1');
	
	return out;
}

void Square::clamp()
{
	if (x<0)
	{
		err << "x too low in square" << err;
		x=0;
	}
	else if (x>7)
	{
		err << "x too high in square" << err;
		x=7;
	}
	
	if (y<0)
	{
		err << "y too low in square" << err;
		y=0;
	}
	else if (y>7)
	{
		err << "y too high in square" << err;
		y=7;
	}
}

}

namespace chess
{

Game::Game()
{
	err.setPrefix("Chess game: ");
	setupBoard();
}

void Game::setupBoard()
{
	int i=0;
	
	for (int color=0; color<2; ++color)
	{
		int y=color*7;
		PieceColor c=(PieceColor)color;
		
		pieces[i++]=Piece(c, KING, Square(4, y));
		pieces[i++]=Piece(c, QUEEN, Square(3, y));
		pieces[i++]=Piece(c, ROOK, Square(0, y));
		pieces[i++]=Piece(c, ROOK, Square(7, y));
		pieces[i++]=Piece(c, KNIGHT, Square(1, y));
		pieces[i++]=Piece(c, KNIGHT, Square(6, y));
		pieces[i++]=Piece(c, BISHOP, Square(2, y));
		pieces[i++]=Piece(c, BISHOP, Square(5, y));
		
		for (int j=0; j<8; ++j)
		{
			pieces[i++]=Piece(c, PAWN, Square(j, color*5+1));
		}
	}
	
	for (int x=0; x<8; ++x)
	{
		for (int y=0; y<8; ++y)
		{
			boardData[x][y]=0;
		}
	}
	
	for (int i=0; i<32; ++i)
	{
		board(pieces[i].square, pieces+i);
	}
}

PieceColor Game::getWinner()
{
	return NO_COLOR;
}

Piece Game::getPiece(Square square)
{
	if (board(square))
	{
		return *board(square);
	}
	else
	{
		return Piece();
	}
}

bool Game::playMove(Square s, Square e)
{
	s.clamp();
	e.clamp();
	
	if (!board(s))
	{
		err << "no piece at source " << s.str() << err;
		return 0;
	}
	
	if (checkMovePath(s, e))
	{
		forceMove(s, e);
		history.insert(history.end(), {s, e, *board(e)});
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::checkMovePath(Square s, Square e)
{
	//get piece
	Piece p=*board(s);
	
	//all pawn movement
	if (p.type==PAWN)
	{
		//forward pawn movement
		if (s.x==e.x)
		{
			if (s.y+(p.color==WHITE?1:-1)==e.y || (s.y==(p.color==WHITE?1:6) && e.y==(p.color==WHITE?3:4)))
			{
				if (!board(e))
					return true;
				else
				{
					err << "pawns can only capture digonally" << err;
					return false;
				}
			}
			else
			{
				err << "pawn can only move one space forward at a time (or two if it's its first move)" << err;
				return false;
			}
		}
		//normal pawn capture
		else if (s.x+1==e.x || s.x-1==e.x)
		{
			//check y position
			if (s.y+(p.color==WHITE?1:-1)==e.y)
			{
				//normal capture
				if (board(e) && board(e)->color!=p.color)
				{
					return true;
				}
				//en passante
				else if (e.y==(p.color==WHITE?5:2) && board(Square(e.x, (p.color==WHITE?4:3))))
				{
					Square e0=history.end()->e;
					if (e0.x==e.x && e0.y==(p.color==WHITE?4:3))
					{
						//move pawn back so it will be captured
						forceMove(Square(e.x, (p.color==WHITE?4:3)), e);
						return true;
					}
					else
					{
						err << "en passante only works if the other pawn just moved" << err;
						return false;
					}
				}
				else
				{
					err << "pawns can move diagonally only when capturing" << err;
					return false;
				}
			}
			else
			{
				err << "pawn must always move and capture forward" << err;
			}
		}
		else
		{
			err << "pawn must move straight or digaonal" << err;
		}
	}
	
	return false;
}

void Game::forceMove(Square s, Square e)
{
	if (board(e))
		board(e)->alive=0;
	board(e, board(s));
	board(s, 0);
	board(e)->square=e;
}

}

