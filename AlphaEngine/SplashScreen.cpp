/******************************************************************************/
/*!
\file		SplashScreen.cpp
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the splash screen.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "SplashScreen.h"
#include "Global.h"
#include "GameStateList.h"
#include "Graphics.h"
#include "Transition.h"
#include "AudioManager.h"

AEGfxTexture* ss_DigiPen_Logo;
AEGfxTexture* ss_TeamNameANDLogo;
AEGfxVertexList* ss_Mesh;

Rendering ss_createMesh;

float timer;

float splashScreenCenterX;
float splashScreenCenterY;

float splashScreenWidth;
float splashScreenHeight;

bool hyperspacePlayed;


void SplashScreen::load()
{
    ss_DigiPen_Logo = AEGfxTextureLoad("Assets/SplashScreen/ss_DigiPenSplashScreen.png");
    ss_TeamNameANDLogo = AEGfxTextureLoad("Assets/SplashScreen/ss_TeamNameANDLogo.png");

    AudioManager::LoadSound("Assets/BGM/hyperspace_jump.mp3", false);
}

void SplashScreen::init()
{
    ss_createMesh.SquareMesh(ss_Mesh);

    timer = 0.0f;

    splashScreenCenterX = 0.f;
    splashScreenCenterY = 0.f;
    hyperspacePlayed = false;
}

void SplashScreen::update()
{
    timer += (float)AEFrameRateControllerGetFrameTime();

    if (timer > 6.0f || AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_RBUTTON))
    {
        AudioManager::PlayOnce("Assets/BGM/hyperspace_jump.mp3", 1.0f);
        transition::isTransitionActive = true;
        transition::resetTimer();
        next_state = GS_MAINMENU;
    }
}

void SplashScreen::draw()
{
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;

    // Fade in and fade out DigiPen_Logo
    if (timer < 3.0f)
    {
        if (timer < 1.0f)
        {
            a = timer / 1.0f;
        }
        else if (timer > 2.0f)
        {
            a = (3.0f - timer) / 1.0f;
        }
        Rendering::RenderSpriteWithRotations(ss_DigiPen_Logo, splashScreenCenterX, splashScreenCenterY, g_windowWidth, g_windowHeight, ss_Mesh, 0, r, g, b, a);
    }
    // Fade in and fade out TeamNameANDLogo
    else if (timer >= 3.0f)
    {
        float localTimer = timer - 3.0f;
        if (localTimer < 1.0f)
        {
            a = localTimer / 1.0f;
        }
        else if (localTimer > 2.0f)
        {
            a = (3.0f - localTimer) / 1.0f;
        }
        Rendering::RenderSpriteWithRotations(ss_TeamNameANDLogo, splashScreenCenterX, splashScreenCenterY, g_windowWidth, g_windowHeight, ss_Mesh, 0, r, g, b, a);
    }
}

void SplashScreen::free()
{
    AEGfxMeshFree(ss_Mesh);
}

void SplashScreen::unload()
{
    AEGfxTextureUnload(ss_DigiPen_Logo);
    AEGfxTextureUnload(ss_TeamNameANDLogo);
}