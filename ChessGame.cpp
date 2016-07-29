#include "ChessGame.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

namespace chess
{

Square::Square(int xIn, int yIn)
{
	x=xIn;
	y=yIn;
	valid=true;
	clamp();
}

Square::Square(string str)
{
	if (str.length()!=2)
	{
		err << "Square sent string \"" << str << "\" with wrong length" << err;
		x=0;
		y=0;
		valid=false;
	}
	
	if (str[0]>='A' && str[0]<='H')
		x=str[0]-'A';
	else if (str[0]>='a' && str[0]<='h')
		x=str[0]-'a';
	else
	{
		err << "Square sent string \"" << str << "\" with bad first character" << err;
		x=0;
		valid=false;
	}
		
	if (str[1]>='1' && str[1]<='8')
		y=str[1]-'1';
	else
	{
		err << "Square sent string \"" << str << "\" with bad second character" << err;
		y=0;
		valid=false;
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
		valid=false;
		x=0;
	}
	else if (x>7)
	{
		err << "x too high in square" << err;
		valid=false;
		x=7;
	}
	
	if (y<0)
	{
		err << "y too low in square" << err;
		valid=false;
		y=0;
	}
	else if (y>7)
	{
		err << "y too high in square" << err;
		valid=false;
		y=7;
	}
}

std::string pieceType2Name(PieceType type)
{
	switch (type)
	{
	case KING:
		return "king";
		break;
		
	case QUEEN:
		return "queen";
		break;
		
	case ROOK:
		return "rook";
		break;
		
	case KNIGHT:
		return "knight";
		break;
		
	case BISHOP:
		return "bishop";
		break;
		
	case PAWN:
		return "pawn";
		break;
	}
	
	return "[no piece name]";
}

std::string pieceColor2Name(PieceColor color)
{
	switch (color)
	{
	case WHITE:
		return "white";
		break;
		
	case BLACK:
		return "black";
		break;
		
	default:
		return "no color";
	}
}

}

namespace chess
{

Game::Game()
{
	err.setPrefix("Chess game: ");
	pieceToPromoteTo[0]=pieceToPromoteTo[1]=QUEEN;
	colorToMove=WHITE;
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
	
	if (s.x==e.x && s.y==e.y)
	{
		err << "start and end squares are the same" << err;
		return false;
	}
	
	if (!board(s))
	{
		err << "no piece on starting square" << err;
		return false;
	}
	
	if (board(s)->color!=colorToMove)
	{
		err << "it is " << pieceColor2Name(colorToMove) << "'s move" << err;
		return false;
	}
	
	if (checkMovePath(s, e))
	{
		forceMove(s, e);
		history.insert(history.end(), {s, e, *board(e)});
		if (colorToMove==WHITE)
			colorToMove=BLACK;
		else
			colorToMove=WHITE;
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
	
	if (board(e) && board(e)->color==p.color)
	{
		err << "you cannot move onto a square already occupied by one of your pieces" << err;
		return false;
	}
	
	//all pawn stuff
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
				if (board(e))
				{
					return true;
				}
				//en passante
				else if (e.y==(p.color==WHITE?5:2))
				{
					Piece * p0=board(Square(e.x, (p.color==WHITE?4:3)));
					
					if (p0 && p0->type==PAWN && p0->color!=p.color && !history.empty())
					{
						Square e0=history.back().e;
						
						if (e0.x==e.x && e0.y==(p.color==WHITE?4:3))
						{
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
					err << "pawns can move diagonally only when capturing" << err;
					return false;
				}
			}
			else
			{
				err << "pawn must always move and capture forward" << err;
				return false;
			}
		}
		else
		{
			err << "pawn must move straight or digaonal" << err;
			return false;
		}
	}
	
	if (p.type==KNIGHT)
	{
		if ((abs(s.x-e.x)==2 && abs(s.y-e.y)==1) || (abs(s.x-e.x)==1 && abs(s.y-e.y)==2))
		{
			return true;
		}
		else
		{
			err << "knight must move in an 'L' shape" << err;
			return false;
		}
	}
	
	if (p.type==KING)
	{
		if (abs(s.x-e.x)<=1 && abs(s.y-e.y)<=1)
		{
			return true;
		}
		//castling
		else if (s.y==e.y && s.y==(p.color==WHITE?0:7) && abs(e.x-s.x)==2)
		{
			Square rookSqr((s.x<e.x?0:7), s.y);
			
			if (board(rookSqr) && board(rookSqr)->type==ROOK && board(rookSqr)->color==p.color)
			{
				std::list<Move>::const_iterator i;
				
				for (i=history.begin(); i!=history.end(); ++i)
				{
					Square sqr=i->s;
					if (sqr.x==s.x && sqr.y==s.y)
					{
						err << "king has moved and therefor you cannot castle" << err;
						return false;
					}
					
					if (sqr.x==rookSqr.x && sqr.y==rookSqr.y)
					{
						err << "rook has moved and therefor you cannot castle" << err;
						return false;
					}
				}
				
				return true;
			}
			else
			{
				err << "rook is not in position to castle" << err;
			}
		}
		else
		{
			err << "king can only move one space at a time with the exception of castling" << err;
			return false;
		}
	}
	
	//straight moves
	if (s.x==e.x || s.y==e.y)
	{
		if (p.type==ROOK || p.type==QUEEN)
		{
			Piece * p0;
			
			if (s.x==e.x)
			{
				if (s.y<e.y)
				{
					for (int i=s.y+1; i<e.y; ++i)
					{
						if ((p0=board(Square(s.x, i))))
						{
							err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
					
					return true;
				}
				else
				{
					for (int i=s.y-1; i>e.y; --i)
					{
						if ((p0=board(Square(s.x, i))))
						{
							err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
						
					return true;
				}
			}
			else
			{
				if (s.x<e.x)
				{
					for (int i=s.x+1; i<e.x; ++i)
					{
						if ((p0=board(Square(i, s.y))))
						{
							err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
						
					return true;
				}
				else
				{
					for (int i=s.x-1; i>e.x; --i)
					{
						if ((p0=board(Square(i, s.y))))
						{
							err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
						
					return true;
				}
			}
		}
		else
		{
			err << pieceType2Name(p.type) << " can not move straight" << err;
			return false;
		}
	}
	
	//digonal moves
	if (abs(s.x-e.x)==abs(s.y-e.y))
	{
		if (p.type==BISHOP || p.type==QUEEN)
		{
			Piece * p0;
			
			if ((s.x>e.x)==(s.y>e.y))
			{
				if (s.y<e.y)
				{
					for (int i=1; i<e.x-s.x; ++i)
					{
						if ((p0=board(Square(s.x+i, s.y+i))))
						{
							err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
					
					return true;
				}
				else
				{
					for (int i=1; i<s.x-e.x; ++i)
					{
						if ((p0=board(Square(s.x-i, s.y-i))))
						{
							err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
					
					return true;
				}
			}
			else
			{
				if (s.x<e.x)
				{
					for (int i=1; i<e.x-s.x; ++i)
					{
						if ((p0=board(Square(s.x+i, s.y-i))))
						{
							err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
					
					return true;
				}
				else
				{
					for (int i=1; i<s.x-e.x; ++i)
					{
						if ((p0=board(Square(s.x-i, s.y+i))))
						{
							err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
					
					return true;
				}
			}
		}
		else
		{
			err << pieceType2Name(p.type) << " can not move diagonal" << err;
			return false;
		}
	}
	
	err << "invalid move, sorry I can't tell you more" << err;
	return false;
}

void Game::forceMove(Square s, Square e)
{
	if (board(e))
		board(e)->alive=0;
	
	//en passante
	if (board(s)->type==PAWN && s.x!=e.x && !board(e) && e.y==(board(s)->color==WHITE?5:2))
	{
		Square captSq=Square(e.x, (board(s)->color==WHITE?4:3));
		if (board(captSq) && board(captSq)->type==PAWN && board(captSq)->color!=board(s)->color)
		{
			board(captSq)->alive=0;
			board(captSq, 0);
		}
	}
	
	//promotion to queen (asking which piece to promote to will come much later in development)
	if (board(s)->type==PAWN && e.y==(board(s)->color==WHITE?7:0))
	{
		board(s)->type=pieceToPromoteTo[board(s)->color];
	}
	
	//castling
	if (board(s)->type==KING && abs(e.x-s.x)==2)
	{
		Piece * p=board(Square(s.x<e.x?7:0, s.y));
		
		if (p && p->type==ROOK)
		{
			forceMove(p->square, Square(s.x+(s.x<e.x?1:-1), s.y));
		}
	}
	
	board(e, board(s));
	board(s, 0);
	board(e)->square=e;
}

string Game::historyToStr()
{
	string out;
	std::list<Move>::const_iterator iterator;
	
	for (iterator=history.begin(); iterator!=history.end(); ++iterator)
	{
		Move mv=*iterator;
		out+=pieceColor2Name(mv.p.color);
		out+=" ";
		out+=pieceType2Name(mv.p.type);
		out+=" from ";
		out+=mv.s.str();
		out+=" to ";
		out+=mv.e.str();
		out+="\n";
	}
	
	return out;
}

}

