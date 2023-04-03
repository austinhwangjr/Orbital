/******************************************************************************/
/*!
\file		GameStateList.h
\author 	Tan Rui An Ryan (100%)
\par    	email: ruianryan.t\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the list of game states.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
enum GS_STATES
{
	GS_SPLASHSCREEN = 0,

	// Main Menu
	GS_MAINMENU,
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