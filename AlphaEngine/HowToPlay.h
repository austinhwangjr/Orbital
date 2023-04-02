/******************************************************************************/
/*!
\file		HowToPlay.h
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the "How-to-Play" 
            feature.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "Graphics.h"

namespace how_to_play
{
    void load();
    void init();
    void update();
    void draw();
    void free();
    void unload();

    // checking input area stuff
struct Button1
{
    float x;
    float y;
    float width;
    float height;
};

}