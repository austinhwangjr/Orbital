/******************************************************************************/
/*!
\file		PauseMenuButtons.h
\author 	Ong You Yang
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the buttons in 
            the pause menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
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