#pragma once

#include "../../WidapLib2/h/Graphics/Surface.h"
#include "../ChessGame.h"
#include <vector>

class PieceDrawer
{
public:
	
	PieceDrawer();
	void drawPiece(widap::V2d lowIn, widap::V2d hghIn, chess::Piece p);
	void setSurface(widap::Surface * surfaceIn) {surface=surfaceIn;}
	
private:
	
	widap::Surface * surface=nullptr;
	
	std::vector<widap::V2d> pieceShapes[6];
};


