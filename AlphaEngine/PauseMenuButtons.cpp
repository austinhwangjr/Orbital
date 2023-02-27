#include "PauseMenuButtons.h"
#include "Global.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "Input.h"
#include "Graphics.h"
#include "MainLevel.h"
#include <iostream>
#include "PauseMenu.h"

// Set the dimensions of each button
static float pauseMButtonWidth = 200.f;
static float pauseMButtonHeight = 50.f;

// Define the positions for each button
static float resumeX = 0.0f;
static float resumeY = -25.f;

static float restartX = 0.0f;
static float restartY = -100.f;

static float exitMainMenuX = 0.0f;
static float exitMainMenuY = -175.f;

// checking input area stuff
struct Button1
{
    float x;
    float y;
    float width;
    float height;
};

// Define the positions and dimensions for each button
Button1 buttons1[] =
{
    {resumeX, resumeY, pauseMButtonWidth, pauseMButtonHeight},   // Start button
    {restartX, restartY, pauseMButtonWidth, pauseMButtonHeight},  // How to play button
    {exitMainMenuX, exitMainMenuY, pauseMButtonWidth, pauseMButtonHeight},  // Credits button
};


void PauseMenuButtons::load(const char* resumeButtonFilename,
    const char* restartButtonFilename,
    const char* exitMainMenuFilename)
{
    resumeTexture = AEGfxTextureLoad(resumeButtonFilename);
    restartTexture = AEGfxTextureLoad(restartButtonFilename);
    exitMainMenuTexture = AEGfxTextureLoad(exitMainMenuFilename);
}

void PauseMenuButtons::init()
{
    //empty
}

void PauseMenuButtons::update()
{
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked
        int clickedButton = -1;
        for (int i = 0; i < 3; i++) {
            if (Input::isButtonClicked(buttons1[i].x, buttons1[i].y, buttons1[i].width, buttons1[i].height)) {
                clickedButton = i;
                break;
            }
        }

        // Set the next game state based on the button that was clicked
        switch (clickedButton)
        {
        case 0: current_state = previous_state;
                is_paused = false;
                break;
        case 1: next_state = GS_RESTART;
                is_paused = false;
                break;
        case 2: next_state = GS_MAINMENU;
                is_paused = false;
                break;
        }

        // debugging yy
        std::cout << "Clicked button: " << clickedButton << std::endl;
        std::cout << "Next game state: " << next_state << std::endl;
        std::cout << "GameState changed to: " << current_state << std::endl;

        //gsm::changeGameState(next_state); // add this line to change the game state

    }
}


void PauseMenuButtons::draw(AEGfxVertexList* pMeshP)
{
    RenderSprite(resumeTexture, resumeX, resumeY, pauseMButtonWidth, pauseMButtonHeight, pMeshP);
    RenderSprite(restartTexture, restartX, restartY, pauseMButtonWidth, pauseMButtonHeight, pMeshP);
    RenderSprite(exitMainMenuTexture, exitMainMenuX, exitMainMenuY, pauseMButtonWidth, pauseMButtonHeight, pMeshP);
}


void PauseMenuButtons::free()
{

}

void PauseMenuButtons::unload()
{
    AEGfxTextureUnload(resumeTexture);
    AEGfxTextureUnload(restartTexture);
    AEGfxTextureUnload(exitMainMenuTexture);
}