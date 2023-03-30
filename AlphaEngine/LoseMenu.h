/******************************************************************************/
/*!
\file		LoseMenu.h
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the lose menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
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