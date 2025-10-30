#include "UIManager.h"

void UIManager::KeyInput(ObjManager o, WPARAM wParam)
{
	switch (wParam)
	{
	case 'Q': PostQuitMessage(0);
	}
	return;
}

void UIManager::MouseInput(ObjManager o, LPARAM lParam, double deltaTime)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	o.m_snakes[0].m_target_x = x;
	o.m_snakes[0].m_target_y = y;
	return;
}
