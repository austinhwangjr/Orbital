/******************************************************************************/
/*!
\file		Input.h
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for button click and
            mouse hover.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
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
    static bool isMouseHover(float center_x, float center_y, float width, float height, float hoverWidth, float hoverHeight);

    static int GetMouseWheelDelta();

    // Member function to handle input
//void handle();
};
