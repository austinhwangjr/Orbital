/******************************************************************************/
/*!
\file		PauseMenuButtons.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the buttons in 
            the pause menu.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
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