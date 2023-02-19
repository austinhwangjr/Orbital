#pragma once
#include "Graphics.h"

class Menu_Button : public graphics
{
public:
    AEGfxTexture* howToPlayButtonTexture;
    AEGfxTexture* creditsButtonTexture;
    AEGfxTexture* optionsButtonTexture;
    AEGfxTexture* quitButtonTexture;

    void load(const char* filename);
    void init();
    void update();
    void draw(AEGfxVertexList* pMesh1);
    void free();
    void unload();

    float width;
    float height;
    

private:
    AEMtx33 transform;
    AEGfxTexture* buttonTexture;
};
