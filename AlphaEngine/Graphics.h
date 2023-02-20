#pragma once
#include "AEEngine.h"

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
    void RenderSprite(AEGfxTexture* buttonTexture,
                      float centerX,
                      float centerY, 
                      float width,
                      float height,
                      AEGfxVertexList* pMesh);
};
