/******************************************************************************/
/*!
\file		PauseMenu.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the pause menu.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once

#include "AEEngine.h"

struct pause_menu
{
    void load();
    void init();
    void update();
    void draw(AEVec2 const& camPos);
    void free();
    void unload();
};