#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "StartButton.h"
#include <iostream>

AEGfxTexture* startButtonTex;

//static float texWidth = 100.0f;
//static float texHeight = 50.0f;
//AEMtx33				transform;

void Start_Button::load()
{
    startButtonTex = AEGfxTextureLoad("Assets/start_test.png");
}

void Start_Button::init()
{
    // Empty init function
}

void Start_Button::update()
{
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        s32 mouseX, mouseY;
        AEInputGetCursorPosition(&mouseX, &mouseY);

        float screenX = (mouseX / (float)g_windowWidth) * 2.0f - 1.0f;
        float screenY = (mouseY / (float)g_windowHeight) * -2.0f + 1.0f;

        if (screenX > -0.5f && screenX < 0.5f && screenY > -0.5f && screenY < 0.5f)
        {
            // Start button clicked, go to game state
            if (current_state == GS_MAINMENU) {
                next_state = GS_MAINLEVEL;
                std::cout << "GameState changed to: " << current_state << std::endl;
            }
        }
    }

    if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
        next_state = GS_QUIT;

    //AEMtx33 scale = {};
    AEMtx33Scale(&scale, 50, 50);

    //AEMtx33 rotate = { 0 };
    AEMtx33Rot(&rotate, 0);

    //AEMtx33 translate = { 0 };
    AEMtx33Trans(&translate, 0.f, 0.f);

    //AEMtx33 transform = { 0 };
    AEMtx33Concat(&transform, &rotate, &scale);
    AEMtx33Concat(&transform, &translate, &transform);
}

void Start_Button::draw(AEGfxVertexList* pMesh1)
{
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    AEGfxTextureSet(startButtonTex, 0, 0);

    AEGfxSetTransform(transform.m);

    AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);
}

void Start_Button::free()
{


}

void Start_Button::unload()
{
    AEGfxTextureUnload(startButtonTex);
}
