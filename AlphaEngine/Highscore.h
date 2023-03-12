#pragma once
#include "AEEngine.h"
#include "Player.h"

namespace HighScore
{
	void load();
	void init();
	void update();
	void draw();
	void free();
	void unload();
};

void putHighScore(int score);