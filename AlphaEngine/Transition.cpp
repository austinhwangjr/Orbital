/******************************************************************************/
/*!
\file		Transition.cpp
\author		Ong You Yang (100%)
\par		youyang.o\@digipen.edu
\date		March 28, 2023
\brief		This file contains the definitions of functions for the transitions.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/

#include "AEEngine.h"
#include "Transition.h"
#include "Global.h"
#include "Graphics.h"
#include "Easing.h"

namespace transition
{
    AEGfxTexture* transition_space;
    AEGfxVertexList* transition_mesh;

    Rendering transition_createMesh;

    bool isTransitionActive = false;

    float transition_timer;
    float transition_alpha;
    float pause_duration;
}

void transition::load()
{
    transition_space = AEGfxTextureLoad("Assets/Transition/transition_space.png");
}

void transition::init()
{
    transition_createMesh.MainMenuSquareMesh(transition_mesh);
    transition_timer = 0.0f;
    transition_alpha = 0.0f;
    isTransitionActive = false;
}

void transition::update(float deltaTime)
{
    if (pause_duration > 0.0f)
    {
        pause_duration -= deltaTime;
        return;
    }

    transition_timer += deltaTime;

    if (transition_timer < 1.0f)
    {
        // Using EaseOutExpo for fade-in
        transition_alpha = EaseOutExpo(0.0f, 1.0f, transition_timer);
        isTransitionActive = true;
    }
    else if (transition_timer >= 1.0f && transition_timer < 2.0f)
    {
        // Using EaseInQuad for fade-out
        transition_alpha = EaseInQuad(1.0f - (transition_timer - 1.0f));
    }
    else
    {
        transition_alpha = 0.0f;
        isTransitionActive = false;
    }
}
void transition::draw()
{
    if (transition_alpha > 0.0f)
    {
        float r = 1.0f, g = 1.0f, b = 1.0f;
        float centerX = 0.f + g_camPos.x;
        float centerY = 0.f  + g_camPos.y;
        float width = g_windowWidth;
        float height = g_windowHeight;

        Rendering::RenderSpriteWithRotations(transition_space, centerX, centerY, width, height, transition_mesh, 0, r, g, b, transition_alpha);
    }
}

void transition::free()
{
    AEGfxMeshFree(transition_mesh);
}

void transition::unload()
{
    AEGfxTextureUnload(transition_space);
}

void transition::resetTimer()
{
    transition_timer = 0.0f;
    pause_duration = 0.01f;
}

