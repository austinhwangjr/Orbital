#pragma once
#include "AEEngine.h"
#include "MainMenu.h"

class Rendering
{
public:
// ----------------------------------------------------------------------------
/*
 * This method draws a button with the given
 * @param texture
 * @param centerX The X coordinate of the center of the button
 * @param centerY The Y coordinate of the center of the button
 * @param width The width of the button
 * @param height The height of the button
 * @param pMesh A pointer to the AEGfxVertexList object to use for the button mesh
 */
 // ----------------------------------------------------------------------------
    static void RenderSprite(AEGfxTexture* texture,
                      float centerX, float centerY, 
                      float width, float height,
                      AEGfxVertexList* pMesh);

    static void RenderFadedBackground(AEGfxTexture* texture, float centerX, float centerY, float width, float height, AEGfxVertexList* pMesh, float alpha);


    static void SquareMesh(AEGfxVertexList*& pMesh);

    static void IGSquareMesh(AEGfxVertexList*& pMesh);

    static void BackgroundMesh(AEGfxVertexList*& pMesh);

    void RenderText(const char* text, float x, float y, int fontSize);
};
