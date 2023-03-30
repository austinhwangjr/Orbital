/******************************************************************************/
/*!
\file		MenuButtons.h
\author 	Ong You Yang
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the menu buttons.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
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
        const char* exitButtonHoverFilename,
        const char* squareTextureFilename);

    void init();
    void update();
    void draw(AEGfxVertexList* pMesh);
    void free();

    void unload();

private:
    AEGfxTexture* squareTexture;
    float squareRotations[6];
    float tint[4];


    AEGfxTexture* normalButtonTextures[6];
    AEGfxTexture* hoverButtonTextures[6];
    bool hoverStates[6];
    Input input;
    Rendering rendering;

};