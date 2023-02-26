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
#include "MainMenu.h"
#include "MainLevel.h"
#include "PauseMenu.h"

unsigned int current_state, previous_state, next_state;

FP fpLoad = nullptr, fpInit = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

// ----------------------------------------------------------------------------
// Initialize the Game State Manager
// Set current, previous and next game state to be equal to starting state
// ----------------------------------------------------------------------------
void gsm::init(int start_state)
{
	current_state = previous_state = next_state = start_state;
}

// ----------------------------------------------------------------------------
// Update the Game State Manager
// Switch states depending on current state
// ----------------------------------------------------------------------------
void gsm::update()
{
	// Depending on current state, assign function pointers to corresponding state
	switch (current_state)
	{
	/*case GS_SPLASHSCREEN:
		fpLoad = main_menu::load;
		fpInit		= main_menu::init;
		fpUpdate	= main_menu::update;
		fpDraw		= main_menu::draw;
		fpFree		= main_menu::free;
		fpUnload	= main_menu::unload;
		break;*/
	case GS_MAINMENU:
		fpLoad		= main_menu::load;
		fpInit		= main_menu::init;
		fpUpdate	= main_menu::update;
		fpDraw		= main_menu::draw;
		fpFree		= main_menu::free;
		fpUnload	= main_menu::unload;
		break;
	//case GS_HOWTOPLAY:
	//	fpLoad		= how_to_play::load;
	//	fpInit		= how_to_play::init;
	//	fpUpdate	= how_to_play::update;
	//	fpDraw		= how_to_play::draw;
	//	fpFree		= how_to_play::free;
	//	fpUnload	= how_to_play::unload;
	//	break;
	case GS_MAINLEVEL:
		fpLoad		= main_level::load;
		fpInit		= main_level::init;
		fpUpdate	= main_level::update;
		fpDraw		= main_level::draw;
		fpFree		= main_level::free;
		fpUnload	= main_level::unload;
		break;
	case GS_PAUSEMENU:
		fpLoad = pause_menu::load;
		fpInit = pause_menu::init;
		fpUpdate = pause_menu::update;
		fpDraw = pause_menu::draw;
		fpFree = pause_menu::free;
		fpUnload = pause_menu::unload;
		break;
	case GS_RESTART:
		break;
	case GS_QUIT:

		
		break;
	default:
		break;
	}
}

void GSM_RestartLevel()
{

	next_state = GS_RESTART;
}