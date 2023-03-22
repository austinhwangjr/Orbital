#include "LoseMenu.h"
#include "AEEngine.h"
#include "Graphics.h"
#include "Input.h"
#include "GameStateManager.h"
#include "Global.h"
#include <iostream>

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
}

void lose_menu::init()
{
	renderLoseMenu.SquareMesh(pMeshLoseMenu);

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
            next_state = GS_RESTART;
            break;
        case 1:
            next_state = GS_MAINMENU;
            break;
        }

        if (AEInputCheckTriggered(AEVK_F11))
        {
            // If the window close button has been clicked, set the game state to quit
            Global_ToggleScreen();
            std::cout << "Toggling Screen " << std::endl;
        }
    }
}

void lose_menu::draw(const AEVec2& camPos)
{
    float translatedPlayAgainX = lm_PlayAgainX + camPos.x;
    float translatedPlayAgainY = lm_PlayAgainY + camPos.y;
    float translatedMainMenuX = lm_MainMenuX + camPos.x;
    float translatedMainMenuY = lm_MainMenuY + camPos.y;

    renderLoseMenu.RenderSprite(TexPlayAgain, translatedPlayAgainX, translatedPlayAgainY, lm_ButtonWidth, lm_ButtonHeight, pMeshLoseMenu);
    renderLoseMenu.RenderSprite(TexMainMenu, translatedMainMenuX, translatedMainMenuY, lm_ButtonWidth, lm_ButtonHeight, pMeshLoseMenu);
}


void lose_menu::free()
{
    AEGfxMeshFree(pMeshLoseMenu);
}

void lose_menu::unload()
{
    AEGfxTextureUnload(TexPlayAgain);
    AEGfxTextureUnload(TexMainMenu);

}
