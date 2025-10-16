#include "KeyManager.h"

void KeyManager::Input(ObjManager o, WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP: o.MoveSnake(UP); break;
	case VK_DOWN: o.MoveSnake(DOWN); break;
	case VK_LEFT: o.MoveSnake(LEFT); break;
	case VK_RIGHT: o.MoveSnake(RIGHT); break;

	case 'W': o.MoveOtherSnake(UP); break;
	case 'S': o.MoveOtherSnake(DOWN); break;
	case 'A': o.MoveOtherSnake(LEFT); break;
	case 'D': o.MoveOtherSnake(RIGHT); break;

	case 'Q': PostQuitMessage(0);
	}
}
