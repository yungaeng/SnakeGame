#include "UIManager.h"

void UIManager::Input(ObjManager o, WPARAM wParam)
{
	switch (wParam)
	{
	case 'Q': PostQuitMessage(0);
	}
}

void UIManager::MouseInput(ObjManager o, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	o.m_snakes[0].m_target_x = x;
	o.m_snakes[0].m_target_y = y;
}
