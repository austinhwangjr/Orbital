#pragma once
#include "Graphics.h"

class PauseMenuButtons : public Rendering
{
public:
    void load(const char* resumeButtonFilename,
        const char* restartButtonFilename,
        const char* exitMainMenuFilename,
        const char* fadeMenuFilename);
    void init();
    void update();
    void draw(AEGfxVertexList* pMeshP, const AEVec2& camPos);
    void free();
    void unload();

    float width;
    float height;

private:


    AEMtx33 transform;
    AEGfxTexture* resumeTexture;
    AEGfxTexture* restartTexture;
    AEGfxTexture* exitMainMenuTexture;
    AEGfxTexture* fadeTex;

};