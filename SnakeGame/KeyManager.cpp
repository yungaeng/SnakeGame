#include "KeyManager.h"

void KeyManager::Input(ObjManager o, WPARAM wParam)
{
	switch (wParam)
	{

	case 'Q': PostQuitMessage(0);
	}
}

void KeyManager::MouseInput(ObjManager o, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	o.MoveSnake(0, x, y);
}
