#pragma once
#include <wtypes.h>
#include "ObjManager.h"

class KeyManager
{
public:
	KeyManager() {};
	void Input(ObjManager o, WPARAM wParam);
	void MouseInput(ObjManager o, LPARAM lParam);
};

