#include "AEEngine.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "MenuButtons.h"
#include <iostream>
#include "input.h"
#include "Graphics.h"
#include "HowToPlay.h"

// Set the dimensions of each button
static float buttonWidth1 = 200.f;
static float buttonHeight1 = 50.f;

AEGfxTexture* howToPlayBGTexture = nullptr;
AEGfxTexture* returnToMMTexture = nullptr;

AEGfxVertexList* pMeshHTPBackground;
AEGfxVertexList* pMeshHTP1;

Rendering createMeshHTP;
Rendering renderhtp;

static float howToPlayBGX = 0.0f;
static float howToPlayBGY = 0.0f;

static float returnToMMX = 650.0f;
static float returnToMMY = -20.0f;



// Define the positions and dimensions for each button
How_To_Play::Button1 buttons1[] = {
    {returnToMMX, returnToMMY, buttonWidth1, buttonHeight1},   // Return to main menu button
};



void How_To_Play::load()
{
    howToPlayBGTexture = AEGfxTextureLoad("Assets/howToPlay.png");
    returnToMMTexture = AEGfxTextureLoad("Assets/p_ExitMainMenuButton.png");
}

void How_To_Play::init()
{
    createMeshHTP.BackgroundMesh(pMeshHTPBackground);
    createMeshHTP.SquareMesh(pMeshHTP1);
}

void How_To_Play::update()
{
    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked

        if (Input::isButtonClicked(buttons1[0].x, buttons1[0].y, buttons1[0].width, buttons1[0].height))
        {

            next_state = GS_MAINMENU;
        }
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

void How_To_Play::draw()
{
    // Clear the screen
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    // Draw the start button
    renderhtp.RenderSprite(howToPlayBGTexture, howToPlayBGX, howToPlayBGY, 800, 450, pMeshHTPBackground);
    renderhtp.RenderSprite(returnToMMTexture, returnToMMX, returnToMMY, buttonWidth1, buttonHeight1, pMeshHTP1);
}


void How_To_Play::free()
{
    AEGfxMeshFree(pMeshHTPBackground);
    AEGfxMeshFree(pMeshHTP1);

}

void How_To_Play::unload()
{
    // unload the texture for the start button
    AEGfxTextureUnload(howToPlayBGTexture);
    AEGfxTextureUnload(returnToMMTexture);

}