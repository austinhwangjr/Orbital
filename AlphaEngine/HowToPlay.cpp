/******************************************************************************/
/*!
\file		HowToPlay.cpp
\author 	Ong You Yang (100%)
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the "How-to-Play" 
            feature.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "MenuButtons.h"
#include <iostream>
#include "input.h"
#include "Graphics.h"
#include "HowToPlay.h"
#include <iostream>
#include <fstream>
#include "data.h"

// Set the dimensions of each button
static float buttonWidth1;
static float buttonHeight1;

AEGfxTexture* howToPlayBGTexture = nullptr;
AEGfxTexture* returnToMMTexture = nullptr;

AEGfxVertexList* pMeshHTPBackground;
AEGfxVertexList* pMeshHTP1;

Rendering createMeshHTP;
Rendering renderhtp;

static float howToPlayBGX;
static float howToPlayBGY;

static float returnToMMX;
static float returnToMMY;

how_to_play::Button1 buttons1;


void how_to_play::load()
{
    howToPlayBGTexture = AEGfxTextureLoad("Assets/MainMenu/HowToPlay/htp_HowToPlayBackground.png");
    returnToMMTexture = AEGfxTextureLoad("Assets/MainMenu/HowToPlay/htp_ExitButton.png");
    ImportDataFromFileHowToPlay("Assets/MainMenu/HowToPlay/HowToPlayData.txt");
    
}

void how_to_play::init()
{
    createMeshHTP.BackgroundMesh(pMeshHTPBackground);
    createMeshHTP.SquareMesh(pMeshHTP1);
    // Define the positions and dimensions for each button
    buttons1.x = returnToMMX;
    buttons1.y = returnToMMY;
    buttons1.width = buttonWidth1;
    buttons1.height = buttonHeight1;

  
}

void how_to_play::update()
{
    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked

        if (Input::isButtonClicked(buttons1.x, buttons1.y, buttons1.width, buttons1.height))
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

void how_to_play::draw()
{
    // Clear the screen
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    // Draw the start button
    renderhtp.RenderSprite(howToPlayBGTexture, howToPlayBGX, howToPlayBGY, 800, 450, pMeshHTPBackground);
    renderhtp.RenderSprite(returnToMMTexture, returnToMMX, returnToMMY, buttonWidth1, buttonHeight1, pMeshHTP1);
}


void how_to_play::free()
{
    AEGfxMeshFree(pMeshHTPBackground);
    AEGfxMeshFree(pMeshHTP1);

}

void how_to_play::unload()
{
    // unload the texture for the start button
    AEGfxTextureUnload(howToPlayBGTexture);
    AEGfxTextureUnload(returnToMMTexture);

}

int ImportDataFromFileHowToPlay(const char* FileName)
{

    std::ifstream ifs{ FileName, std::ios_base::in };
    if (!ifs.is_open()) {											// Check if file exist/open	
        std::cout << FileName << "does not exist." << '\n';
        return 0;
    }

    ifs.ignore(400, ' ');				
    ifs >> buttonWidth1;
    ifs.ignore(400, ' ');				
    ifs >> buttonHeight1;
    ifs.ignore(400, ' ');
    ifs >> howToPlayBGX;
    ifs.ignore(400, ' ');
    ifs >> howToPlayBGY;
    ifs.ignore(400, ' ');
    ifs >> returnToMMX;
    ifs.ignore(400, ' ');
    ifs >> returnToMMY;
    
    ifs.close();

    return 1;
}