#pragma once
#include "Graphics.h"

class PauseMenuButtons : public Rendering
{
public:
    void load(const char* resumeButtonFilename,
        const char* restartButtonFilename,
        const char* exitMainMenuFilename);
    void init();
    void update();
    void draw(AEGfxVertexList* pMeshP);
    void free();
    void unload();

    float width;
    float height;

private:


    AEMtx33 transform;
    AEGfxTexture* resumeTexture;
    AEGfxTexture* restartTexture;
    AEGfxTexture* exitMainMenuTexture;
};