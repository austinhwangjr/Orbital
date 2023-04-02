/******************************************************************************/
/*!
\file		GameStateManager.h
\author 	Hwang Jing Rui, Austin (50%)
\co-author	Ong You Yang (50%)
\par    	email: jingruiaustin.hwang\@digipen.edu
				   youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the Game State
			Manager.

			Austin 		- Basic Implementation
			You Yang	- Implemented the RestartLevel and changeGameState()
						  functions

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include <functional>

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
