/******************************************************************************/
/*!
\file		Options.h
\author 	Ong You Yang
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the options
            screen.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
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