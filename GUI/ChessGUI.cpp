
#include "ChessGUI.h"

#include "BoardViewUI.h"

#include "../../WidapLib2/h/Window/WindowSFML.h"
#include "../../WidapLib2/h/UI/NavViewUI.h"
#include "../../WidapLib2/h/UI/StackViewUI.h"
#include "../../WidapLib2/h/UI/ButtonViewUI.h"
#include "../../WidapLib2/h/UI/LabelViewUI.h"

#include "../AI/ChessAI.h"

#include <iostream>

using namespace widap;
using namespace chess;

class NewGamePrompt: public StackViewUI
{
public:
	NewGamePrompt(Game * gameIn, NavViewUI * navIn)
	{
		setLoyout(StackViewUI::CENTER, StackViewUI::CENTER);
		
		yes.setClickAction([=](){gameIn->reset(); navIn->popView();});
		no.setClickAction(navIn->popViewLambda());
		
		buttonContainer.setLoyout(TRAILING, TRAILING, true);
		buttonContainer.addChild(&yes);
		buttonContainer.addChild(&no);
		
		title.setStyle(2);
		
		addChild(&title);
		addChild(&buttonContainer);
		
	}

	LabelViewUI title={"Are you sure you want to start a new game?"};
	StackViewUI buttonContainer;
	ButtonViewUI yes={"Yes"}, no={"No"};
};

ChessGUI::ChessGUI()
{
	window=new WindowSFML();
	
	whiteAI=new ChessAI();
	blackAI=new ChessAI();
	
	whiteAI->setup(&game, WHITE);
	blackAI->setup(&game, BLACK);
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
	window->open(V2u(1200, 1200), "Widap Chess GUI");
	
	NavViewUI nav;
	
	BoardViewUI boardView(&game);
	boardView.setWatchForInput(true);
	
	StackViewUI toolbar;
	toolbar.setLoyout(StackViewUI::LEADING, StackViewUI::LEADING, true, false);
	toolbar.theme.outerBuffer=V2d(8, 0); toolbar.contentsChanged();
	
	NewGamePrompt newGamePrompt(&game, &nav);
	ButtonViewUI newGameBtn("New Game");
	newGameBtn.setClickAction(nav.pushViewLambda(&newGamePrompt));
	toolbar.addChild(&newGameBtn);
	
	ButtonViewUI undoBtn("Undo");
	toolbar.addChild(&undoBtn);
	undoBtn.setClickAction([this](){game.undo();});
	
	ButtonViewUI aiPlayBtn("Play AI");
	toolbar.addChild(&aiPlayBtn);
	aiPlayBtn.setClickAction([this]()
		{
			if (!game.getIfGameOver())
			{
				if (game.getColorToMove()==WHITE)
					whiteAI->nextMove();
				else
					blackAI->nextMove();
			}
		}
	);
	
	StackViewUI boardParent;
	boardParent.setLoyout(StackViewUI::LEADING, StackViewUI::FILL, false, false);
	boardParent.theme.outerBuffer.zero(); boardParent.theme.innerBuffer=0; boardParent.contentsChanged();
	boardParent.addChild(&toolbar);
	boardParent.addChild(&boardView);
	
	nav.setWindow(window);
	nav.pushView(&boardParent);
	nav.setRect(V2d(), window->getDim());
	
	V2u oldDim=window->getDim();
	
	do
	{
		nav.update();
		
		if (window->getDim()!=oldDim)
		{
			nav.setRect(V2d(), window->getDim());
			oldDim=window->getDim();
		}
		
		nav.drawBknd();
		nav.draw();
		
		if (game.getColorToMove()==BLACK)
			blackAI->nextMove();
	}
	while (window->nextFrame());
}




