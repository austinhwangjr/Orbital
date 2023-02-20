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
AEGfxTexture* buttonTexture = nullptr;

// class declearation 
Menu_Button menuButtons;

void main_menu::load()
{
    menuButtons.load("Assets/buttonTest.png");
    std::cout << "--------------------MainMenu::load completed--------------------" << std::endl;

    // Font for text
    //fontID = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 50);
}

void main_menu::init()
{
    menuButtons.init();
   
    // Informing the library that we're about to start adding triangles 
    AEGfxMeshStart();

    // This shape has 2 triangles that makes up a square
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

    // Saving the mesh (list of triangles) in pMesh 
    pMesh1 = AEGfxMeshEnd();

    // debugging logs
    AE_ASSERT_MESG(pMesh1, "Error: Failed to create pMesh1 in MainMenu.cpp!");

    std::cout << "--------------------MainMenu::init completed--------------------" << std::endl;
}

void main_menu::update()
{
    //std::cout << "GameState: " << current_state << std::endl;


    menuButtons.update();

    if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
        next_state = GS_QUIT;

    //std::cout << "--------------------main_menu::update completed--------------------" << std::endl;
    
}

void main_menu::draw()
{
    // Clear the screen
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    // Call the draw function of the menuButtons object
    menuButtons.draw(pMesh1);

    //std::cout << "--------------------main_menu::draw completed--------------------" << std::endl;
}


void main_menu::free()
{
    AEGfxMeshFree(pMesh1);
}

void main_menu::unload()
{
    menuButtons.unload();
}
