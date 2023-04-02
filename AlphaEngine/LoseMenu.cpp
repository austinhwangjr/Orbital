/******************************************************************************/
/*!
\file		LoseMenu.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the lose menu.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "LoseMenu.h"
#include "AEEngine.h"
#include "Graphics.h"
#include "Input.h"
#include "GameStateList.h"
#include "Global.h"
#include <iostream>
#include "Highscore.h"

AEGfxTexture* TexPlayAgain = nullptr;
AEGfxTexture* TexMainMenu = nullptr;

AEGfxVertexList* pMeshLoseMenu;

Rendering renderLoseMenu;

// Set the dimensions of each button
static float lm_ButtonWidth = 200.f;
static float lm_ButtonHeight = 50.f;

// Define the positions for each button
static float lm_PlayAgainX = 0.0f;
static float lm_PlayAgainY = -25.f;

static float lm_MainMenuX = 0.0f;
static float lm_MainMenuY = -175.f;

// checking input area stuff
struct lm_Button
{
	float x;
	float y;
	float width;
	float height;
};

// Define the positions and dimensions for each button
lose_menu::lm_Button lm_buttons[] =
{
	{lm_PlayAgainX, lm_PlayAgainY, lm_ButtonWidth, lm_ButtonHeight},   // Start button
	{lm_MainMenuX, lm_MainMenuY, lm_ButtonWidth, lm_ButtonHeight},  // How to play button
};

void lose_menu::load()
{
	TexPlayAgain = AEGfxTextureLoad("Assets/MainLevel/LoseMenu/lm_PlayAgainButton.png");
	TexMainMenu = AEGfxTextureLoad("Assets/MainLevel/LoseMenu/lm_MainMenuButton.png");

    //AudioManager::LoadSound("Assets/BGM/bgm_ml_losemenu.wav", false);
}

void lose_menu::init()
{
	renderLoseMenu.SquareMesh(pMeshLoseMenu);
    AudioManager::PlayOnce("Assets/BGM/bgm_ml_losemenu.wav", 0.5f);

}

void lose_menu::update()
{

    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked
        int clickedButton = -1;
        for (int i = 0; i < 2; i++) {
            if (Input::isButtonClicked(lm_buttons[i].x, lm_buttons[i].y, lm_buttons[i].width, lm_buttons[i].height)) {
                clickedButton = i;
                break;
            }
        }

        switch (clickedButton)
        {
        case 0:
            next_state = GS_MAINLEVEL;
            break;
        case 1:
            next_state = GS_MAINMENU;
            putHighScore(player_score);
            break;
        }

        if (AEInputCheckTriggered(AEVK_F11))
        {
            // If the window close button has been clicked, set the game state to quit
            Global_ToggleScreen();
            std::cout << "Toggling Screen " << std::endl;
        }
    }
    AudioManager::Update();

}

void lose_menu::draw()
{
    /*f32 camPos_x{}, camPos_y{};
    AEGfxGetCamPosition(&camPos_x, &camPos_y);*/

    float translatedPlayAgainX = lm_PlayAgainX + g_camPos.x;
    float translatedPlayAgainY = lm_PlayAgainY + g_camPos.y;
    float translatedMainMenuX = lm_MainMenuX + g_camPos.x;
    float translatedMainMenuY = lm_MainMenuY + g_camPos.y;

    renderLoseMenu.RenderSprite(TexPlayAgain, translatedPlayAgainX, translatedPlayAgainY, lm_ButtonWidth, lm_ButtonHeight, pMeshLoseMenu);
    renderLoseMenu.RenderSprite(TexMainMenu, translatedMainMenuX, translatedMainMenuY, lm_ButtonWidth, lm_ButtonHeight, pMeshLoseMenu);
}


void lose_menu::free()
{
    AEGfxMeshFree(pMeshLoseMenu);
    AudioManager::UnloadAllSounds();
}

void lose_menu::unload()
{
    AEGfxTextureUnload(TexPlayAgain);
    AEGfxTextureUnload(TexMainMenu);

}
