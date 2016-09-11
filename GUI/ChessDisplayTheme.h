#pragma once

#include "../../WidapLib2/h/Graphics/Color.h"

namespace chess
{

struct ChessDisplayTheme
{
	ChessDisplayTheme();
	
	widap::Clr boardW, boardB, boardClicked, pieceW, pieceB, bknd, cordText;
};

extern ChessDisplayTheme theme;

}

