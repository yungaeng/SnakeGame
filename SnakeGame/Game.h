#pragma once
#include "Painter.h"
#include "ObjManager.h"
class Game
{
	Painter p = {};
	ObjManager o = {};
public:
	Game() {
		p = {};
		o = {};
	};
	~Game() {};
};

