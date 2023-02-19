/* Start Header ************************************************************************/
/*!
\file		Input.cpp
\author		Hwang Jing Rui, Austin, jingruiaustin.hwang, 2200601
\par		jingruiaustin.hwang\@digipen.edu
\date		Jan 14, 2023
\brief		This file contains the definition of the Input_Handle() function for the 
			input handler.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "pch.h"
#include "Input.h"
#include <iostream>
#include "global.h"

// This initializes the mouse coordinates to 0.
int mouseX = 0;
int mouseY = 0;

// ----------------------------------------------------------------------------
// This function handles the input in the game loop
// ----------------------------------------------------------------------------
void input_handle()
{
	// Message to show input handling is successfully carried out
	//std::cout << "Input:Handle\n";
}

// ----------------------------------------------------------------------------
// This function checks if the button is clicked by the user and returns true if the button is clicked,
// and false otherwise.
// ----------------------------------------------------------------------------
bool IsButtonClicked(float center_x, float center_y, float width, float height)
{
    // Get the current mouse position
    AEInputGetCursorPosition(&mouseX, &mouseY);

    // Convert the click coordinates to normalized device coordinates
    float screen_x = (mouseX / (float)g_windowWidth) * 2.0f - 1.0f;
    float screen_y = (mouseY / (float)g_windowHeight) * -2.0f + 1.0f;

    // Calculate the minimum and maximum x and y coordinates of the button
    float x_min = center_x - width / 2.0f;
    float x_max = center_x + width / 2.0f;
    float y_min = center_y - height / 2.0f;
    float y_max = center_y + height / 2.0f;

    // Check if the click coordinates are within the bounds of the button
    if (screen_x >= x_min && screen_x <= x_max && screen_y >= y_min && screen_y <= y_max) {
        return true;
    }

    return false;
}
