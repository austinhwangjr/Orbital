#pragma once
#include "AEEngine.h"

#define FONT_ID_SIZE 20
#define FONT_ID_SHOP_SIZE 15

extern bool		g_isMute;

extern bool g_isFullScreen;

extern bool g_consoleOn;
extern bool g_consoleOff;

extern double g_windowWidth;
extern double g_windowHeight;

extern float g_screenLeft;
extern float g_screenRight;
extern float g_screenTop;
extern float g_screenBottom;

extern RECT g_WindowRect;

void Global_InitWindowSize(bool isFullScreen);

void Global_UpdateWindowSize();

void Global_ToggleScreen();
