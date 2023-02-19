#include "AEEngine.h"
#include "GameObjects.h"

#include "Global.h"
#include "GameStateManager.h"
#include "Graphics.h"

#include "SplashScreen.h"
#include "MainMenu.h"
#include "MenuButtons.h"
#include "Input.h"

void graphics::DrawButton(AEGfxTexture* buttonTexture, float centerX, float centerY, float width, float height, AEGfxVertexList* pMesh)
{
    // Set up the transformation matrix for the button
    AEMtx33 scale = {};
    AEMtx33 rotate = {};
    AEMtx33 translate = {};
    AEMtx33 transform = {};

    AEMtx33Scale(&scale, width, height);                         // Scale the button by given dimensions
    AEMtx33Rot(&rotate, 0);                                     // Set the rotation angle to 0 degrees
    AEMtx33Trans(&translate, centerX, centerY);                 // Set the translation to (centerX, centerY)
    AEMtx33Concat(&transform, &rotate, &scale);                 // Concatenate the scale and rotation matrices
    AEMtx33Concat(&transform, &translate, &transform);          // Concatenate the translation matrix

    // Set up the rendering pipeline for the button
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);                      // Set the render mode to use textures
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);                  // Set the tint color to white
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);                         // Set the blend mode to alpha blending
    AEGfxSetTransparency(1.0f);                                 // Set the transparency to fully opaque

    AEGfxTextureSet(buttonTexture, 0, 0);                       // Set the texture to use for drawing
    AEGfxSetTransform(transform.m);                             // Set the transformation matrix for the mesh
    AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);                 // Draw the mesh
}
