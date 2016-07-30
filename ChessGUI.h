#include "ChessGame.h"
#include "../WidapLib2/sfml_widap.h"
#include "../WidapLib2/h/SimpMath.h"
#include <vector>

class ChessGUI
{
public:
	
	ChessGUI();
	~ChessGUI() {}
	
	//run the app, function will exit when the window is closed
	void run();
	
public:
	
	struct DisplayTheme
	{
		widap::Clr boardW, boardB, boardClicked, pieceW, pieceB, bknd, cordText;
	};
	
	DisplayTheme theme;
	
private:
	
	void setupPieceShapes();
	
	void processInput();
	
	void drawBoard();
	void drawPiece(widap::V2d low, widap::V2d hgh, chess::Piece p);
	
private:
	
	class Animation
	{
		public:
		Animation() {length=0; reset();}
		Animation(int lengthIn) {length=lengthIn; reset();}
		void reset() {frame=0;}
		void advance() {if (frame<length) frame++;}
		double get() {return widap::lerp(frame, 0, length, 0, 1);}
		
	private:
		int frame, length;
	};
	
	/// important objects
	
	widap::WindowSFML window;
	chess::Game game;
	
	
	/// info on drawing the board
	
	widap::V2d boardCorner;
	double boardSide;
	
	
	/// piece data
	
	std::vector<widap::V2d> pieceShapes[6];
	
	
	/// info on specific squares and pieces
	
	//where the last move was
	chess::Square lastStart, lastEnd;
	Animation lastMoveAnim;
	
	//the square that is selected
	chess::Square squareClicked;
};

