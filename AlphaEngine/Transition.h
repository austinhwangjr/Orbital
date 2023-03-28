#pragma once

namespace transition
{
	void load();
	void init();
	void update(float deltaTime);
	void draw();
	void free();
	void unload();
	void resetTimer();
	void activateTransition();

	extern bool isTransitionActive;
	extern float transition_alpha;

}
