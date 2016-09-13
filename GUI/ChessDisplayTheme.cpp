
#include "ChessDisplayTheme.h"

namespace chess
{

using widap::clr;

ChessDisplayTheme::ChessDisplayTheme()
{
	bknd=clr(82, 32, 14);
	boardW=clr(209, 175, 125);
	boardB=clr(124, 81, 47);
	boardClicked=clr(17, 173, 88);
	pieceW=clr(255, 242, 217);
	pieceB=clr(22, 29, 42);
	cordText=theme.boardB;
}

ChessDisplayTheme theme;

}

