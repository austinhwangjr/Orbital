/******************************************************************************/
/*!
\file		Main.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the main loop of the game.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
// --------------------------------- MAIN GAME LOOP ------------------------------------------
// includes
#include "pch.h"
#include "GameStateList.h"
#include "Global.h"
#include "Systems.h"
#include "Input.h"
#include "GameStateManager.h"
#include "AudioManager.h"
#include "Transition.h"
#include "MainLevel.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	system_call::init(hInstance, nCmdShow);		// Initialize the system

	AudioManager::Init();

	// Initialize the game state manager with the starting state
	//gsm::init(GS_SPLASHSCREEN);					// Initialize the Game State Manager (GSM) with Level1 as the initial game state
	gsm::init(GS_MAINLEVEL);
	transition::load();
	transition::init();

	// While the current game state is not equal to the quit state
	while (current_state != GS_QUIT)
	{
		AEGfxReset();											// Reset the graphics engine before initializing the game state manager

		// If the current game state is not equal to the restart state
		if (current_state != GS_RESTART)
		{
			gsm::update();       // Call Update() to update game state manager
			fpLoad();           // Call Load() to load current game state
		}
		else
		{
			next_state = previous_state;    // Set next game state to be equal to previous game state
			current_state = previous_state; // Set current game state to be equal to previous game state
		}

		fpInit();         // Initialize current game state

		//The game loop (while the next game state is equal to the current state)
		while (next_state == current_state)
		{
			// Informing the system about the loop's start
			AESysFrameStart();

			// Handling Input
			AEInputUpdate();

			// Update global variables
			Global_UpdateGlobals();

			fpUpdate();         // Update current game state

			// Update transition
			if (transition::isTransitionActive)
			{
				transition::update(g_dt);
			}

			fpDraw(); // Render current game state

			// Draw transition
			if (transition::isTransitionActive)
			{
				transition::draw();
			}

			AESysFrameEnd();
		}

		AEGfxReset();				// Reset graphics context

		fpFree();               // Free current game state

		// If next game state is not equal to restart game state
		if (next_state != GS_RESTART)
		{
			fpUnload();         // Unload() current game state
		}

		previous_state = current_state;     // Set previous game state to be equal to current game state
		current_state = next_state;         // Set current game state to be equal to next game state
	}
	transition::free();
	transition::unload();

	system_call::unload();              // Systems exit (terminate)
	
}
