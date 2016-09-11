
#include "../ChessGame.h"

namespace widap {class WindowBase;}

namespace chess {class ChessAI;}

class ChessGUI
{
public:
	
	ChessGUI();
	~ChessGUI();
	
	//run the app, function will exit when the window is closed
	void run();
	
private:
	
	void processInput();
	
	void moveAI();
	
private:
	
	/*class Animation
	{
		public:
		Animation() {length=0; reset();}
		Animation(int lengthIn) {length=lengthIn; reset();}
		void reset() {frame=0;}
		void advance() {if (frame<length) frame++;}
		double get() {return widap::lerp(frame, 0, length, 0, 1);}
		
	private:
		int frame, length;
	};*/
	
	/// important objects
	
	widap::WindowBase * window;
	chess::Game game;
	chess::ChessAI * whiteAI=nullptr, * blackAI=nullptr;
};

