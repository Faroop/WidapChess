
#pragma once

#include <string>
#include <list>
#include "Error.h"

/// problems with this class:

//does not have repeat three position tie handling
//does not have 50 move tie handling

namespace chess
{

class Square
{
	
public:
	int x, y;
	bool valid;
	
	Square() {x=0; y=0; valid=false;}
	Square(int xIn, int yIn);
	
	//set a location with a 2 character string like "E4"
	Square(std::string str);
	
	//get the two character string
	std::string str();
	
	//clamps the square to the board size
	void clamp();
	
	//returns if the square location is in the board
	bool isInBoard() {return x>=0 && x<8 && y>=0 && y<8;}
	
	bool operator== (Square in) {return x==in.x && y==in.y;}
};

enum PieceType {KING=0, QUEEN=1, ROOK=2, KNIGHT=3, BISHOP=4, PAWN=5};
enum PieceColor {NO_COLOR=-1, WHITE=0, BLACK=1};

PieceColor getOtherColor(PieceColor in);
std::string pieceType2Name(PieceType type);
std::string pieceColor2Name(PieceColor color);

class Piece
{
public:
	
	Piece() {alive=0; color=NO_COLOR; type=QUEEN, square=Square(0, 0);}
	Piece(PieceColor colorIn, PieceType typeIn, Square squareIn) {alive=1; color=colorIn; type=typeIn, square=squareIn;}
	
	std::string str();
	
	bool alive;
	PieceColor color;
	PieceType type;
	Square square;
};

struct HistoryMove
{
	Square s, e;
	Piece p;
	Piece * killed;
	
};

class Game
{
public:
	
	Game();
	~Game() {}
	
	void reset();
	
	bool getIfGameOver();
	PieceColor getWinner();
	
	//send it where you want to move to and from, it will return true if the move is valid
	bool playMove(Square src, Square dst);
	
	//undo one move
	void undo();
	
	//returns true if there is an error with the board
	bool checkBoard();
	
	Piece getPiece(Square square);
	PieceColor getColorToMove() {return colorToMove;}
	
	std::string historyToStr();
	
	//the white and black piece that is promoted to when a pawn reaches the end
	//a client may change them at any time; default is Queen
	PieceType pieceToPromoteTo[2];
	
protected:
	
	//set up a normal chess board with all the pieces
	void setupBoard();
	
	//return true only if the piece can move to the location and isn't blocked, doesn't check for check or special move
	bool checkMovePath(Square s, Square e);
	
	//returns true only if the given move will not put or leave the mover in check
	bool checkCheck(PieceColor kingColor, Square s, Square e);
	Piece * pieceWillBeInSquare(Square i, PieceColor ignoreColor, PieceType type, Square s, Square e); //only used in checkCheck
	bool checkLine(PieceColor friendlyColor, Square i, int vx, int vy, Square s, Square e); //only used in checkCheck
	
	//check for a win state and set the variables
	void setWinState();
	
	//move the source piece to the destination; doesn't do any checks or aditional actions (castling takes two of these)
	void forceMove(Square src, Square dst);
	
	void copyInData(Game * game);
	
	Piece * board(Square loc) {return boardData[loc.x][loc.y];}
	inline void board(Square loc, Piece * ptr) {boardData[loc.x][loc.y]=ptr;}
	
protected:
	
	Piece * boardData[8][8];
	Piece pieces[32];
	Piece * kings[2];
	bool gameOver;
	PieceColor winner;
	
	//the color to make the next move
	PieceColor colorToMove;
	
	std::list<HistoryMove> history;
	
	//if the report errors when checking moves
	bool reportErrors=true;
	Error err;
};

}


