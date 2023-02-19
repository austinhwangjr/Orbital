#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "StartButton.h"
#include <iostream>
#include "input.h"

// declare a pointer to a texture object for the start button
AEGfxTexture* startButtonTex;   

// declare variables for the transformation matrix
//static float texWidth = 100.0f;
//static float texHeight = 50.0f;
AEMtx33 transform;
AEMtx33 scale = {};
AEMtx33 rotate = {};
AEMtx33 translate = { 0 };

void Start_Button::load()
{
    // load the texture image for the start button
    startButtonTex = AEGfxTextureLoad("Assets/start_test.png");
}

void Start_Button::init()
{
    // empty initialization function
}

void Start_Button::update()
{
    // check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check if the mouse is within the bounds of the start button
        if (IsButtonClicked(0.0f, 0.0f, 100.0f, 50.0f)) {
            // if the start button is clicked, change the game state to main level
            if (current_state == GS_MAINMENU)
            {
                next_state = GS_MAINLEVEL;
                std::cout << "GameState changed to: " << current_state << std::endl;
            }
        }


    }

    // check if the escape key has been pressed or the window has been closed
    if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
        next_state = GS_QUIT;

    // set up the transformation matrix for the start button
    AEMtx33Scale(&scale, 50, 50);                           // scale the button by 50 units in both dimensions
    AEMtx33Rot(&rotate, 0);                                 // set the rotation angle to 0 degrees
    AEMtx33Trans(&translate, 0.f, 0.f);                     // set the translation to (0, 0)
    AEMtx33Concat(&transform, &rotate, &scale);             // concatenate the scale and rotation matrices
    AEMtx33Concat(&transform, &translate, &transform);      // concatenate the translation matrix
}

void Start_Button::draw(AEGfxVertexList* pMesh1)
{
    // set up the rendering pipeline for the start button
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);                  // set the render mode to use textures
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);              // set the tint color to white
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);                     // set the blend mode to alpha blending
    AEGfxSetTransparency(1.0f);                             // set the transparency to fully opaque
    AEGfxTextureSet(startButtonTex, 0, 0);                  // set the texture to use for drawing
    AEGfxSetTransform(transform.m);                         // set the transformation matrix for the mesh
    AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);            // draw the mesh
}

void Start_Button::free()
{
    // empty free function
}

void Start_Button::unload()
{
    // unload the texture for the start button
    AEGfxTextureUnload(startButtonTex);
}
