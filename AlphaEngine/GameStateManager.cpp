/* Start Header ************************************************************************/
/*!
\file		GameStateManager.cpp
\author		Hwang Jing Rui, Austin, jingruiaustin.hwang, 2200601
\par		jingruiaustin.hwang\@digipen.edu
\date		Jan 14, 2023
\brief		This file contains the definition for the Initialize() and Update() 
			functions for the Game State Manager.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "pch.h"
#include "GameStateManager.h"
#include "MainLevel.h"

int current = 0, previous = 0, next = 0;

FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

// ----------------------------------------------------------------------------
// Initialize the Game State Manager
// Set current, previous and next game state to be equal to starting state
// ----------------------------------------------------------------------------
void gsm_initialize(int startingState)
{
	current = previous = next = startingState;
}

// ----------------------------------------------------------------------------
// Update the Game State Manager
// Switch states depending on current state
// ----------------------------------------------------------------------------
void gsm_update()
{
	// Depending on current state, assign function pointers to corresponding state
	switch (current)
	{
	case GS_MAINLEVEL:		
		fpLoad = main_level_load;
		fpInitialize = main_level_initialize;
		fpUpdate = main_level_update;
		fpDraw = main_level_draw;
		fpFree = main_level_free;
		fpUnload = main_level_unload;
		break;
	case GS_MAINMENU:
		break;
	case GS_RESTART:
		break;
	case GS_QUIT:
		break;
	default:
		break;
	}
}