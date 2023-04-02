/******************************************************************************/
/*!
\file		MenuButtons.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the menu buttons.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "pch.h"
#include "gch.h"
#include "Global.h"
#include "Input.h"

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