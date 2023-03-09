#include "PauseMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "Graphics.h"
#include "PauseMenuButtons.h"

//AEGfxTexture* TexPauseBackground = nullptr;

//AEGfxVertexList* pMeshPauseBackground;
AEGfxVertexList* pMeshP;

//PauseMenuButtons pauseButtons;
PauseMenuButtons pauseButtons;
Rendering createMeshPauseScreen;            // meshfor pause screen essentially its sq mesh
//Rendering RenderPMBackground;               // render the tranlucent background for pause mnenu


void pause_menu::load()
{
    /*TexPauseBackground = AEGfxTextureLoad("Assets/Background.png");*/

    pauseButtons.load("Assets/p_ResumeButton.png", "Assets/p_RestartButton.png", "Assets/p_ExitMainMenuButton.png");
}

void pause_menu::init()
{
    
    createMeshPauseScreen.SquareMesh(pMeshP);

    pauseButtons.init();

    AE_ASSERT_MESG(pMeshP, "Error: Failed to create pMeshP in PauseMenu.cpp!");

}

void pause_menu::update()
{
    pauseButtons.update();
}

void pause_menu::draw(AEVec2 const& camPos)
{
    AEGfxSetCamPosition(camPos.x, camPos.y);
    pauseButtons.draw(pMeshP);

    AEGfxSetBackgroundColor(0.5f, 0.0f, 0.0f);
    //AEGfxSetTransparency(1.0f);


    //if (TexPauseBackground && pMeshP)
    //{
    //    RenderPMBackground.RenderSprite(TexPauseBackground, 0.f, 0.f, 800.f, 450.f, pMeshP);
    //}
}


void pause_menu::free()
{
    AEGfxMeshFree(pMeshP);
}

void pause_menu::unload()
{
    pauseButtons.unload();

    /*AEGfxTextureUnload(TexPauseBackground);*/
}