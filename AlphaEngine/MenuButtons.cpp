#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "MenuButtons.h"
#include <iostream>
#include "input.h"
#include "Graphics.h"

// declare a pointer to a texture object for the start button
//AEGfxTexture* startButtonTex;

// declare variables for the transformation matrix
//static float texWidth = 100.0f;
//static float texHeight = 50.0f;
AEMtx33 transform;
AEMtx33 scale = {};
AEMtx33 rotate = {};
AEMtx33 translate = {};

void Menu_Button::load(const char* filename)
{
    buttonTexture = AEGfxTextureLoad(filename);
    //howToPlayButtonTexture = AEGfxTextureLoad(filename);
    //creditsButtonTexture = AEGfxTextureLoad(filename);
    //optionsButtonTexture = AEGfxTextureLoad(filename);
    quitButtonTexture = AEGfxTextureLoad(filename);
}


void Menu_Button::init()
{
    // empty initialization function
}

void Menu_Button::update()
{
    // Set the dimensions of the button
    float width = 200.f;
    float height = 50.f;

    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check if the mouse is within the bounds of the start button
        float center_x = 0.0f;
        float center_y = -25.f;
        if (Input::isButtonClicked(center_x, center_y, width, height))
        {
            // if the start button is clicked, change the game state to main level
            if (current_state == GS_MAINMENU)
            {
                next_state = GS_MAINLEVEL;
                std::cout << "GameState changed to: " << current_state << std::endl;
            }
        }
    }

    

    //// Check if the Quit button has been clicked
    //if (AEInputCheckTriggered(AEVK_LBUTTON))
    //{
    //    float center_x = 0.0f;
    //    float center_y = -1.5f * height;
    //    if (Input::isButtonClicked(center_x, center_y, width, height))
    //    {
    //        // Quit the game
    //        next_state = GS_QUIT;
    //    }
    //}

    //// check if the escape key has been pressed or the window has been closed
    //if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
    //    next_state = GS_QUIT;
}

void Menu_Button::draw(AEGfxVertexList* pMesh1)
{
    // Set the position and dimensions of the button
    float centerX = 0.0f;
    float centerY = -25.0f;
    float width = 200.f;
    float height = 50.f;

    // Call the DrawButton function to draw the button with the desired texture and dimensions
    RenderSprite(this->buttonTexture, centerX, centerY, width, height, pMesh1);
    
    //// Draw the How To Play button
    //centerX = 0.0f;
    //centerY = -1.5f * height;
    //RenderSprite(quitButtonTexture, centerX, centerY, width, height, pMesh1);
}

void Menu_Button::free()
{
    // empty free function
}

void Menu_Button::unload()
{
    // unload the texture for the start button
    AEGfxTextureUnload(buttonTexture);
    AEGfxTextureUnload(quitButtonTexture);
}
