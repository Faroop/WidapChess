
#include "ChessGUI.h"
#include "../WidapLib2/h/SimpMath.h"
#include "../WidapLib2/h/ComplexMath.h"
#include <iostream>

using namespace widap;
using namespace chess;

ChessGUI::ChessGUI()
{
	lastMoveAnim=Animation(40);
	
	theme.bknd=clr(82, 32, 14);
	theme.boardW=clr(209, 175, 125);
	theme.boardB=clr(124, 81, 47);
	theme.boardClicked=clr(190, 182, 39);
	theme.pieceW=clr(255, 242, 217);
	theme.pieceB=clr(22, 29, 42);
	theme.cordText=theme.boardB;
}

void ChessGUI::setupPieceShapes()
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
	
	pieceShapes[PAWN].push_back(V2d(0.30, 0.00));
	pieceShapes[PAWN].push_back(V2d(0.70, 0.00));
	pieceShapes[PAWN].push_back(V2d(0.60, 0.20));
	pieceShapes[PAWN].push_back(V2d(0.80, 0.40));
	pieceShapes[PAWN].push_back(V2d(0.60, 0.60));
	pieceShapes[PAWN].push_back(V2d(0.40, 0.60));
	pieceShapes[PAWN].push_back(V2d(0.20, 0.40));
	pieceShapes[PAWN].push_back(V2d(0.40, 0.20));
}

void ChessGUI::run()
{
	window.open(V2u(1080, 1080), "Widap Chess GUI");
	setupPieceShapes();
	
	do
	{
		lastMoveAnim.advance();
		
		//set vars
		V2d low=V2d();
		V2d hgh=window.getDim();
		boardSide=min(hgh.x-low.x, hgh.y-low.y)*0.875;
		boardCorner=(hgh-low)/2.0-V2d(boardSide/2, boardSide/2);
		
		//handle input
		processInput();
		
		//draw
		window.clear(theme.bknd);
		drawBoard();
	}
	while (window.nextFrame());
}

void ChessGUI::processInput()
{
	V2d m=window.mouseLoc();
	
	window.circle(m, 8, clr(255, 0, 0));
	
	if (window.lClick() && m.x>=boardCorner.x && m.x<=boardCorner.x+boardSide && m.y>=boardCorner.y && m.y<=boardCorner.y+boardSide)
	{
		V2i loc=(((m-boardCorner)*8)/boardSide);
		loc=loc.clamp(V2i(0, 0), V2i(7, 7));
		Square square=Square(loc.x, loc.y);
		
		if (game.getPiece(square).color==game.getColorToMove())
		{
			if (squareClicked.valid && squareClicked==square)
				squareClicked.valid=false;
			else
				squareClicked=square;
		}
		else if (squareClicked.valid)
		{
			if (game.playMove(squareClicked, square))
			{
				lastStart=squareClicked;
				lastEnd=square;
				lastMoveAnim.reset();
				squareClicked.valid=false;
			}
			else
			{
				squareClicked.valid=false;
			}
		}
	}
	else if (window.lClick())
	{
		squareClicked.valid=false;
	}
}

void ChessGUI::drawBoard()
{
	V2i i;
	Clr color;
	
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
			
			window.rect(lowCorner, hghCorner, color);
		}
	}
	
	for (i.y=0; i.y<8; i.y++)
	{
		for (i.x=0; i.x<8; i.x++)
		{
			V2d lowCorner=boardCorner+i*(boardSide/8);
			V2d hghCorner=boardCorner+(i+V2d(1, 1))*(boardSide/8);
			
			Piece piece=game.getPiece(Square(i.x, i.y));
			
			if (piece.color!=NO_COLOR)
				drawPiece(lowCorner, hghCorner, piece);
		}
	}
	
	for (int j=0; j<8; j++)
	{
		V2d loc;
		
		loc=V2d(boardCorner.x+((j+0.375)*(boardSide/8)), boardCorner.y-boardSide/128);
		window.text(string()+(char)(j+'A'), loc, boardSide/16, theme.cordText);
		
		loc=V2d(boardCorner.x-boardSide/64-boardSide/32, boardCorner.y+((j+0.75)*(boardSide/8)));
		window.text(string()+(char)(j+'1'), loc, boardSide/16, theme.cordText);
	}
}

void ChessGUI::drawPiece(V2d lowIn, V2d hghIn, Piece p)
{
	Clr c=(p.color==WHITE?theme.pieceW:theme.pieceB);
	int vertNum=pieceShapes[p.type].size();
	V2d* shape=new V2d[vertNum];
	V2d zero(0, 0), one(1, 1);
	V2d low=lerp(V2d(0.1, 0.1), zero, one, lowIn, hghIn), hgh=lerp(V2d(0.9, 0.9), zero, one, lowIn, hghIn);
	
	for (int i=0; i<vertNum; ++i)
		shape[i]=lerp(pieceShapes[p.type][i], zero, one, low, hgh);
	
	window.poly(shape, vertNum, c);
	
	if (shape)
		delete[] shape;
}


