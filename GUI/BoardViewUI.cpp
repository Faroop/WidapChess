
#include "BoardViewUI.h"
#include "../../WidapLib2/h/Math/SimpMath.h"
#include "ChessDisplayTheme.h"

BoardViewUI::BoardViewUI(Game * gameIn)
{
	game=gameIn;
	setupPieceShapes();
}

void BoardViewUI::setupPieceShapes()
{
	pieceShapes[KING].push_back(V2d(0.40, 0.00));
	pieceShapes[KING].push_back(V2d(0.60, 0.00));
	pieceShapes[KING].push_back(V2d(1.00, 0.40));
	pieceShapes[KING].push_back(V2d(0.55, 0.50));
	pieceShapes[KING].push_back(V2d(0.55, 0.70));
	pieceShapes[KING].push_back(V2d(0.70, 0.70));
	pieceShapes[KING].push_back(V2d(0.70, 0.80));
	pieceShapes[KING].push_back(V2d(0.55, 0.80));
	pieceShapes[KING].push_back(V2d(0.55, 0.95));
	pieceShapes[KING].push_back(V2d(0.45, 0.95));
	pieceShapes[KING].push_back(V2d(0.45, 0.80));
	pieceShapes[KING].push_back(V2d(0.30, 0.80));
	pieceShapes[KING].push_back(V2d(0.30, 0.70));
	pieceShapes[KING].push_back(V2d(0.45, 0.70));
	pieceShapes[KING].push_back(V2d(0.45, 0.50));
	pieceShapes[KING].push_back(V2d(0.00, 0.40));
	
	pieceShapes[QUEEN].push_back(V2d(0.20, 0.00));
	pieceShapes[QUEEN].push_back(V2d(0.80, 0.00));
	pieceShapes[QUEEN].push_back(V2d(1.00, 1.00));
	pieceShapes[QUEEN].push_back(V2d(0.70, 0.50));
	pieceShapes[QUEEN].push_back(V2d(0.50, 1.00));
	pieceShapes[QUEEN].push_back(V2d(0.30, 0.50));
	pieceShapes[QUEEN].push_back(V2d(0.00, 1.00));
	
	pieceShapes[ROOK].push_back(V2d(0.20, 0.00));
	pieceShapes[ROOK].push_back(V2d(0.80, 0.00));
	pieceShapes[ROOK].push_back(V2d(0.80, 0.60));
	pieceShapes[ROOK].push_back(V2d(0.90, 0.60));
	pieceShapes[ROOK].push_back(V2d(0.90, 1.00));
	pieceShapes[ROOK].push_back(V2d(0.70, 1.00));
	pieceShapes[ROOK].push_back(V2d(0.70, 0.90));
	pieceShapes[ROOK].push_back(V2d(0.50, 0.90));
	pieceShapes[ROOK].push_back(V2d(0.50, 1.00));
	pieceShapes[ROOK].push_back(V2d(0.30, 1.00));
	pieceShapes[ROOK].push_back(V2d(0.30, 0.90));
	pieceShapes[ROOK].push_back(V2d(0.10, 0.90));
	pieceShapes[ROOK].push_back(V2d(0.10, 0.60));
	pieceShapes[ROOK].push_back(V2d(0.20, 0.60));
	
	pieceShapes[KNIGHT].push_back(V2d(0.30, 0.00));
	pieceShapes[KNIGHT].push_back(V2d(0.70, 0.00));
	pieceShapes[KNIGHT].push_back(V2d(0.80, 1.00));
	pieceShapes[KNIGHT].push_back(V2d(0.20, 0.80));
	pieceShapes[KNIGHT].push_back(V2d(0.20, 0.60));
	pieceShapes[KNIGHT].push_back(V2d(0.50, 0.60));
	
	pieceShapes[BISHOP].push_back(V2d(0.30, 0.00));
	pieceShapes[BISHOP].push_back(V2d(0.70, 0.00));
	pieceShapes[BISHOP].push_back(V2d(0.60, 0.50));
	pieceShapes[BISHOP].push_back(V2d(0.70, 0.70));
	pieceShapes[BISHOP].push_back(V2d(0.50, 1.00));
	pieceShapes[BISHOP].push_back(V2d(0.30, 0.75));
	pieceShapes[BISHOP].push_back(V2d(0.50, 0.70));
	pieceShapes[BISHOP].push_back(V2d(0.34, 0.70));
	pieceShapes[BISHOP].push_back(V2d(0.40, 0.50));
	
	pieceShapes[PAWN].push_back(V2d(0.30, 0.20));
	pieceShapes[PAWN].push_back(V2d(0.70, 0.20));
	pieceShapes[PAWN].push_back(V2d(0.60, 0.40));
	pieceShapes[PAWN].push_back(V2d(0.80, 0.60));
	pieceShapes[PAWN].push_back(V2d(0.60, 0.80));
	pieceShapes[PAWN].push_back(V2d(0.40, 0.80));
	pieceShapes[PAWN].push_back(V2d(0.20, 0.60));
	pieceShapes[PAWN].push_back(V2d(0.40, 0.40));
}

void BoardViewUI::update()
{
	V2d m=input->mouseLoc();
	
	if (watchForInput)
	{
		if (input->lClick())
		{
			if (m.isInRect(boardCorner, boardCorner+V2d(boardSide, boardSide)))
			{
				V2i loc=(((m-boardCorner)*8)/boardSide);
				loc=loc.clamp(V2i(0, 0), V2i(7, 7));
				Square square=Square(loc.x, loc.y);
				
				if (game->getPiece(square).color==game->getColorToMove())
				{
					if (squareClicked.valid && squareClicked==square)
						squareClicked.valid=false;
					else
						squareClicked=square;
				}
				else if (squareClicked.valid)
				{
					game->playMove(squareClicked, square);
					
					squareClicked.valid=false;
				}
			}
			else
			{
				squareClicked.valid=false;
			}
		}
	}
	else
	{
		squareClicked.valid=false;
	}
}

void BoardViewUI::drawBknd()
{
	V2i i;
	Clr color;
	
	surface->rect(getLow(), getHgh(), theme.bknd);
	
	for (i.y=0; i.y<8; i.y++)
	{
		for (i.x=0; i.x<8; i.x++)
		{
			V2d lowCorner=boardCorner+i*(boardSide/8);
			V2d hghCorner=boardCorner+(i+V2d(1, 1))*(boardSide/8);
			
			if (squareClicked.valid && squareClicked.x==i.x && squareClicked.y==i.y)
			{
				color=theme.boardClicked;
			}
			else if ((i.x+i.y)%2)
				color=theme.boardW;
			else
				color=theme.boardB;
			
			surface->rect(lowCorner, hghCorner, color);
		}
	}
}

void BoardViewUI::draw()
{
	V2i i;
	Clr color;
	
	for (i.y=0; i.y<8; i.y++)
	{
		for (i.x=0; i.x<8; i.x++)
		{
			V2d lowCorner=boardCorner+i*(boardSide/8);
			V2d hghCorner=boardCorner+(i+V2d(1, 1))*(boardSide/8);
			
			Piece piece=game->getPiece(Square(i.x, i.y));
			
			if (piece.color!=NO_COLOR)
				drawPiece(lowCorner, hghCorner, piece);
		}
	}
	
	for (int j=0; j<8; j++)
	{
		V2d loc;
		
		loc=V2d(boardCorner.x+((j+0.375)*(boardSide/8)), boardCorner.y-boardSide/128);
		surface->text()->draw(string()+(char)(j+'A'), loc, boardSide/16, theme.cordText);
		
		loc=V2d(boardCorner.x-boardSide/64-boardSide/32, boardCorner.y+((j+0.75)*(boardSide/8)));
		surface->text()->draw(string()+(char)(j+'1'), loc, boardSide/16, theme.cordText);
	}
	
	if (game->getIfGameOver())
	{
		string msg;
		
		if (game->getWinner()==NO_COLOR)
			msg="the game\nis a\ndraw";
		else
			msg+=pieceColor2Name(game->getWinner())+"\nhas won!";
		
		surface->rect(boardCorner, boardCorner+V2d(boardSide, boardSide), clr(0, 0, 0), 0.5);
		surface->text()->draw(msg, V2d(boardCorner.x+boardSide*0.1, boardCorner.y+boardSide-boardSide*0.1), boardSide*0.2, theme.pieceW);
	}
}

void BoardViewUI::drawPiece(V2d lowIn, V2d hghIn, Piece p)
{
	Clr c=(p.color==WHITE?theme.pieceW:theme.pieceB);
	int vertNum=pieceShapes[p.type].size();
	V2d* shape=new V2d[vertNum];
	V2d zero(0, 0), one(1, 1);
	V2d low=lerp(V2d(0.1, 0.1), zero, one, lowIn, hghIn), hgh=lerp(V2d(0.9, 0.9), zero, one, lowIn, hghIn);
	
	for (int i=0; i<vertNum; ++i)
		shape[i]=lerp(pieceShapes[p.type][i], zero, one, low, hgh);
	
	surface->poly(shape, vertNum, c);
	
	if (shape)
		delete[] shape;
}

void BoardViewUI::rectChanged()
{
	V2d hgh=getHgh(), low=getLow();
	
	boardSide=min(hgh.x-low.x, hgh.y-low.y)*0.875;
	boardCorner=(hgh+low)/2.0-V2d(boardSide/2, boardSide/2);
}


