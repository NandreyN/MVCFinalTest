//#include <windows.h>
#include "View.h"
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	unique_ptr<View> viewPtr(new View(hinstance, prevHinstance, lpCmdLine, nCmdShow));
	viewPtr->run();
}