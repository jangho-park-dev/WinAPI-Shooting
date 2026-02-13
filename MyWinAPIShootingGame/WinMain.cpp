#include "GameMain.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	GameMain game(hInstance);
	if (!game.Initialize(nCmdShow))	return -1;
	game.Run();

	return 0;
}