/******************************************************************************/
/*!
\file		Graphics.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the Rendering 
            class.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "Global.h"
#include "Graphics.h"

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

    AEGfxTextureSet(texture, 0, 0);                         // Set the texture to use for drawing
    AEGfxSetTransform(transform.m);                         // Set the transformation matrix for the mesh
    AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);             // Draw the mesh
}

void Rendering::RenderSpriteWithRotations(AEGfxTexture* texture, float centerX, float centerY, float width, float height, AEGfxVertexList* pMesh, float rotation, float r, float g, float b, float a)
{
    // Set up the transformation matrix for the button
    AEMtx33 scale = {};
    AEMtx33 rotate = {};
    AEMtx33 translate = {};
    AEMtx33 transform = {};

    AEMtx33Scale(&scale, width, height);                    // Scale the button by given dimensions
    AEMtx33Rot(&rotate, rotation);                          // Set the rotation angle to the specified value
    AEMtx33Trans(&translate, centerX, centerY);             // Set the translation to (centerX, centerY)
    AEMtx33Concat(&transform, &rotate, &scale);             // Concatenate the scale and rotation matrices
    AEMtx33Concat(&transform, &translate, &transform);      // Concatenate the translation matrix

    // Set up the rendering pipeline for the button
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);                  // Set the render mode to use textures
    AEGfxSetTintColor(r, g, b, a);                          // Set the tint color
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);                     // Set the blend mode to alpha blending
    AEGfxSetTransparency(1.0f);                             // Set the transparency to fully opaque

    AEGfxTextureSet(texture, 0, 0);                         // Set the texture to use for drawing
    AEGfxSetTransform(transform.m);                         // Set the transformation matrix for the mesh
    AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);             // Draw the mesh
}



void Rendering::RenderFadedBackground(AEGfxTexture* texture, float centerX, float centerY, float width, float height, AEGfxVertexList* pMesh, float alpha)
{
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, alpha); // Set the tint color to white with the specified alpha value
    RenderSprite(texture, centerX, centerY, width, height, pMesh);
}


void Rendering::MainMenuSquareMesh(AEGfxVertexList*& pMesh)
{
    // Informing the library that we're about to start adding triangles 
    AEGfxMeshStart();

    // This shape has 2 triangles that makes up a square
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
         0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
        -0.5f,  0.5f, 0xFF00FFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
         0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
         0.5f,  0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    // Saving the mesh (list of triangles) in pMesh 
    pMesh = AEGfxMeshEnd();

    // debugging logs
    AE_ASSERT_MESG(pMesh, "Error: Failed to create pMesh in MainMenuSquareMesh!");
}

void Rendering::GameLevelSquareMesh(AEGfxVertexList*& pMesh)
{
    // Informing the library that we're about to start adding triangles 
    AEGfxMeshStart();

    // This shape has 2 triangles that makes up a square
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFA500, 1.0f, 0.0f,
        0.5f, -0.5f, 0xFFFFA500, 0.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFA500, 1.0f, 1.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFA500, 0.0f, 0.0f,
        0.5f, 0.5f, 0xFFFFA500, 0.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFA500, 1.0f, 1.0f);

    // Saving the mesh (list of triangles) in pMesh 
    pMesh = AEGfxMeshEnd();

    // debugging logs
    AE_ASSERT_MESG(pMesh, "Error: Failed to create pMesh in GameLevelSquareMesh!");
}

void Rendering::ParticleMesh(AEGfxVertexList*& pMesh)
{
    // Informing the library that we're about to start adding triangles 
    AEGfxMeshStart();

    // This shape has 2 triangles that makes up a square
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFF454BF7, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFF454BF7, 0.0f, 0.0f,
        -0.5f, 0.5f, 0xFF454BF7, 0.0f, 0.0f);

    AEGfxTriAdd(
        -0.5f, 0.5f, 0xFF454BF7, 0.0f, 0.0f,
        0.5f, -0.5f, 0xFF454BF7, 0.0f, 0.0f,
        0.5f, 0.5f, 0xFF454BF7, 0.0f, 0.0f);

    // Saving the mesh (list of triangles) in pMesh 
    pMesh = AEGfxMeshEnd();

    // debugging logs
    AE_ASSERT_MESG(pMesh, "Error: Failed to create pMesh in ParticleMesh!");
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
        -1.0f,  1.0f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
         1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
         1.0f, 1.0f,  0xFFFFFFFF, 1.0f, 0.0f,
        -1.0f, 1.0f,  0xFFFFFFFF, 0.0f, 0.0f);

    // Saving the mesh (list of triangles) in pMesh 
    pMesh = AEGfxMeshEnd();

    // debugging logs
    AE_ASSERT_MESG(pMesh, "Error: Failed to create pMesh in createBackgroundMesh!");
}
