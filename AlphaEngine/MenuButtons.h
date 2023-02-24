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
    void draw(AEGfxVertexList* pMeshMM);
    void free();
    void unload();

    float width;
    float height;
    
private:
    //// Set the dimensions of each button
    //static float buttonWidth;
    //static float buttonHeight;

    //// Define the positions for each button
    //static float startX;
    //static float startY;

    //static float howToPlayX;
    //static float howToPlayY;

    //static float creditsX;
    //static float creditsY;

    //static float optionsX;
    //static float optionsY;

    //static float quitX;
    //static float quitY;

    AEMtx33 transform;
    AEGfxTexture* startButtonTexture;
    AEGfxTexture* howToPlayButtonTexture;
    AEGfxTexture* creditsButtonTexture;
    AEGfxTexture* optionsButtonTexture;
    AEGfxTexture* exitButtonTexture;
};
