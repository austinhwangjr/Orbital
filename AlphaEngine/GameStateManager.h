enum GS_STATES
{
	GS_SPLASHSCREEN,
	GS_MAINMENU,
	GS_MAINLEVEL = 0,


	GS_PAUSE,
	GS_QUIT,
	GS_RESTART
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
