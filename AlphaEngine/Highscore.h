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

	struct Button3
	{
		float x;
		float y;
		float width;
		float height;
	};
};

void putHighScore(int score);