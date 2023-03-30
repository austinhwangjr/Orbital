/******************************************************************************/
/*!
\file		PauseMenuButtons.cpp
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the buttons in 
            the pause menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
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
                            const char* exitMainMenuFilename,
                            const char* fadeMenuFilename)
{
    resumeTexture = AEGfxTextureLoad(resumeButtonFilename);
    restartTexture = AEGfxTextureLoad(restartButtonFilename);
    exitMainMenuTexture = AEGfxTextureLoad(exitMainMenuFilename);
    fadeTex = AEGfxTextureLoad(fadeMenuFilename);
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
        case 0: current_state = GS_MAINLEVEL;
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

void PauseMenuButtons::draw(AEGfxVertexList* pMeshP, const AEVec2& camPos)
{
    // Calculate the dimensions of the screen
    float screenWidth = static_cast<float>(AEGetWindowWidth());
    float screenHeight = static_cast<float>(AEGetWindowHeight());

    float translatedCentX = camPos.x;
    float translatedCentY = camPos.y;

    float translatedResumeX = resumeX + camPos.x;
    float translatedResumeY = resumeY + camPos.y;
    float translatedRestartX = restartX + camPos.x;
    float translatedRestartY = restartY + camPos.y;
    float translatedExitMainMenuX = exitMainMenuX + camPos.x;
    float translatedExitMainMenuY = exitMainMenuY + camPos.y;

    // Render the faded background
    RenderFadedBackground(fadeTex, translatedCentX, translatedCentY, screenWidth, screenHeight, pMeshP, 0.5f);

    RenderSprite(resumeTexture, translatedResumeX, translatedResumeY, pauseMButtonWidth, pauseMButtonHeight, pMeshP);
    RenderSprite(restartTexture, translatedRestartX, translatedRestartY, pauseMButtonWidth, pauseMButtonHeight, pMeshP);
    RenderSprite(exitMainMenuTexture, translatedExitMainMenuX, translatedExitMainMenuY, pauseMButtonWidth, pauseMButtonHeight, pMeshP);
}


void PauseMenuButtons::free()
{

}

void PauseMenuButtons::unload()
{
    AEGfxTextureUnload(resumeTexture);
    AEGfxTextureUnload(restartTexture);
    AEGfxTextureUnload(exitMainMenuTexture);
    AEGfxTextureUnload(fadeTex);

}