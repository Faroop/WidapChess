#include "ChessGame.h"
#include <iostream>

using std::string;
using std::cout;
using std::cin;
using std::endl;

using namespace chess;

void displayBoardCLI(Game * game);

void mainChessCLI()
{
	Game game;
	string moveStrSrc, moveGameDst;
	
	while (game.getWinner()==NO_COLOR)
	{
		displayBoardCLI(&game);
		
		do
		{
			cin >> moveStrSrc;
			cin >> moveGameDst;
		}
		while (!game.playMove(moveStrSrc, moveGameDst));
	}
}

void displayBoardCLI(Game * game)
{
	for (int i=0; i<8; i++)
		cout << endl;
	
	for (int y=7; y>=0; --y)
	{
		cout << y+1 << " ";
		for (int x=0; x<8; ++x)
		{
			string c;
			Piece piece=game->getPiece(Square(x, y));
			
			if (piece.color==WHITE)
			{
				//♔♕♖♗♘♙♚♛♜♝♞♟
				     if (piece.type==KING)   c="♚";
				else if (piece.type==QUEEN)  c="♛";
				else if (piece.type==ROOK)   c="♜";
				else if (piece.type==KNIGHT) c="♞";
				else if (piece.type==BISHOP) c="♝";
				else if (piece.type==PAWN)   c="♟";
			}
			else if (piece.color==BLACK)
			{
				     if (piece.type==KING)   c="♔";
				else if (piece.type==QUEEN)  c="♕";
				else if (piece.type==ROOK)   c="♖";
				else if (piece.type==KNIGHT) c="♘";
				else if (piece.type==BISHOP) c="♙";
				else if (piece.type==PAWN)   c="♙";
			}
			else
			{
				if ((y+x+1)%2)
					c="░";
				else
					c=" ";
			}
			cout << c;
		}
		
		cout << endl;
	}
	
	cout << "  ABCDEFGH" << endl;
}


