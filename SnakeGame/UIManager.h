#pragma once
#include <wtypes.h>
#include "ObjManager.h"

class UIManager
{
public:
	UIManager() {};
	void KeyInput(ObjManager o, WPARAM wParam);
	void MouseInput(ObjManager o, LPARAM lParam, double deltaTime);
};

