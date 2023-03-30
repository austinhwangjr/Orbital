/******************************************************************************/
/*!
\file		MainMenu.h
\author 	Ong You Yang
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the main menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once

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
}

extern f64 MMframe_time;
extern f64 MMtotal_time;

