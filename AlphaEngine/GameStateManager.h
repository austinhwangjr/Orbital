/******************************************************************************/
/*!
\file		GameStateManager.h
\author 	Hwang Jing Rui, Austin
\co-author	Ong You Yang
\par    	email: jingruiaustin.hwang\@digipen.edu
				   youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the Game State
			Manager.

			Austin 		- Basic Implementation
			You Yang	- Implemented the RestartLevel and changeGameState()
						  functions

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once
#include <functional>

enum GS_STATES
{
	GS_SPLASHSCREEN = 0,

	// Main Menu
	GS_MAINMENU,
	GS_HOWTOPLAY,
	GS_HIGHSCORE,
	GS_OPTIONS,
	GS_CREDITS,

	// Main Level
	GS_MAINLEVEL, 
	GS_PAUSEMENU,		
	GS_LOSEMENU,
	GS_RESTART,

	// Global
	GS_QUIT

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
	//void draw();
	//void free();
	//void unload();
};

void GSM_RestartLevel();
