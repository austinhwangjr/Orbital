#include "pch.h"
#include "Input.h"

#include "Global.h"
#include "Graphics.h"
#include <algorithm>
#include <iostream>

// This initializes the mouse coordinates to 0.
int mouseX = 0;
int mouseY = 0;

// Constructor
Input::Input()
{
}

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
    int windowWidth = AEGetWindowWidth();
    int windowHeight = AEGetWindowHeight();

    // Convert the click coordinates to normalized device coordinates
    float screen_x = mouseX - (float)windowWidth / 2.0f;
    float screen_y = (float)windowHeight / 2.0f - mouseY;

  /*  float screen_x = (mouseX / (float)windowWidth) * 2.0f - 1.0f;
    float screen_y = (mouseY / (float)windowHeight) * -2.0f + 1.0f;*/

    // Calculate the minimum and maximum x and y coordinates of the button
    float x_min = center_x - width / 2.0f;
    float x_max = center_x + width / 2.0f;
    float y_min = center_y - height / 2.0f;
    float y_max = center_y + height / 2.0f;

    // Debugging output
    std::cout << "Mouse coordinates: (" << mouseX << ", " << mouseY << ")\n";
    std::cout << "Normalized device coordinates: (" << screen_x << ", " << screen_y << ")\n";
    std::cout << "Button bounds: (" << "Xmin: " << x_min << ", " << "Ymin: " << y_min << ") to (" << "Xmax: " << x_max << ", " << "Ymax: " << y_max << ")\n";

    // Check if the click coordinates are within the bounds of the button
    if (screen_x >= x_min && screen_x <= x_max && screen_y >= y_min && screen_y <= y_max) {
        return true;
    }
    return false;
    
}


//// ----------------------------------------------------------------------------
//// This member function handles the input in the game loop
//// ----------------------------------------------------------------------------
//void Input::handle()
//{
//    // Message to show input handling is successfully carried out
//    //std::cout << "Input:Handle\n";
//}

