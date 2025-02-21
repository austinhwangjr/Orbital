/******************************************************************************/
/*!
\file		PauseMenuButtons.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the buttons in 
            the pause menu.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
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
static float resumeY = 50.f;

static float restartX = 0.0f;
static float restartY = -50.f;

static float exitMainMenuX = 0.0f;
static float exitMainMenuY = -150.f;

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

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void PauseMenuButtons::init()
{
    //empty
}

/******************************************************************************/
/*!
	Update Pause Menu Buttons
*/
/******************************************************************************/
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
    }
}

/******************************************************************************/
/*!
	Draw Pause Menu Buttons
*/
/******************************************************************************/
void PauseMenuButtons::draw(AEGfxVertexList* pMeshP, const AEVec2& camPos)
{
    // Calculate the dimensions of the screen
    float screenWidth = static_cast<float>(g_windowWidth);
    float screenHeight = static_cast<float>(g_windowHeight);

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


/******************************************************************************/
/*!
	Clean Object Instances
*/
/******************************************************************************/
void PauseMenuButtons::free()
{

}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void PauseMenuButtons::unload()
{
    AEGfxTextureUnload(resumeTexture);
    AEGfxTextureUnload(restartTexture);
    AEGfxTextureUnload(exitMainMenuTexture);
    AEGfxTextureUnload(fadeTex);
}