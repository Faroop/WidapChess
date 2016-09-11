
#include "ChessGUI.h"

#include "BoardViewUI.h"

#include "../../WidapLib2/h/Window/WindowSFML.h"
#include "../../WidapLib2/h/UI/NavViewUI.h"
#include "../../WidapLib2/h/UI/StackViewUI.h"
#include "../../WidapLib2/h/UI/ButtonViewUI.h"

#include "../AI/ChessAI.h"

#include <iostream>

using namespace widap;
using namespace chess;

ChessGUI::ChessGUI()
{
	window=new WindowSFML();
	
	//chessAIs[0].setup(&game, WHITE);
	//chessAIs[1].setup(&game, BLACK);
}

ChessGUI::~ChessGUI()
{
	delete window;
	
	if (whiteAI)
		delete whiteAI;
	
	if (blackAI)
		delete blackAI;
}

void ChessGUI::run()
{
	window->openMaximized("Widap Chess GUI");
	
	NavViewUI nav;
	
	BoardViewUI boardView(&game);
	boardView.setWatchForInput(true);
	
	StackViewUI toolbar;
	toolbar.setLoyout(StackViewUI::LEADING, StackViewUI::LEADING, true, false);
	ButtonViewUI undoBtn("Undo");
	undoBtn.setClickAction([this](){game.undo();});
	toolbar.addChild(&undoBtn);
	ButtonViewUI newGameBtn("New Game");
	//newGameBtn.setClickAction([this](){game=Game();});
	toolbar.addChild(&newGameBtn);
	
	StackViewUI boardParent;
	boardParent.setLoyout(StackViewUI::LEADING, StackViewUI::LEADING, false, false);
	boardParent.addChild(&toolbar);
	boardParent.addChild(&boardView);
	
	nav.setWindow(window);
	nav.pushView(&boardParent);
	nav.setRect(V2d(), window->getDim());
	
	V2u oldDim=window->getDim();
	
	do
	{
		if (window->getDim()!=oldDim)
			nav.setRect(V2d(), window->getDim());
		
		nav.update();
		nav.draw();
	}
	while (window->nextFrame());
}




