/******************************************************************************/
/*!
\file		Credits.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the credits 
            screen.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "Graphics.h"

namespace credits
{
    void load();
    void init();
    void update();
    void draw();
    void free();
    void unload();

    struct Button2
    {
        float x;
        float y;
        float width;
        float height;
    };
    
    struct HoverButton2
    {
        float width;
        float height;
    };
}
