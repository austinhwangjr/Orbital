#pragma once
#include "AEEngine.h"
#include "Input.h"
#include "Graphics.h"

class Menu_Button
{
public:
    void load(const char* startButtonFilename,
        const char* startButtonHoverFilename,
        const char* howToPlayButtonFilename,
        const char* howToPlayButtonHoverFilename,
        const char* highScoreButtonFilename,
        const char* highScoreButtonHoverFilename,
        const char* optionsButtonFilename,
        const char* optionsButtonHoverFilename,
        const char* creditsButtonFilename,
        const char* creditsButtonHoverFilename,
        const char* exitButtonFilename,
        const char* exitButtonHoverFilename);

    void init();
    void update();
    void draw(AEGfxVertexList* pMesh);
    void unload();

private:
    AEGfxTexture* normalButtonTextures[6];
    AEGfxTexture* hoverButtonTextures[6];
    bool hoverStates[6];
    Input input;
    Rendering rendering;

};
