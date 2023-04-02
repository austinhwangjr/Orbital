/******************************************************************************/
/*!
\file		Input.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for button click and
            mouse hover.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "pch.h"
#include "Input.h"
#include "Global.h"
#include "Graphics.h"
#include <iostream>

// This initializes the mouse coordinates to 0.
int mouseX = 0;
int mouseY = 0;

// Constructor
Input::Input()
{}

// ----------------------------------------------------------------------------
/*
 * Check if a button is clicked by the user.
 * @param center_x The x coordinate of the center of the button.
 * @param center_y The y coordinate of the center of the button.
 * @param width The width of the button.
 * @param height The height of the button.
 * @return True if the button is clicked, false otherwise.
 */
 // ----------------------------------------------------------------------------
bool Input::isButtonClicked(float center_x, float center_y, float width, float height)
{
    // Get the current mouse position
    AEInputGetCursorPosition(&mouseX, &mouseY);

    // Get the window width and height
    int windowWidth  = static_cast<int>(g_windowWidth);
    int windowHeight = static_cast<int>(g_windowHeight);

    // Convert the click coordinates to normalized device coordinates
    float screen_x = mouseX - (float)windowWidth / 2.0f;
    float screen_y = (float)windowHeight / 2.0f - mouseY;

    // Calculate the minimum and maximum x and y coordinates of the button
    float x_min = center_x - width / 2.0f;
    float x_max = center_x + width / 2.0f;
    float y_min = center_y - height / 2.0f;
    float y_max = center_y + height / 2.0f;

    // Debugging output (Uncomment when got input issue)
    /*std::cout << "Mouse coordinates: (" << mouseX << ", " << mouseY << ")\n";
    std::cout << "Normalized device coordinates: (" << screen_x << ", " << screen_y << ")\n";
    std::cout << "Button bounds: (" << "Xmin: " << x_min << ", " << "Ymin: " << y_min << ") to (" << "Xmax: " << x_max << ", " << "Ymax: " << y_max << ")\n";*/

    // Check if the click coordinates are within the bounds of the button
    if (screen_x >= x_min && screen_x <= x_max && screen_y >= y_min && screen_y <= y_max)
    {
        return true;
    }
    return false;
}

bool Input::isMouseHover(float center_x, float center_y, float width, float height) {
    // Get the current mouse position
    AEInputGetCursorPosition(&mouseX, &mouseY);

    // Get the window width and height
    int windowWidth  = static_cast<int>(g_windowWidth);
    int windowHeight = static_cast<int>(g_windowHeight);

    // Convert the mouse coordinates to normalized device coordinates
    float screen_x = mouseX - (float)windowWidth / 2.0f;
    float screen_y = (float)windowHeight / 2.0f - mouseY;

    // Calculate the minimum and maximum x and y coordinates of the button
    float x_min = center_x - width / 2.0f;
    float x_max = center_x + width / 2.0f;
    float y_min = center_y - height / 2.0f;
    float y_max = center_y + height / 2.0f;

    // Check if the mouse coordinates are within the bounds of the button
    if (screen_x >= x_min && screen_x <= x_max && screen_y >= y_min && screen_y <= y_max)
    {
        return true;
    }
    return false;
}

int Input::GetMouseWheelDelta()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, WM_MOUSEWHEEL, WM_MOUSEWHEEL, PM_REMOVE))
    {
        if (msg.message == WM_MOUSEWHEEL)
        {
            return GET_WHEEL_DELTA_WPARAM(msg.wParam);
        }
    }
    return 0;
}

