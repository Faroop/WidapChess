#pragma once

#include "../../WidapLib2/h/UI/ViewUI.h"
#include "../ChessGame.h"
#include "PieceDrawer.h"

using namespace widap;
using namespace chess;

class BoardViewUI: public ViewUI
{
public:
	
	BoardViewUI(Game * gameIn);
	
	void drawBknd();
	void draw();
	
	void update();
	
	V2d calcDim() {return {-60, -60};}
	
	void rectChanged();
	
	void setWatchForInput(bool in) {watchForInput=in;}
	
protected:
	void ioChanged() {pieceDrawer.setSurface(surface);}
	
private:
	
	widap::V2d boardCorner;
	double boardSide;
	
	bool watchForInput=false;
	//the square that is selected
	chess::Square squareClicked;
	
	Game * game;
	
	PieceDrawer pieceDrawer;
};
