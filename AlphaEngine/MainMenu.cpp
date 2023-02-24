/*-------------------------------------------------- Main Menu --------------------------------------------------*/
#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"

#include "mainmenu.h"
#include "AEEngine.h"
#include <iostream>
#include "MenuButtons.h"
#include "Graphics.h"

AEGfxVertexList* pMesh1;
AEGfxTexture* startButton = nullptr;

// class declearation 
Menu_Button menuButtons;
Rendering createMesh;

void main_menu::load()
{
    menuButtons.load("Assets/buttonTest.png", "Assets/buttonTest.png", "Assets/buttonTest.png", "Assets/buttonTest.png", "Assets/buttonTest.png");
    //std::cout << "------------------------- MainMenu::load completed -------------------------" << std::endl << std::endl;
}

void main_menu::init()
{
    menuButtons.init();
    AEGfxSetCamPosition(0.f, 0.f);
    // Create a square mesh
    createMesh.SquareMesh(pMesh1);

    // debugging logs
    AE_ASSERT_MESG(pMesh1, "Error: Failed to create pMesh1 in MainMenu.cpp!");
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

    // Call the draw function of the menuButtons object
    menuButtons.draw(pMesh1);
}

void main_menu::free()
{
    AEGfxMeshFree(pMesh1);
}

void main_menu::unload()
{
    menuButtons.unload();
}
