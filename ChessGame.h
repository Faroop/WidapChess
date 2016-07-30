
#pragma once

#include <string>
#include <list>
#include "Error.h"

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

std::string pieceType2Name(PieceType type);
std::string pieceColor2Name(PieceColor color);

class Piece
{
public:
	
	Piece() {alive=0; color=NO_COLOR; type=QUEEN, square=Square(0, 0);}
	Piece(PieceColor colorIn, PieceType typeIn, Square squareIn) {alive=1; color=colorIn; type=typeIn, square=squareIn;}
	
	bool alive;
	PieceColor color;
	PieceType type;
	Square square;
};

struct Move
{
	Square s, e; 
	Piece p;
};

class Game
{
public:
	
	Game();
	
	PieceColor getWinner();
	
	//send it where you want to move to and from, it will return true if the move is valid
	bool playMove(Square src, Square dst);
	
	Piece getPiece(Square square);
	PieceColor getColorToMove() {return colorToMove;}
	
	std::string historyToStr();
	
	//the white and black piece that is promoted to when a pawn reaches the end
	//a client may change them at any time; default is Queen
	PieceType pieceToPromoteTo[2];
	
private:
	
	//set up a normal chess board with all the pieces
	void setupBoard();
	
	//return true only if the piece can move to the location and isn't blocked, doesn't check for check or special move
	bool checkMovePath(Square s, Square e);
	
	//returns true only if the given move will not put or leave the mover in check
	bool checkCheck(PieceColor kingColor, Square s, Square e);
	Piece * boardNext(Square i, Square s, Square e); //only used in checkCheck
	bool checkLine(PieceColor friendlyColor, Square i, int vx, int vy, Square s, Square e); //only used in checkCheck
	
	//move the source piece to the destination; doesn't do any checks or aditional actions (castling takes two of these)
	void forceMove(Square src, Square dst);
	
	Piece * board(Square loc) {return boardData[loc.x][loc.y];}
	void board(Square loc, Piece * ptr) {boardData[loc.x][loc.y]=ptr;}
	
private:
	
	static const bool showDebug=1;
	
	Piece * boardData[8][8];
	Piece pieces[32];
	Piece * kings[2];
	
	//the color to make the next move
	PieceColor colorToMove;
	
	std::list<Move> history;
	
	Error err;
};

}


