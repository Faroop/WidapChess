#include "ChessGame.h"

#include <iostream>

using std::string;
using std::cout;
using std::cin;
using std::getline;
using std::endl;

using namespace chess;

void displayBoardCLI(Game * game);

void mainChessCLI()
{
	Game game;
	string cmd;
	bool quit=false;
	
	while (!quit)
	{
		displayBoardCLI(&game);
		bool valid=false;
		
		do
		{
			getline(cin, cmd);
			
			if (cmd.length()==5 && cmd[2]==' ')
			{
				Square s(cmd.substr(0, 2)), e(cmd.substr(3, 5));
				if (s.valid && e.valid && game.playMove(s, e))
					valid=true;
			}
			else if (cmd=="quit")
			{
				quit=true;
				valid=true;
			}
			else if (cmd=="help")
			{
				cout << "type in you move on one line like 'E2 E4' or type 'quit' to quit" << endl;
			}
			else
			{
				cout << "unrecognised command, type 'help' for help" << endl;
			}
		}
		while (!valid);
		
		if (game.getWinner()!=NO_COLOR)
			quit=true;
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
				else if (piece.type==BISHOP) c="♗";
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


