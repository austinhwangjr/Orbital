#pragma once
#include "Graphics.h"

class Menu_Button : public Rendering
{
public:
    void load(const char* startButtonFilename,
              const char* howToPlayButtonFilename,
              const char* creditsButtonFilename,
              const char* optionsButtonFilename,
              const char* exitButtonFilename);
    void init();
    void update();
    void draw(AEGfxVertexList* pMesh1);
    void free();
    void unload();

    float width;
    float height;
    
private:
    AEMtx33 transform;
    AEGfxTexture* startButtonTexture;
    AEGfxTexture* howToPlayButtonTexture;
    AEGfxTexture* creditsButtonTexture;
    AEGfxTexture* optionsButtonTexture;
    AEGfxTexture* exitButtonTexture;
};
