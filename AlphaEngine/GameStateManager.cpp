/******************************************************************************/
/*!
\file		GameStateManager.cpp
\author 	Hwang Jing Rui, Austin (50%)
\co-author	Ong You Yang (50%)
\par    	email: jingruiaustin.hwang\@digipen.edu
				   youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the Game State
			Manager.

			Austin 		- Setting up of GSM (Basic Implementation)
			You Yang	- Implemented the RestartLevel and changeGameState()
						  functions

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "pch.h"
#include "gch.h"
#include "GameStateManager.h"
#include "MainMenu.h"
#include "MainLevel.h"
#include "PauseMenu.h"
#include "HowToPlay.h"
#include "Credits.h"
#include "Highscore.h"
#include "LoseMenu.h"
#include "Options.h"
#include "SplashScreen.h"
#include <functional>
#include <iostream>
#include "Transition.h"

using FP = std::function<void()>;

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
	case GS_SPLASHSCREEN:
		fpLoad		= SplashScreen::load;
		fpInit		= SplashScreen::init;
		fpUpdate	= SplashScreen::update;
		fpDraw		= SplashScreen::draw;
		fpFree		= SplashScreen::free;
		fpUnload	= SplashScreen::unload;
		break;
	case GS_MAINMENU:
		fpLoad		= main_menu::load;
		fpInit		= main_menu::init;
		fpUpdate	= main_menu::update;
		fpDraw		= main_menu::draw;
		fpFree		= main_menu::free;
		fpUnload	= main_menu::unload;
		break;
	case GS_HOWTOPLAY:
		fpLoad		= how_to_play::load;
		fpInit		= how_to_play::init;
		fpUpdate	= how_to_play::update;
		fpDraw		= how_to_play::draw;
		fpFree		= how_to_play::free;
		fpUnload	= how_to_play::unload;
		break;
	case GS_CREDITS:
		fpLoad		= credits::load;
		fpInit		= credits::init;
		fpUpdate	= credits::update;
		fpDraw		= credits::draw;
		fpFree		= credits::free;
		fpUnload	= credits::unload;
		break;
	case GS_MAINLEVEL:
		fpLoad		= main_level::load;
		fpInit		= main_level::init;
		fpUpdate	= main_level::update;
		fpDraw		= main_level::draw;
		fpFree		= main_level::free;
		fpUnload	= main_level::unload;
		break;
	case GS_HIGHSCORE:
		fpLoad		=	HighScore::load;
		fpInit		=	HighScore::init;
		fpUpdate	=	HighScore::update;
		fpDraw		=	HighScore::draw;
		fpFree		=	HighScore::free;
		fpUnload	=	HighScore::unload;
		break;
	case GS_OPTIONS:
		fpLoad = Options::load;
		fpInit = Options::init;
		fpUpdate = std::bind(Options::update, &Options::volume, &Options::muted);
		fpDraw = Options::draw;
		fpFree = Options::free;
		fpUnload = Options::unload;
		break;
	case GS_LOSEMENU:
		fpLoad = lose_menu::load;
		fpInit = lose_menu::init;
		fpUpdate = lose_menu::update;
		fpDraw = lose_menu::draw;
		fpFree = lose_menu::free;
		fpUnload = lose_menu::unload;
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
