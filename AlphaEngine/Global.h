/******************************************************************************/
/*!
\file		Global.h
\author 	 
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains global variables used.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once
#include "AEEngine.h"

#define FONT_ID_SIZE 20
#define FONT_ID_SHOP_SIZE 20
#define FONT_ID_SPLASHSCREEN_SIZE 20

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
