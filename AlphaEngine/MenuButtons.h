#pragma once
#include "Graphics.h"

class Menu_Button : public Rendering
{
public:
    void load(  const char* startButtonFilename,
                const char* howToPlayButtonFilename,
                const char* highScoreButtonFilename,
                const char* optionsButtonFilename,
                const char* creditsButtonFilename,
                const char* exitButtonFilename          );
    void init();
    void update();
    void draw(AEGfxVertexList* pMeshMM);
    void free();
    void unload();

    float width;
    float height;
    
private:


    AEMtx33 transform;
    AEGfxTexture* startButtonTexture;
    AEGfxTexture* howToPlayButtonTexture;
    AEGfxTexture* highScoreButtonTexture;
    AEGfxTexture* optionsButtonTexture;
    AEGfxTexture* creditsButtonTexture;
    AEGfxTexture* exitButtonTexture;
};
