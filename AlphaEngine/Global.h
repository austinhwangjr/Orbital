/******************************************************************************/
/*!
\file		Global.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains global variables used.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "pch.h"

#define FONT_ID_SIZE 20
#define FONT_ID_SHOP_SIZE 20
#define FONT_ID_SPLASHSCREEN_SIZE 20

extern bool	g_isMute;

extern bool g_isFullScreen;

extern bool g_consoleOn;
extern bool g_consoleOff;

extern f32 g_windowWidth;
extern f32 g_windowHeight;

extern float g_screenLeft;
extern float g_screenRight;
extern float g_screenTop;
extern float g_screenBottom;

extern s32		g_mouseScreen_x, g_mouseScreen_y;
extern AEVec2	g_mouseWorld;
extern AEVec2	g_camPos;
extern f32		g_dt;

extern float backgroundWidth;
extern float backgroundHeight;
extern int	 player_score;

extern RECT g_WindowRect;

void Global_InitWindowSize(bool isFullScreen);

void Global_UpdateGlobals();

void Global_UpdateWindowSize();

void Global_ToggleScreen();

void Global_ToggleWindowed();