/*-------------------------------------------------- Main Menu --------------------------------------------------*/
#include "AEEngine.h"
#include <iostream>
#include "Global.h"
#include "GameStateManager.h"
#include "MainMenu.h"
#include "MenuButtons.h"
#include "Graphics.h"
#include "HowToPlay.h"
#include "AudioManager.h"

AEGfxTexture* TexMMBackground = nullptr;
AEGfxTexture* TexTitle = nullptr;

AEGfxVertexList* pMeshMMBackground;
AEGfxVertexList* pMeshMM;

// class declaration 
Menu_Button menuButtons;
Rendering createMesh;
Rendering RenderMMBackground;

void main_menu::load()
{
    TexMMBackground = AEGfxTextureLoad("Assets/MainMenu/mm_Background.png");
    TexTitle = AEGfxTextureLoad("Assets/MainMenu/mm_Title.png");

    menuButtons.load("Assets/MainMenu/mm_StartButton.png",
        "Assets/MainMenu/mm_StartButtonHover.png",
        "Assets/MainMenu/mm_HowToPlayButton.png",
        "Assets/MainMenu/mm_HowToPlayButtonHover.png",
        "Assets/MainMenu/mm_HighScore.png",
        "Assets/MainMenu/mm_HighScoreHover.png",
        "Assets/MainMenu/mm_InProgressButton.png",
        "Assets/MainMenu/mm_InProgressButtonHover.png",
        "Assets/MainMenu/mm_CreditsButton.png",
        "Assets/MainMenu/mm_CreditsButtonHover.png",
        "Assets/MainMenu/mm_ExitButton.png",
        "Assets/MainMenu/mm_ExitButtonHover.png");

    AudioManager::LoadSound("Assets/BGM/cinescifi.wav", true);
}

void main_menu::init()
{
    // Set the camera position to (0, 0) for the background mesh
    AEGfxSetCamPosition(0.f, 0.f);

    // Create a background mesh
    createMesh.BackgroundMesh(pMeshMMBackground);

    // Create a square mesh for the buttons
    createMesh.SquareMesh(pMeshMM);
    menuButtons.init();

    // debugging logs
    AE_ASSERT_MESG(pMeshMM, "Error: Failed to create pMeshMM in MainMenu.cpp!");
    AE_ASSERT_MESG(pMeshMMBackground, "Error: Failed to create pMeshMMBackground in MainMenu.cpp!");

    std::cout << std::endl;
    std::cout << "------------------------- MainMenu Initialised -------------------------" << std::endl << std::endl;
    AudioManager::PlayBGM("Assets/BGM/cinescifi.wav", 0.25f);
}

void main_menu::update()
{
    //std::cout << "GameState: " << current_state << std::endl;

    menuButtons.update();
}

void main_menu::draw()
{
    // Clear the screen
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    // Draw the background mesh
    RenderMMBackground.RenderSprite(TexMMBackground, 0.f, 0.f, 800.f, 450.f, pMeshMMBackground);
    RenderMMBackground.RenderSprite(TexTitle, 0.f, 0.f, 800.f, 450.f, pMeshMMBackground);

    // Draw the menu buttons using pMesh1
    menuButtons.draw(pMeshMM);
}

void main_menu::free()
{
    AEGfxMeshFree(pMeshMMBackground);
    AEGfxMeshFree(pMeshMM);
}

void main_menu::unload()
{
    menuButtons.unload();

    AEGfxTextureUnload(TexTitle);
    AEGfxTextureUnload(TexMMBackground); // unload the texture for the background image
}
