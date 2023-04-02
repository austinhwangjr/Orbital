/******************************************************************************/
/*!
\file		Input.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for button click and
            mouse hover.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
extern int mouseX;
extern int mouseY;

class Input
{
public:
    Input();

    // Static function to check if a button is clicked by the user
    static bool isButtonClicked(float center_x, float center_y, float width, float height);

    // Static function to check if the mouse is hovering over a button
    static bool isMouseHover(float center_x, float center_y, float width, float height);

    static int GetMouseWheelDelta();
};
