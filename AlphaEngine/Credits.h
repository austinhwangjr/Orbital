/******************************************************************************/
/*!
\file		Credits.h.cpp
\author 	 
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the credits 
            screen.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
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
