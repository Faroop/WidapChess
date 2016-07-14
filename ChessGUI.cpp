
#include "../WidapLib2/sfml_widap.h"

using namespace widap;

int main(int argc, char ** argv)
{
	WindowSFML window;
	
	window.open(V2u(0, 0), "Widap Chess GUI");
	
	return 0;
}

