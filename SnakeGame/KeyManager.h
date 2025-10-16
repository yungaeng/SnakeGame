#pragma once
#include <wtypes.h>
#include "ObjManager.h"

class KeyManager
{
public:
	KeyManager() {};
	~KeyManager() {};
	void Input(ObjManager o, WPARAM wParam);
};

