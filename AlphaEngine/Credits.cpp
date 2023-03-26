#include "AEEngine.h"
#include <iostream>
#include "Global.h"
#include "GameStateManager.h"
#include "MainMenu.h"
#include "MenuButtons.h"
#include "Graphics.h"
#include "Credits.h"
#include "Input.h"

// Set the dimensions of each button
static float buttonWidth2 = 200.f;
static float buttonHeight2 = 50.f;

int currentPage = 1;

AEGfxTexture* texCreditsBackground = nullptr;
AEGfxTexture* texCreditsBackground2 = nullptr;
AEGfxTexture* texReturnToMMfromCredits = nullptr;

AEGfxVertexList* pMeshCreditsBackground;
AEGfxVertexList* pMeshCredits;

Rendering createMeshCredits;
Rendering renderCredits;

static float creditsBGX = 0.0f;
static float creditsBGY = 0.0f;

static float returnToMMfromCreditsX = 650.0f;
static float returnToMMfromCreditsY = -20.0f;

// Define the positions and dimensions for each button
credits::Button2 buttons2[] = {
	{returnToMMfromCreditsX, returnToMMfromCreditsY, buttonWidth2, buttonHeight2},   // Return to main menu button
};

void credits::load()
{
	texCreditsBackground = AEGfxTextureLoad("Assets/MainMenu/Credits/c_credits1.png");
    texCreditsBackground2 = AEGfxTextureLoad("Assets/MainMenu/Credits/c_credits2.png");
	texReturnToMMfromCredits = AEGfxTextureLoad("Assets/MainMenu/Credits/c_ExitButtonCredits.png");
}

void credits::init()
{
	createMeshCredits.BackgroundMesh(pMeshCreditsBackground);
	createMeshCredits.SquareMesh(pMeshCredits);

	AE_ASSERT_MESG(pMeshCreditsBackground, "Error: Failed to create pMeshCreditsBackground in Credits.cpp!");
	AE_ASSERT_MESG(pMeshCredits, "Error: Failed to create pMeshCredits in Credits.cpp!");
}

void credits::update()
{
    if (AEInputCheckTriggered(AEVK_LBUTTON) && currentPage > 1)
    {
        currentPage--;
    }
    if (AEInputCheckTriggered(AEVK_RBUTTON) && currentPage < 2)
    {
        currentPage++;
    }

    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked

        if (Input::isButtonClicked(buttons2[0].x, buttons2[0].y, buttons2[0].width, buttons2[0].height))
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

void credits::draw()
{
    if (currentPage == 1)
    {
        renderCredits.RenderSprite(texCreditsBackground, 0.f, 0.f, 800.f, 450.f, pMeshCreditsBackground);
    }
    else if (currentPage == 2)
    {
        renderCredits.RenderSprite(texCreditsBackground2, 0.f, 0.f, 800.f, 450.f, pMeshCreditsBackground);
    }

    renderCredits.RenderSprite(texReturnToMMfromCredits, returnToMMfromCreditsX, returnToMMfromCreditsY, buttonWidth2, buttonHeight2, pMeshCredits);
}

void credits::free()
{
	AEGfxMeshFree(pMeshCreditsBackground);
	AEGfxMeshFree(pMeshCredits);
}

void credits::unload()
{
	AEGfxTextureUnload(texCreditsBackground);
    AEGfxTextureUnload(texCreditsBackground2);
	AEGfxTextureUnload(texReturnToMMfromCredits);

}