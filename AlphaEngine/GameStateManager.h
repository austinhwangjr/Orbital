#pragma once
#include <functional>

enum GS_STATES
{
	GS_SPLASHSCREEN = 0,
	GS_MAINMENU,		//1
	GS_HOWTOPLAY,
	GS_CREDITS,
	GS_OPTIONS,
	GS_MAINLEVEL, //5 technically this is the startbutton
	GS_PAUSEMENU,		//6
	GS_RESTART,			//7
	GS_QUIT,
	GS_HIGHSCORE
};

extern unsigned int current_state, previous_state, next_state;

using FP = std::function<void()>;

extern FP fpLoad, fpInit, fpUpdate, fpDraw, fpFree, fpUnload;

struct gsm
{
	//void load();
	static void init(int start_state);
	static void update();
	//void restart();
	//void quit();
	void changeGameState(int next_gamestate);
	//void draw();
	//void free();
	//void unload();
};

void GSM_RestartLevel();
