#include "KeyManager.h"

void KeyManager::Input(ObjManager o, WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP: o.MoveSnake(0, UP); break;
	case VK_DOWN: o.MoveSnake(0, DOWN); break;
	case VK_LEFT: o.MoveSnake(0, LEFT); break;
	case VK_RIGHT: o.MoveSnake(0, RIGHT); break;

	case 'W': o.MoveSnake(1, UP); break;
	case 'S': o.MoveSnake(1, DOWN); break;
	case 'A': o.MoveSnake(1, LEFT); break;
	case 'D': o.MoveSnake(1, RIGHT); break;

	case 'Q': PostQuitMessage(0);
	}
}
