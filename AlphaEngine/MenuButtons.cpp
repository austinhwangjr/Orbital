#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "MenuButtons.h"
#include <iostream>
#include "input.h"
#include "Graphics.h"

void Menu_Button::load(const char* startButtonFilename,
                       const char* howToPlayButtonFilename,
                       const char* creditsButtonFilename,
                       const char* optionsButtonFilename,
                       const char* exitButtonFilename)
{
    startButtonTexture     =    AEGfxTextureLoad(startButtonFilename);
    howToPlayButtonTexture =    AEGfxTextureLoad(howToPlayButtonFilename);
    creditsButtonTexture   =    AEGfxTextureLoad(creditsButtonFilename);
    optionsButtonTexture   =    AEGfxTextureLoad(optionsButtonFilename);
    exitButtonTexture      =    AEGfxTextureLoad(exitButtonFilename);
}

void Menu_Button::init()
{
    // empty initialization function
}

void Menu_Button::update()
{
    // Set the dimensions of the buttons
    float buttonWidth = 200.f;
    float buttonHeight = 50.f;

    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        std::cout << "Left mouse button triggered" << std::endl;

        // Check which button has been clicked
        float center_x = 0.0f;
        float center_y = -25.f;
        int clickedButton = Input::isButtonClicked(center_x, center_y, buttonWidth, buttonHeight);

        // Set the next game state based on the button that was clicked
        switch (clickedButton)
        {
        case 1: next_state = GS_MAINLEVEL;  break;
        case 2: next_state = GS_HOWTOPLAY;  break;
        case 3: next_state = GS_CREDITS;    break;
        case 4: next_state = GS_OPTIONS;    break;
        case 5: next_state = GS_QUIT;       break;
        }

        // Output the new game state
        std::cout << "GameState changed to: " << current_state << std::endl;
    }

    // Check if the window close button has been clicked
    if (AEInputCheckTriggered(AEVK_ESCAPE))
    {
        // If the window close button has been clicked, set the game state to quit
        next_state = GS_QUIT;
        std::cout << "GameState changed to: " << current_state << std::endl;
    }
}



void Menu_Button::draw(AEGfxVertexList* pMesh1)
{
    // Set the dimensions of each button
    float buttonWidth = 200.f;
    float buttonHeight = 50.f;

    // Define the positions for each button
    float startX = 0.0f;
    float startY = -25.f;

    float howToPlayX = 0.0f;
    float howToPlayY = -100.f;

    float creditsX = 0.0f;
    float creditsY = -175.f;

    float optionsX = 0.0f;
    float optionsY = -250.f;

    float quitX = 0.0f;
    float quitY = -325.f;

    // Draw the start button
    RenderSprite(startButtonTexture, startX, startY, buttonWidth, buttonHeight, pMesh1);

    // Draw the how to play button
    RenderSprite(howToPlayButtonTexture, howToPlayX, howToPlayY, buttonWidth, buttonHeight, pMesh1);

    // Draw the credits button
    RenderSprite(creditsButtonTexture, creditsX, creditsY, buttonWidth, buttonHeight, pMesh1);

    // Draw the options button
    RenderSprite(optionsButtonTexture, optionsX, optionsY, buttonWidth, buttonHeight, pMesh1);

    // Draw the quit button
    RenderSprite(exitButtonTexture, quitX, quitY, buttonWidth, buttonHeight, pMesh1);
}

void Menu_Button::free()
{
    // empty free function
}

void Menu_Button::unload()
{
    // unload the texture for the start button
    AEGfxTextureUnload(startButtonTexture);
    AEGfxTextureUnload(howToPlayButtonTexture);
    AEGfxTextureUnload(creditsButtonTexture);
    AEGfxTextureUnload(optionsButtonTexture);
    AEGfxTextureUnload(exitButtonTexture);
   
}
