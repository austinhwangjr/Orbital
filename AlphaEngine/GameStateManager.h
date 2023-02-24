#pragma once
enum GS_STATES
{
	GS_SPLASHSCREEN = 0,
	GS_MAINMENU,
	GS_HOWTOPLAY,
	GS_CREDITS,
	GS_OPTIONS,
	GS_MAINLEVEL, //5 technically this is the startbutton
	GS_PAUSE,
	GS_RESTART,
	GS_QUIT
};

extern unsigned int current_state, previous_state, next_state;

typedef void(*FP)(void);

extern FP fpLoad, fpInit, fpUpdate, fpDraw, fpFree, fpUnload;

namespace gsm
{
	//void load();
	void init(int start_state);
	void update();
	//void restart();
	//void quit();
	//void changeGameState(int next_gamestate)
	//void draw();
	//void free();
	//void unload();
}

void GSM_RestartLevel();