#include "PieceDrawer.h"
#include "../../WidapLib2/h/Mscl/Error.h"
#include "ChessDisplayTheme.h"

using namespace widap;
using namespace chess;

PieceDrawer::PieceDrawer()
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

void PieceDrawer::drawPiece(V2d lowIn, V2d hghIn, Piece p)
{
	if (!surface)
	{
		widap::err << "PieceDrawer::drawPiece called with surfece still set to null" << widap::err;
		return;
	}
	
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


