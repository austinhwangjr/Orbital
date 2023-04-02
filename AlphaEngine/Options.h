/******************************************************************************/
/*!
\file		Options.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the options
            screen.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#ifndef OPTIONS_H
#define OPTIONS_H

#include "AEEngine.h"

namespace Options
{
    void load();
    void init();
    void update(float* options_volume, bool* options_muted);
    void draw();
    void free();
    void unload();

    extern bool muteHoverState;
    extern bool draggingSlider;

    extern float muteButtonX;
    extern float muteButtonY;
    extern float sliderX;
    extern float sliderY;
    extern float sliderThumbX;
    extern float sliderThumbY;

    extern float volume;
    extern bool muted;

    struct Button11
    {
        float x;
        float y;
        float width;
        float height;
    };
}

#endif // OPTIONS_H