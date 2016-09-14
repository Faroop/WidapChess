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
	valid=isInBoard();
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
		valid=false;
		x=0;
	}
	else if (x>7)
	{
		valid=false;
		x=7;
	}
	
	if (y<0)
	{
		valid=false;
		y=0;
	}
	else if (y>7)
	{
		valid=false;
		y=7;
	}
}

string Piece::str()
{
	return pieceType2Name(type) + " at " + square.str();
}

PieceColor getOtherColor(PieceColor in)
{
	if (in==WHITE)
		return BLACK;
	else if (in==BLACK)
		return WHITE;
	else
		return NO_COLOR;
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
	reset();
}

void Game::reset()
{
	pieceToPromoteTo[0]=pieceToPromoteTo[1]=QUEEN;
	colorToMove=WHITE;
	gameOver=false;
	winner=NO_COLOR;
	history.clear();
	setupBoard();
}

void Game::setupBoard()
{
	int i=0;
	
	history.clear();
	
	for (int color=0; color<2; ++color)
	{
		int y=color*7;
		PieceColor c=(PieceColor)color;
		
		kings[c]=&(pieces[i++]=Piece(c, KING, Square(4, y)));
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
	return winner;
}

bool Game::getIfGameOver()
{
	return gameOver;
}

Piece Game::getPiece(Square square)
{
	if (square.isInBoard() && board(square))
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
		if (reportErrors) err << "start and end squares are the same" << err;
		return false;
	}
	
	if (!board(s))
	{
		if (reportErrors) err << "no piece on starting square" << err;
		return false;
	}
	
	if (board(s)->color!=colorToMove)
	{
		if (reportErrors) err << "it is " << pieceColor2Name(colorToMove) << "'s move" << err;
		return false;
	}
	
	if (checkMovePath(s, e) && checkCheck(colorToMove, s, e))
	{
		forceMove(s, e);
		
		setWinState();
		
		checkBoard();
		
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::checkMovePath(Square s, Square e)
{
	if (!board(s))
	{
		err << "tried to move with a starting location that doesn't have a piece" << err;
		return false;
	}
	
	//get piece
	Piece p=*board(s);
	
	if (board(e) && board(e)->color==p.color)
	{
		if (reportErrors)
			if (reportErrors) err << "you cannot move onto a square already occupied by one of your pieces" << err;
		return false;
	}
	
	//all pawn stuff
	if (p.type==PAWN)
	{
		//forward pawn movement
		if (s.x==e.x)
		{
			//single space
			if (s.y+(p.color==WHITE?1:-1)==e.y)
			{
				if (!board(e))
					return true;
				else
				{
					if (reportErrors) err << "pawns can only capture digonally" << err;
					return false;
				}
			}
			//two spaces
			else if (s.y==(p.color==WHITE?1:6) && e.y==(p.color==WHITE?3:4))
			{
				if (board(e))
				{
					if (reportErrors) err << "pawns can only capture digonally" << err;
					return false;
				}
				else if (board(Square((p.color==WHITE?2:5), e.y)))
				{
					if (reportErrors) err << "piece is in the way" << err;
					return false;
				}
				else
					return true;
			}
			else
			{
				if (reportErrors) err << "pawn can only move one space forward at a time (or two if it's its first move)" << err;
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
							if (reportErrors) err << "en passante only works if the other pawn just moved" << err;
							return false;
						}
					}
					else
					{
						if (reportErrors) err << "pawns can move diagonally only when capturing" << err;
						return false;
					}
				}
				else
				{
					if (reportErrors) err << "pawns can move diagonally only when capturing" << err;
					return false;
				}
			}
			else
			{
				if (reportErrors) err << "pawn must always move and capture forward" << err;
				return false;
			}
		}
		else
		{
			if (reportErrors) err << "pawn must move straight or digaonal" << err;
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
			if (reportErrors) err << "knight must move in an 'L' shape" << err;
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
			Square rookSqr((s.x<e.x?7:0), s.y);
			
			if (board(rookSqr) && board(rookSqr)->type==ROOK && board(rookSqr)->color==p.color)
			{
				Square checkSquare((s.x<e.x)?s.x+1:s.x-1, s.y);
				
				while (checkSquare.x!=rookSqr.x)
				{
					if (board(checkSquare))
					{
						if (reportErrors) err << "castling blocked by " << pieceType2Name(board(checkSquare)->type) << " at " << checkSquare.str() << err;
						return false;
					}
					
					if (checkSquare.x>=std::min(s.x, e.x) && checkSquare.x<=std::max(s.x, e.x) && !checkCheck(p.color, s, checkSquare))
					{
						if (reportErrors) err << "king would be in ckeck at " << checkSquare.str() << ", so you can't castle" << err;
						return false;
					}
					
					if (s.x<e.x)
						++checkSquare.x;
					else
						--checkSquare.x;
					
				}
				
				for (auto i=history.begin(); i!=history.end(); i++)
				{
					Square sqr=(*i).s;
					
					if (sqr.x==s.x && sqr.y==s.y)
					{
						if (reportErrors) err << "king has moved and therefor you cannot castle" << err;
						return false;
					}
					
					if (sqr.x==rookSqr.x && sqr.y==rookSqr.y)
					{
						if (reportErrors) err << "rook has moved and therefor you cannot castle" << err;
						return false;
					}
				}
				
				return true;
			}
			else
			{
				if (reportErrors) err << "rook is not in position to castle" << err;
			}
		}
		else
		{
			if (reportErrors) err << "king can only move one space at a time with the exception of castling" << err;
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
							if (reportErrors) err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
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
							if (reportErrors) err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
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
							if (reportErrors) err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
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
							if (reportErrors) err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
						
					return true;
				}
			}
		}
		else
		{
			if (reportErrors) err << pieceType2Name(p.type) << " can not move straight" << err;
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
							if (reportErrors) err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
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
							if (reportErrors) err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
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
							if (reportErrors) err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
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
							if (reportErrors) err << pieceType2Name(p0->type) << " at " << p0->square.str() << " is in the way" << err;
							return false;
						}
					}
					
					return true;
				}
			}
		}
		else
		{
			if (reportErrors) err << pieceType2Name(p.type) << " can not move diagonal" << err;
			return false;
		}
	}
	
	if (reportErrors) err << "invalid move, sorry I can't tell you more" << err;
	return false;
}

bool Game::checkCheck(PieceColor kingColor, Square s, Square e)
{
	Square i, k;
	int j;
	
	int pawnX[]={-1, 1};
	
	int kingX[]={ 0,  1,  1,  1,  0, -1, -1, -1};
	int kingY[]={ 1,  1,  0, -1, -1, -1,  0,  1};
	
	int knightX[]={-2, -1,  1,  2,  2,  1, -1, -2};
	int knightY[]={ 1,  2,  2,  1, -1, -2, -2, -1};
	
	if (kings[colorToMove]->square==s)
		k=e;
	else
		k=kings[colorToMove]->square;
	
	for (j=0; j<8; ++j)
	{
		if (pieceWillBeInSquare(i=Square(k.x+kingX[j], k.y+kingY[j]), kingColor, KING, s, e))
		{
			if (reportErrors)
				if (reportErrors) err << "king at " << i.str() << " would put the king in check" << err;
			return false;
		}
	}
	
	for (j=0; j<8; ++j)
	{
		if (pieceWillBeInSquare(i=Square(k.x+knightX[j], k.y+knightY[j]), kingColor, KNIGHT, s, e))
		{
			if (reportErrors)
				if (reportErrors) err << "knight at " << i.str() << " would put the king in check" << err;
			return false;
		}
	}
	
	for (j=0; j<2; ++j)
	{
		if (pieceWillBeInSquare(i=Square(k.x+pawnX[j], k.y+(kingColor==WHITE?1:-1)), kingColor, PAWN, s, e))
		{
			if (reportErrors)
				if (reportErrors) err << "pawn at " << i.str() << " would put the king in check" << err;
			return false;
		}
	}
	
	if (
		!checkLine(kingColor, k,  1,  0, s, e) ||
		!checkLine(kingColor, k,  1,  1, s, e) ||
		!checkLine(kingColor, k,  0,  1, s, e) ||
		!checkLine(kingColor, k, -1,  1, s, e) ||
		!checkLine(kingColor, k, -1,  0, s, e) ||
		!checkLine(kingColor, k, -1, -1, s, e) ||
		!checkLine(kingColor, k,  0, -1, s, e) ||
		!checkLine(kingColor, k,  1, -1, s, e)
		)
	{
		return false;
	}
	
	
	
	return true;
}

Piece * Game::pieceWillBeInSquare(Square i, PieceColor ignoreColor, PieceType type, Square s, Square e)
{
	Piece * ptr;
	
	if (!i.isInBoard())
		ptr=0;
	else if (i==e)
		ptr=board(s);
	else if (i==s)
		ptr=0;
	else
		ptr=board(i);
	
	if (ptr && ptr->color!=ignoreColor && ptr->type==type)
		return ptr;
	else
		return 0;
}

bool Game::checkLine(PieceColor friendlyColor, Square i, int vx, int vy, Square s, Square e)
{
	Piece p, * ptr;
	
	 while (true)
	{
		i.x+=vx;
		i.y+=vy;
		
		if (!i.isInBoard())
			break;
		
		if (!(ptr=pieceWillBeInSquare(i, friendlyColor, QUEEN, s, e)))
		{
			if (vx==0 || vy==0)
				ptr=pieceWillBeInSquare(i, friendlyColor, ROOK, s, e);
			else
				ptr=pieceWillBeInSquare(i, friendlyColor, BISHOP, s, e);
		}
		
		if (ptr)
		{
			if (reportErrors)
				if (reportErrors) err << pieceType2Name(ptr->type) << " at " << i.str() << " would put the king in check" << err;
			return false;
		}
		else
		{
			if (i==e || (!(i==s) && board(i)))
				break;
		}
	}
	
	return true;
}

void Game::setWinState()
{
	int i;
	Square j, k;
	bool moveFound=false;
	bool reportErrorsWasOn=reportErrors;
	reportErrors=false;
	
	for (i=(colorToMove==WHITE?0:16); i<(colorToMove==WHITE?16:32) && !moveFound; ++i)
	{
		if (pieces[i].alive)
		{
			k=pieces[i].square;
			
			for (j.y=0; j.y<8 && !moveFound; ++j.y)
			{
				for (j.x=0; j.x<8 && !moveFound; ++j.x)
				{
					if (checkMovePath(k, j) && checkCheck(colorToMove, k, j))
					{
						moveFound=true;
						//err << pieces[i].str() << " to " << j.str() << err;
					}
				}
			}
		}
	}
	
	if (!moveFound)
	{
		gameOver=true;
		
		if (checkCheck(colorToMove, Square(), Square()))
			winner=NO_COLOR;
		else
			winner=getOtherColor(colorToMove);
	}
	else
	{
		if (checkForBoringTie())
		{
			gameOver=true;
			winner=NO_COLOR;
		}
		else
		{
			gameOver=false;
			winner=NO_COLOR;
		}
	}
	
	reportErrors=reportErrorsWasOn;
}

bool Game::checkForBoringTie()
{
	auto i=history.end();
	int j=0;
	
	while (i!=history.begin())
	{
		--i;
		
		if ((*i).killed!=nullptr || (*i).p.type==PAWN)
		{
			break;
		}
		
		j++;
	}
	
	if (j>=50)
		return true;
	
	for (int k=4; k<=j/3; ++k)
	{
		bool foundDifference=false;
		
		for (int l=0; l<k; ++l)
		{
			HistoryMove mv0=history[history.size()-l-1], mv1=history[history.size()-k-l-1];
			
			if (mv0.s==mv1.s && mv0.e==mv1.e && mv0.p.type==mv1.p.type && mv0.p.color==mv1.p.color)
			{
				HistoryMove mv2=history[history.size()-k*2-l-1];
				
				if (mv2.s==mv1.s && mv2.e==mv1.e && mv2.p.type==mv1.p.type && mv2.p.color==mv1.p.color)
				{
					
				}
				else
				{
					foundDifference=true;
					break;
				}
			}
			else
			{
				foundDifference=true;
				break;
			}
		}
		
		if (!foundDifference)
			return true;
	}
	
	return false;
}

void Game::forceMove(Square s, Square e)
{
	HistoryMove mv;
	mv.s=s;
	mv.e=e;
	mv.p=*board(s);
	mv.killed=board(e);
	
	if (board(e))
		board(e)->alive=0;
	
	//en passante
	if (board(s)->type==PAWN && s.x!=e.x && !board(e) && e.y==(board(s)->color==WHITE?5:2))
	{
		Square captSq=Square(e.x, (board(s)->color==WHITE?4:3));
		
		if (board(captSq) && board(captSq)->type==PAWN && board(captSq)->color!=board(s)->color)
		{	//pawn promotion

			mv.killed=board(captSq);
			board(captSq)->alive=0;
			board(captSq, 0);
		}
	}
	
	//pawn promotion
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
			Square rookS, rookE;
			
			rookS=p->square;
			rookE=Square(s.x+(s.x<e.x?1:-1), s.y);
			
			board(rookE, board(rookS));
			board(rookS, 0);
			p->square=rookE;
		}
	}
	
	board(e, board(s));
	board(s, 0);
	board(e)->square=e;
	
	colorToMove=getOtherColor(colorToMove);
	history.push_back(mv);
}

void Game::copyInData(Game * game)
{
	gameOver=game->gameOver;
	winner=game->winner;
	colorToMove=game->colorToMove;
	
	pieceToPromoteTo[WHITE]=game->pieceToPromoteTo[WHITE];
	pieceToPromoteTo[BLACK]=game->pieceToPromoteTo[BLACK];
	
	unsigned long offset=pieces-game->pieces;
	
	kings[WHITE]=game->kings[WHITE]+offset;
	kings[BLACK]=game->kings[BLACK]+offset;
	
	for (int i=0; i<32; ++i)
	{
		pieces[i]=game->pieces[i];
	}
	
	for (int x=0; x<8; ++x)
	{
		for (int y=0; y<8; ++y)
		{
			if (game->boardData[x][y])
				boardData[x][y]=game->boardData[x][y]+offset;
			else
				boardData[x][y]=0;
		}
	}
	
	history.clear();
	for (auto i=game->history.begin(); i!=game->history.end(); ++i)
	{
		HistoryMove mv=*i;
		mv.killed=(*i).killed+offset;
		history.push_back(mv);
	}
}

void Game::undo()
{
	if (history.empty())
		return;
	
	HistoryMove mv=history.back();
	
	Piece * p=board(mv.e);
	
	//move the main piece back
	board(mv.s, p);
	p->square=mv.s;
	
	board(mv.e, nullptr);
	
	//restore a captured piece
	if (mv.killed)
	{
		board(mv.killed->square, mv.killed);
		mv.killed->alive=true;
	}
	
	//restore type in case of pawn promotion
	p->type=mv.p.type;
	
	//castling
	if (mv.p.type==KING && abs(mv.s.x-mv.e.x)==2)
	{
		Square rookS, rookE;
		
		rookS=Square(mv.s.x<mv.e.x?7:0, mv.s.y);
		rookE=Square(mv.e.x+(mv.s.x<mv.e.x?-1:1), mv.s.y);
		
		board(rookE)->square=rookS;
		board(rookS, board(rookE));
		board(rookE, 0);
	}
	
	colorToMove=getOtherColor(colorToMove);
	history.pop_back();
	
	if (getIfGameOver())
		setWinState();
}

bool Game::checkBoard()
{
	bool problem=false;
	
	if (!kings[WHITE]->alive)
	{
		err << "white king dead" << err;
		problem=true;
	}
	
	if (!kings[BLACK]->alive)
	{
		err << "black king dead" << err;
		problem=true;
	}
	
	Square i;
	
	for (i.y=0; i.y<8; ++i.y)
	{
		for (i.x=0; i.x<8; ++i.x)
		{
			if (board(i))
			{
				Piece p=*board(i);
				
				if (!p.alive)
				{
					err << pieceType2Name(p.type) << " at " << i.str() << " is dead but it is still on the board" << err;
					problem=true;
				}
				
				if (!(p.square==i))
				{
					err << pieceType2Name(p.type) << " is on board at " << i.str() << " but thinks its on " << p.square.str() << err;
					problem=true;
				}
			}
		}
	}
	
	for (int i=0; i<32; ++i)
	{
		if (pieces[i].alive)
		{
			if (board(pieces[i].square)!=&pieces[i])
			{
				if (board(pieces[i].square)==nullptr)
				{
					err << pieceType2Name(pieces[i].type) << " thinks its on " << pieces[i].square.str() << " but the board there is empty" << err;
					problem=true;
				}
				else if (board(pieces[i].square)->type!=pieces[i].type || board(pieces[i].square)->color!=pieces[i].color)
				{
					err << pieceType2Name(pieces[i].type) << " thinks its on " << pieces[i].square.str() << " but the board has a different piece there" << err;
					problem=true;
				}
				else
				{
					err << pieceType2Name(pieces[i].type) << " thinks its on " << pieces[i].square.str() << " but the board has a pointer to a different instance of the same piece" << err;
					problem=true;
				}
			}
		}
	}
	
	return problem;
}

string Game::historyToStr()
{
	string out;
	
	for (auto i=history.begin(); i!=history.end(); i++)
	{
		HistoryMove mv=*i;
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

