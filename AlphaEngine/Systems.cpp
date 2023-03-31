/******************************************************************************/
/*!
\file		Systems.cpp
\author		Ong You Yang
\par		email: youyang.o\@digipen.edu
\date		March 28, 2023
\brief		This file contains the definition of functions for the system.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "pch.h"
#include "Systems.h"
#include "Global.h"
#include "AudioManager.h"

// Fonts
s8 font_id, font_id_shop, font_id_splashscreen;

// ----------------------------------------------------------------------------
// This function initializes the system.
// ----------------------------------------------------------------------------
void system_call::init(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
	Global_InitWindowSize(false);																	// Initialize game window size (yy added)
	AESysInit(hInstance, static_cast<s32>(nCmdShow), 
		static_cast<s32>(g_windowWidth), static_cast<s32>(g_windowHeight), 
		g_consoleOn, 60, true, NULL);																// Initialize the system
	AESysSetWindowTitle("Orbital");																	// Set the window title
	AESysReset();																					// Reset the system modules

	std::cout << "------------------------- Initializing System -------------------------" << std::endl << std::endl;

	// Fonts

	font_id = AEGfxCreateFont("Assets/Fonts/Tungsten-Semibold.ttf", FONT_ID_SIZE);
	font_id_shop = AEGfxCreateFont("Assets/Fonts/Tungsten-Semibold.ttf", FONT_ID_SHOP_SIZE);
	font_id_splashscreen = AEGfxCreateFont("Assets/Fonts/Tungsten-Semibold.ttf", FONT_ID_SPLASHSCREEN_SIZE);
}

// ----------------------------------------------------------------------------
// This function terminates the system.
// ----------------------------------------------------------------------------
void system_call::unload()
{
	// Fonts
	AudioManager::Shutdown();
	AEGfxDestroyFont(font_id);
	AEGfxDestroyFont(font_id_shop);

	// Free the system
	AESysExit();
}

