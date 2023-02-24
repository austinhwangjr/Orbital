#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "MenuButtons.h"
#include <iostream>
#include "input.h"
#include "Graphics.h"

struct Button {
    float x;
    float y;
    float width;
    float height;
};

// Define the positions and dimensions for each button
Button buttons[] = {
    {0.0f, -25.0f, 200.0f, 50.0f},   // Start button
    {0.0f, -100.0f, 200.0f, 50.0f},  // How to play button
    {0.0f, -175.0f, 200.0f, 50.0f},  // Credits button
    {0.0f, -250.0f, 200.0f, 50.0f},  // Options button
    {0.0f, -325.0f, 200.0f, 50.0f}   // Quit button
};

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
    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked
        int clickedButton = -1;
        for (int i = 0; i < 5; i++) {
            if (Input::isButtonClicked(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height)) {
                clickedButton = i;
                break;
            }
        }

        // Set the next game state based on the button that was clicked
        switch (clickedButton)
        {
        case 0: next_state = GS_MAINLEVEL;  break;
        case 1: next_state = GS_HOWTOPLAY;  break;
        case 2: next_state = GS_CREDITS;    break;
        case 3: next_state = GS_OPTIONS;    break;
        case 4: next_state = GS_QUIT;       break;
        }

        // debugging yy
        std::cout << "Clicked button: " << clickedButton << std::endl;
        std::cout << "Next game state: " << next_state << std::endl;
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

    // Draw the quit buttona
    RenderSprite(exitButtonTexture, quitX, quitY, buttonWidth, buttonHeight, pMesh1);
}

void Menu_Button::free()
{
    // empty
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
