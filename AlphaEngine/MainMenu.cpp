/*-------------------------------------------------- Main Menu --------------------------------------------------*/
#include "AEEngine.h"
#include <iostream>

#include "Global.h"
#include "GameStateManager.h"

#include "MainMenu.h"
#include "MenuButtons.h"

#include "GameObjects.h"
#include "Graphics.h"

AEGfxTexture* bgTexture = nullptr;

AEGfxVertexList* pMeshBackground;
AEGfxVertexList* pMesh1;

// class declearation 
Menu_Button menuButtons;
Rendering createMesh;
Rendering drawBackground;

void main_menu::load()
{
    bgTexture = AEGfxTextureLoad("Assets/Background.png");
    menuButtons.load("Assets/buttonTest.png", "Assets/buttonTest.png", "Assets/buttonTest.png", "Assets/buttonTest.png", "Assets/quitTest.png");
    //std::cout << "------------------------- MainMenu::load completed -------------------------" << std::endl << std::endl;
}

void main_menu::init()
{
    // Set the camera position to (0, 0) for the background mesh
    AEGfxSetCamPosition(0.f, 0.f);

    // Create a square mesh for the buttons
    createMesh.SquareMesh(pMesh1);
    menuButtons.init();

    // Create a background mesh
    createMesh.BackgroundMesh(pMeshBackground);

    // debugging logs
    AE_ASSERT_MESG(pMesh1, "Error: Failed to create pMesh1 in MainMenu.cpp!");
    AE_ASSERT_MESG(pMeshBackground, "Error: Failed to create pMeshBackground in MainMenu.cpp!");

    std::cout << std::endl;
    std::cout << "------------------------- MainMenu Initialised -------------------------" << std::endl << std::endl;
}

void main_menu::update()
{
    //std::cout << "GameState: " << current_state << std::endl;

    menuButtons.update();

    if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
        next_state = GS_QUIT;
}

void main_menu::draw()
{
    // Clear the screen
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    // Draw the background mesh
    drawBackground.RenderSprite(bgTexture, 0.f, 0.f, 800.f, 600.f, pMeshBackground);

    // Draw the menu buttons using pMesh1
    menuButtons.draw(pMesh1);
}

void main_menu::free()
{
    AEGfxMeshFree(pMesh1);
}

void main_menu::unload()
{
    menuButtons.unload();
    AEGfxTextureUnload(bgTexture); // unload the texture for the background image
}
