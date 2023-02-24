/* Start Header ************************************************************************/
/*!
\file		Systems.cpp
\author		Hwang Jing Rui, Austin, jingruiaustin.hwang, 2200601
\par		jingruiaustin.hwang\@digipen.edu
\date		Jan 14, 2023
\brief		This file contains the definition of the Initialize() and Exit() functions
			for the System.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "pch.h"
#include "Systems.h"
#include "Global.h"
#include <iostream>

// Fonts
s8 font_id, font_id_shop;

// ----------------------------------------------------------------------------
// This function initializes the system.
// ----------------------------------------------------------------------------
void system_call::init(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
	Global_InitWindowSize(false);																	// Initialize game window size (yy added)
	AESysInit(hInstance, nCmdShow, g_windowWidth, g_windowHeight, g_consoleOn, 60, true, NULL);		// Initialize the system
	AESysSetWindowTitle("Orbital");																	// Set the window title
	AESysReset();																					// Reset the system modules

	std::cout << "------------------------- Initializing System -------------------------" << std::endl << std::endl;

	// Fonts
	font_id = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 50);
	font_id_shop = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
}

// ----------------------------------------------------------------------------
// This function terminates the system.
// ----------------------------------------------------------------------------
void system_call::unload()
{
	// Fonts
	AEGfxDestroyFont(font_id);
	AEGfxDestroyFont(font_id_shop);

	// Free the system
	AESysExit();
}

/**
 * @brief Toggles between full-screen and windowed mode.
 */
void Global_ToggleWindowed()
{
	AESysToggleFullScreen(!g_isFullScreen);  // Toggle full-screen mode
	if (g_isFullScreen) {}
	Global_InitWindowSize(!g_isFullScreen);  // Reinitialize game window size
	Global_UpdateWindowSize();  // Update game window size
}
