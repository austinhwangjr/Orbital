#include "PauseMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "Graphics.h"
#include "PauseMenuButtons.h"


AEGfxVertexList* pMeshP;

PauseMenuButtons pauseButtons;
Rendering createMeshPauseScreen;            // meshfor pause screen essentially its sq mesh


void pause_menu::load()
{
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
}


void pause_menu::free()
{
    AEGfxMeshFree(pMeshP);
}

void pause_menu::unload()
{
    pauseButtons.unload();

}