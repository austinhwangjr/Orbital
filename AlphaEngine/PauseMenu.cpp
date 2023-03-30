/******************************************************************************/
/*!
\file		PauseMenu.cpp
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the pause menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "PauseMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "Graphics.h"
#include "PauseMenuButtons.h"
#include "Highscore.h"

AEGfxVertexList* pMeshP;
extern Player player;
PauseMenuButtons pauseButtons;
Rendering createMeshPauseScreen;            // meshfor pause screen essentially its sq mesh

void pause_menu::load()
{
    pauseButtons.load(  "Assets/MainLevel/PauseMenu/p_ResumeButton.png",
                        "Assets/MainLevel/PauseMenu/p_RestartButton.png",
                        "Assets/MainLevel/PauseMenu/p_ExitMainMenuButton.png",
                        "Assets/MainLevel/PauseMenu/fade.png");
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
    pauseButtons.draw(pMeshP, camPos);
}

void pause_menu::free()
{
    AEGfxMeshFree(pMeshP);
}

void pause_menu::unload()
{

    pauseButtons.unload();

}