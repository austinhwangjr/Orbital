#include "AEEngine.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "MenuButtons.h"
#include <iostream>
#include "input.h"
#include "Graphics.h"

//extern s8 font1;

// Set the dimensions of each button
static float buttonWidth    = 200.f;
static float buttonHeight   = 50.f;

// Define the positions for each button
static float startX         = 0.0f;
static float startY         = -25.f;

static float howToPlayX     = 0.0f;
static float howToPlayY     = -100.f;

static float creditsX       = 0.0f;
static float creditsY       = -175.f;

static float optionsX       = 0.0f;
static float optionsY       = -250.f;

static float quitX          = 0.0f;
static float quitY          = -325.f;

// checking input area stuff
struct Button
{
    float x;
    float y;
    float width;
    float height;
};

// Define the positions and dimensions for each button
Button buttons[] = {
    {startX, startY, buttonWidth, buttonHeight},   // Start button
    {howToPlayX, howToPlayY, buttonWidth, buttonHeight},  // How to play button
    {creditsX, creditsY, buttonWidth, buttonHeight},  // Credits button
    {optionsX, optionsY, buttonWidth, buttonHeight},  // Options button
    {quitX, -quitY, buttonWidth, buttonHeight}   // Quit button
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



    if (AEInputCheckTriggered(AEVK_F11))
    {
        // If the window close button has been clicked, set the game state to quit
        Global_ToggleScreen();
        std::cout << "Toggling Screen " << std::endl;
    }

    // Check if the window close button has been clicked
    if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
    {
        // If the window close button has been clicked, set the game state to quit
        next_state = GS_QUIT;
    }
}

void Menu_Button::draw(AEGfxVertexList* pMeshMM)
{
    // Draw the start button
    RenderSprite(startButtonTexture, startX, startY, buttonWidth, buttonHeight, pMeshMM);
    /*RenderText("START", startX, startY, 20);*/

    // Draw the how to play button
    RenderSprite(howToPlayButtonTexture, howToPlayX, howToPlayY, buttonWidth, buttonHeight, pMeshMM);
    /*RenderText("HOW TO PLAY", howToPlayX, howToPlayY, 20);*/

    // Draw the credits button
    RenderSprite(creditsButtonTexture, creditsX, creditsY, buttonWidth, buttonHeight, pMeshMM);
    /*RenderText("CREDITS", creditsX, creditsY, 20);*/

    // Draw the options button
    RenderSprite(optionsButtonTexture, optionsX, optionsY, buttonWidth, buttonHeight, pMeshMM);
    /*RenderText("OPTIONS", optionsX, optionsY, 20);*/

    // Draw the quit button
    RenderSprite(exitButtonTexture, quitX, quitY, buttonWidth, buttonHeight, pMeshMM);
    /*RenderText("QUIT", quitX, quitY, 20);*/
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
