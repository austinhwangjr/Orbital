#include "AEEngine.h"
#include "Transition.h"
#include "Global.h"
#include "Graphics.h"

namespace transition
{
    AEGfxTexture* transition_space;
    AEGfxVertexList* transition_mesh;

    Rendering transition_createMesh;

    bool isTransitionActive = false;

    float transition_timer;
    float transition_alpha;

}


void transition::load()
{
    transition_space = AEGfxTextureLoad("Assets/Transition/transition_space.png");
}

void transition::init()
{
    transition_createMesh.SquareMesh(transition_mesh);
    transition_timer = 0.0f;
    transition_alpha = 0.0f;
    isTransitionActive = false;

}

void transition::update(float deltaTime)
{
    transition_timer += deltaTime;

    if (transition_timer < 1.0f)
    {
        transition_alpha = transition_timer;
        isTransitionActive = true;

    }
    else if (transition_timer > 2.0f && transition_timer < 3.0f)
    {
        transition_alpha = 3.0f - transition_timer;
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
        float centerX = 0.f;
        float centerY = 0.f;
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
}

void transition::activateTransition()
{
    isTransitionActive = true;
    resetTimer();
}