#include "PauseMenu.h"
#include "Global.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "Graphics.h"
#include "PauseMenuButtons.h"

AEGfxTexture* TexPauseBackground = nullptr;

//AEGfxVertexList* pMeshPauseBackground;
AEGfxVertexList* pMeshP;

//PauseMenuButtons pauseButtons;
Rendering createMeshPauseScreen;
Rendering RenderPMBackground;


void pause_menu::load()
{
    TexPauseBackground = AEGfxTextureLoad("Assets/quitButton.png");

    //pauseButtons.load("Assets/p_ResumeButton.png", "Assets/p_Restart.png", "Assets/p_ExitMainMenu.png");


}

void pause_menu::init()
{
    createMeshPauseScreen.SquareMesh(pMeshP);
    //pauseButtons.init();

    AE_ASSERT_MESG(pMeshP, "Error: Failed to create pMeshP in PauseMenu.cpp!");

}

void pause_menu::update()
{
    if (AEInputCheckTriggered(AEVK_P))
    {
        next_state = GS_MAINLEVEL;
    }

}

void pause_menu::draw()
{
    if (TexPauseBackground && pMeshP)
    {
        RenderPMBackground.RenderSprite(TexPauseBackground, 0.f, 0.f, 800.f, 450.f, pMeshP);
    }
}


void pause_menu::free()
{


    AEGfxMeshFree(pMeshP);
}

void pause_menu::unload()
{

    AEGfxTextureUnload(TexPauseBackground);
}


