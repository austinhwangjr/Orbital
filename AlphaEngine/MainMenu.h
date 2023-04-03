/******************************************************************************/
/*!
\file		MainMenu.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the main menu.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once

#include "pch.h"
#include "Global.h"

namespace main_menu
{
    enum MainMenuState
    {
        MENU,
        HOW_TO_PLAY
    };

    extern MainMenuState currentState;

    void load();
    void init();
    void update();
    void draw();
    void free();
    void unload();

    struct mm_Button
    {
        float x;
        float y;
        float width;
        float height;
    };
}

extern f32 MMg_dt;
extern f32 MMtotal_time;
