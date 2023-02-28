#include "AEEngine.h"

#include "Global.h"
#include "GameStateManager.h"
#include "Graphics.h"

#include "SplashScreen.h"
#include "MainMenu.h"
#include "MenuButtons.h"
#include "Input.h"

extern s8 font1;

// ----------------------------------------------------------------------------
/*
 * Draws a button with a given texture and dimensions at the specified center coordinates.
 *
 * @param buttonTexture A pointer to the AEGfxTexture object to use for the button texture
 * @param centerX The X coordinate of the center of the button
 * @param centerY The Y coordinate of the center of the button
 * @param width The width of the button
 * @param height The height of the button
 * @param pMesh A pointer to the AEGfxVertexList object to use for the button mesh
 */
 // ----------------------------------------------------------------------------
void Rendering::RenderSprite(AEGfxTexture* texture, float centerX, float centerY, float width, float height, AEGfxVertexList* pMesh)
{
    // Set up the transformation matrix for the button
    AEMtx33 scale = {};
    AEMtx33 rotate = {};
    AEMtx33 translate = {};
    AEMtx33 transform = {};

    AEMtx33Scale(&scale, width, height);                    // Scale the button by given dimensions
    AEMtx33Rot(&rotate, 0);                                 // Set the rotation angle to 0 degrees
    AEMtx33Trans(&translate, centerX, centerY);             // Set the translation to (centerX, centerY)
    AEMtx33Concat(&transform, &rotate, &scale);             // Concatenate the scale and rotation matrices
    AEMtx33Concat(&transform, &translate, &transform);      // Concatenate the translation matrix

    // Set up the rendering pipeline for the button
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);                  // Set the render mode to use textures
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);              // Set the tint color to white
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);                     // Set the blend mode to alpha blending
    AEGfxSetTransparency(1.0f);                             // Set the transparency to fully opaque

    AEGfxTextureSet(texture, 0, 0);                   // Set the texture to use for drawing
    AEGfxSetTransform(transform.m);                         // Set the transformation matrix for the mesh
    AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);             // Draw the mesh
}

void Rendering::SquareMesh(AEGfxVertexList*& pMesh)
{
    // Informing the library that we're about to start adding triangles 
    AEGfxMeshStart();

    // This shape has 2 triangles that makes up a square
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    // Saving the mesh (list of triangles) in pMesh 
    pMesh = AEGfxMeshEnd();

    // debugging logs
    AE_ASSERT_MESG(pMesh, "Error: Failed to create pMesh in createSquareMesh!");
}

void Rendering::IGSquareMesh(AEGfxVertexList*& pMesh)
{
    // Informing the library that we're about to start adding triangles 
    AEGfxMeshStart();

    // This shape has 2 triangles that makes up a square
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

    // Saving the mesh (list of triangles) in pMesh 
    pMesh = AEGfxMeshEnd();

    // debugging logs
    AE_ASSERT_MESG(pMesh, "Error: Failed to create pMesh in createSquareMesh!");
}

void Rendering::BackgroundMesh(AEGfxVertexList*& pMesh)
{
    // Informing the library that we're about to start adding triangles 
    AEGfxMeshStart();

    // This shape has 2 triangles that make up the entire screen
    // Color parameters represent colors as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
        -1.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f,
        1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
        -1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
        1.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,
        -1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);

    // Saving the mesh (list of triangles) in pMesh 
    pMesh = AEGfxMeshEnd();

    // debugging logs
    AE_ASSERT_MESG(pMesh, "Error: Failed to create pMesh in createBackgroundMesh!");
}

//void Rendering::RenderText(const char* text, float x, float y, int fontSize)
//{
//    // Calculate the screen position of the text based on the button position and size
//    float screenX = x + 200.f / 2 - (float)strlen(text) * (float)fontSize / 2;
//    float screenY = y + 50.f / 2 - fontSize / 2;
//
//    // Calculate the scale factor for the text
//    float scale = fontSize / 12.0f;
//
//    // Set the render mode to solid and set the font color
//    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
//    AEGfxSetTransparency(1.0f);
//    //AEGfxSetTransform(nullptr);
//    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
//
//    // Render the text using AEGfxPrint
//    AEGfxPrint(font1, (s8*)text, screenX / scale, screenY / scale, scale, 1.0f, 1.0f, 1.0f);
//}




