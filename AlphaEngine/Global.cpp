/******************************************************************************/
/*!
\file		Global.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file initializes global variables used.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "Global.h"	
#include "Data.h"

bool		g_isMute = false;

// Variables related to console display
bool		g_consoleOn = true;
bool		g_consoleOff = false;

// Variables related to game window size
bool		g_isFullScreen	= false;
f32			g_windowWidth;
f32			g_windowHeight;
float		g_screenLeft;					// Left (minimum) X coordinate of the game screen
float		g_screenRight;					// Right (maximum) X coordinate of the game screen
float		g_screenTop;					// Top (minimum) Y coordinate of the game screen
float		g_screenBottom;					// Bottom (maximum) Y coordinate of the game screen
RECT		g_WindowRect;
s32			g_mouseScreen_x;				// Mouse X coordinate of the game screen
s32			g_mouseScreen_y;				// Mouse Y coordinate of the game screen
AEVec2		g_mouseWorld;					// Mouse coordinates of world
AEVec2		g_camPos;						// Camera coordinates
f32			g_dt;							// Game loop delta time

float		backgroundWidth;
float		backgroundHeight;
int			player_score;

// IMPORT DATA VECTOR
std::map<std::string, f32> 	GlobalDataMap;
std::vector<Data> 			GlobalData;

// Initializes the game window size based on whether it's full-screen or not
void Global_InitWindowSize(bool isFullScreen)
{
	RECT desktopRect;
	GetWindowRect(GetDesktopWindow(), &desktopRect);							// Get size of desktop display

	ImportDataFromFile("Assets/GameObjectData/GlobalVariablesData.txt", GlobalData, GlobalDataMap);
	g_screenLeft		= GlobalDataMap["g_ScreenLeft"];
	g_screenRight		= GlobalDataMap["g_ScreenRight"];
	g_screenTop			= GlobalDataMap["g_ScreenTop"];
	g_screenBottom		= GlobalDataMap["g_ScreenBottom"];

	backgroundWidth		= GlobalDataMap["g_BackgroundWidth"];
	backgroundHeight	= GlobalDataMap["g_BackgroundHeight"];
	player_score		= static_cast<int>(GlobalDataMap["g_Player_score"]);

	if (isFullScreen)
	{
		g_isFullScreen	= true;
		g_windowWidth	= static_cast<f32>(desktopRect.right - desktopRect.left);
		g_windowHeight	= static_cast<f32>(desktopRect.bottom - desktopRect.top);
	}
	else
	{
		g_isFullScreen	= false;
		g_windowWidth	= GlobalDataMap["g_WindowWidth"];				// Default window width
		g_windowHeight	= GlobalDataMap["g_WindowHeight"];				// Default window height
	}
}

void Global_UpdateGlobals()
{
	// Update global frame time for each loop
	g_dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	// Get mouse coordinates (screen)
	AEInputGetCursorPosition(&g_mouseScreen_x, &g_mouseScreen_y);

	// Setting mouse coordinates (world)
	g_mouseWorld.x = g_camPos.x + g_mouseScreen_x - static_cast<f32>(g_windowWidth / 2);
	g_mouseWorld.y = g_camPos.y + static_cast<f32>(g_windowHeight / 2.f) - g_mouseScreen_y;
}

// Updates the game window size based on the current window size
void Global_UpdateWindowSize()
{
	GetWindowRect(AESysGetWindowHandle(), &g_WindowRect);			// Get size of the game window display
	g_windowWidth	= static_cast<f32>(AEGetWindowWidth());			// Calculate window width
	g_windowHeight	= static_cast<f32>(AEGetWindowHeight());		// Calculate window height

	g_screenLeft	= static_cast<float>(g_windowWidth / -2.0f);
	g_screenRight	= static_cast<float>(g_windowWidth / 2.0f);
	g_screenTop		= static_cast<float>(g_windowHeight / -2.0f);
	g_screenBottom	= static_cast<float>(g_windowHeight / 2.0f);

}

void Global_ToggleScreen()
{
	Global_ToggleWindowed();
}

/**
 * @brief Toggles between full-screen and windowed mode.
 */
void Global_ToggleWindowed()
{
	AESysToggleFullScreen(!g_isFullScreen); // Toggle full-screen mode
	Global_InitWindowSize(!g_isFullScreen); // Reinitialize game window size
	Global_UpdateWindowSize();  			// Update game window size
}
