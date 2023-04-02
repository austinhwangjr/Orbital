/******************************************************************************/
/*!
\file		LoseMenu.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the lose menu.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "AEEngine.h"

namespace lose_menu
{
    void load();
    void init();
    void update();
    //void draw(const AEVec2& camPos);
    void draw();
    void free();
    void unload();

    struct lm_Button
    {
        float x;
        float y;
        float width;
        float height;
    };
}