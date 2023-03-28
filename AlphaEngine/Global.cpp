#include "Global.h"										
#include "AEEngine.h"
#include "Systems.h"

bool		g_isMute = false;
extern f64 MMframe_time;

// Variables related to console display
bool g_consoleOn = true;
bool g_consoleOff = false;

// Variables related to game window size
bool		g_isFullScreen	= false;
double		g_windowWidth;
double		g_windowHeight;
float		g_screenLeft	= 0.0f;												// Left (minimum) X coordinate of the game screen
float		g_screenRight	= 0.0f;												// Right (maximum) X coordinate of the game screen
float		g_screenTop		= 0.0f;												// Top (minimum) Y coordinate of the game screen
float		g_screenBottom	= 0.0f;												// Bottom (maximum) Y coordinate of the game screen
RECT		g_WindowRect;

// Initializes the game window size based on whether it's full-screen or not
void Global_InitWindowSize(bool isFullScreen)
{
	RECT desktopRect;
	GetWindowRect(GetDesktopWindow(), &desktopRect);							// Get size of desktop display

	if (isFullScreen)
	{
		g_isFullScreen	= true;
		g_windowWidth	= desktopRect.right - desktopRect.left;
		g_windowHeight	= desktopRect.bottom - desktopRect.top;
	}
	else
	{
		g_isFullScreen	= false;
		g_windowWidth	= 1600;													// Default window width
		g_windowHeight	= 900;													// Default window height
	}
}

// Updates the game window size based on the current window size
void Global_UpdateWindowSize()
{
	GetWindowRect(AESysGetWindowHandle(), &g_WindowRect);						// Get size of the game window display
	g_windowWidth	= g_WindowRect.right - g_WindowRect.left;						// Calculate window width
	g_windowHeight	= g_WindowRect.bottom - g_WindowRect.top;					// Calculate window height

	g_screenLeft	= static_cast<float>(g_windowWidth / -2.0f);
	g_screenRight	= static_cast<float>(g_windowWidth / 2.0f);
	g_screenTop		= static_cast<float>(g_windowHeight / -2.0f);
	g_screenBottom	= static_cast<float>(g_windowHeight / 2.0f);
}

void Global_ToggleScreen()
{
	Global_ToggleWindowed();
}
