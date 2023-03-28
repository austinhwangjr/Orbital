/******************************************************************************/
/*!
\file		Main.cpp
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the main loop of the game.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
// --------------------------------- MAIN GAME LOOP ------------------------------------------
// includes
#include "pch.h"
#include "GameStateManager.h"
#include "Systems.h"
#include "Input.h"
#include <iostream>
#include "MainLevel.h"
#include "AudioManager.h"

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

	system_call::init(hInstance, nCmdShow);					// Initialize the system

	// debugging log for mainmenu -yy
	/*AEGfxReset();*/											// Reset the graphics engine before initializing the game state manager

	AudioManager::Init();

	// Initialize the game state manager with the starting state
	//gsm::init(GS_SPLASHSCREEN);
	gsm::init(GS_MAINMENU);
	//gsm::init(GS_MAINLEVEL);																					// Initialize the Game State Manager (GSM) with Level1 as the initial game state
	//gsm::init(GS_PAUSEMENU);

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

			fpUpdate();         // Update current game state

			fpDraw();           // Render current game state

			AESysFrameEnd();
		}

		// debugging log for mainmenu -yy
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

	
	system_call::unload();              // Systems exit (terminate)
	
}
