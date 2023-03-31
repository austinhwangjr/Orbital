/******************************************************************************/
/*!
\file		Highscore.cpp
\author 	Leong Wen Bin, Aceson
\par    	email: leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the highscore 
            feature.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "AEEngine.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include "Input.h"
#include "GameStateList.h"
#include "Graphics.h"
#include "Highscore.h"
#include "Global.h"
#include "Transition.h"
#include "AudioManager.h"

std::vector<int> top_10_highscores;
extern s8 font_id;

AEGfxTexture* texReturnToMMfromHighScore = nullptr;
AEGfxTexture* TEX_HS_BACKGROUND = nullptr;

AEGfxVertexList* pMeshHighScore;
AEGfxVertexList* pMeshHighScore1;

Rendering renderScoreboard;

static float timer4 = 0.f;

static float buttonWidth3 = 200.f;
static float buttonHeight3 = 50.f;

static float returnToMMfromHighScoreX = 0.0f;
static float returnToMMfromHighScoreY = -400.0f;

// Define the positions and dimensions for each button
HighScore::Button3 buttons3[] = {
    {returnToMMfromHighScoreX, returnToMMfromHighScoreY, buttonWidth3, buttonHeight3},   // Return to main menu button
};

void HighScore::load()
{
    texReturnToMMfromHighScore = AEGfxTextureLoad("Assets/MainMenu/HighScore/hs_ExitHighScore.png");
    TEX_HS_BACKGROUND = AEGfxTextureLoad("Assets/MainMenu/HighScore/hs_background.png");

    AudioManager::LoadSound("Assets/BGM/hyperspace_jump.mp3", false);
}

void HighScore::init()
{
    renderScoreboard.SquareMesh(pMeshHighScore);
    renderScoreboard.BackgroundMesh(pMeshHighScore1);
    AE_ASSERT_MESG(pMeshHighScore, "Error: Failed to create pMeshHighScore in HighScore.cpp!");

}

void HighScore::update()
{
    timer4 += AEFrameRateControllerGetFrameTime();

    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked

        if (Input::isButtonClicked(buttons3[0].x, buttons3[0].y, buttons3[0].width, buttons3[0].height))
        {
            AudioManager::PlayOneShot("Assets/BGM/hyperspace_jump.mp3", 1.0f);
            next_state = GS_MAINMENU;
            transition::isTransitionActive = true;
            transition::resetTimer();
            timer4 = 0.f;
        }
    }

    if (AEInputCheckTriggered(AEVK_F11))
    {
        // If the window close button has been clicked, set the game state to quit
        Global_ToggleScreen();
        std::cout << "Toggling Screen " << std::endl;
    }

    FILE* file_highscores;
    errno_t file_check_highscores_read = fopen_s(&file_highscores, "Assets/MainMenu/HighScore/Highscore.txt", "r");
    
    if (file_check_highscores_read != 0) {									// Check if file exist/open	
        std::cout << "Highscore.txt does not exist." << '\n';
        return;
    }
    else
    {
        //add highscores into vector
        char highscore[60];
        while (fgets(highscore, 60, file_highscores))
        {
            int get_highscore = (int)atof(highscore);
            top_10_highscores.push_back(get_highscore);
        }
    }
    fclose(file_highscores);
}

void HighScore::draw() {
    if (timer4 >= 1.f) {
        renderScoreboard.RenderSprite(TEX_HS_BACKGROUND, 0, 0, 800, 450, pMeshHighScore1);

        renderScoreboard.RenderSprite(texReturnToMMfromHighScore, returnToMMfromHighScoreX, returnToMMfromHighScoreY, buttonWidth3, buttonHeight3, pMeshHighScore);

        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetTransparency(1.f);
        AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);


        std::string s = std::to_string(top_10_highscores[0]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.7f, 2.5f, 0.51f, 0.39f, 0.09f);

        s = std::to_string(top_10_highscores[1]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.55f, 2.5f, 0.48f, 0.5f, 0.53f);

        s = std::to_string(top_10_highscores[2]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.40f, 2.5f, 0.62f, 0.35f, 0.13f);

        s = std::to_string(top_10_highscores[3]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.25f, 2.5f, 1.f, 1.f, 1.f);

        s = std::to_string(top_10_highscores[4]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.10f, 2.5f, 1.f, 1.f, 1.f);

        s = std::to_string(top_10_highscores[5]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.05f, 2.5f, 1.f, 1.f, 1.f);

        s = std::to_string(top_10_highscores[6]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.20f, 2.5f, 1.f, 1.f, 1.f);

        s = std::to_string(top_10_highscores[7]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.35f, 2.5f, 1.f, 1.f, 1.f);

        s = std::to_string(top_10_highscores[8]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.50f, 2.5f, 1.f, 1.f, 1.f);

        s = std::to_string(top_10_highscores[9]);
        AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.65f, 2.5f, 1.f, 1.f, 1.f);

    }
}

void HighScore::free()
{
    top_10_highscores.clear();
    AEGfxMeshFree(pMeshHighScore);
    AEGfxMeshFree(pMeshHighScore1);
}


void HighScore::unload()
{
    AEGfxTextureUnload(texReturnToMMfromHighScore);
    AEGfxTextureUnload(TEX_HS_BACKGROUND);
}

void putHighScore(int score)
{
    FILE* file_highscores;
    errno_t file_check_highscores_read = fopen_s(&file_highscores, "Assets/MainMenu/HighScore/Highscore.txt", "r");

    if (file_check_highscores_read != 0) {									// Check if file exist/open	
        std::cout << "Highscore.txt does not exist." << '\n';
        return;
    }
    else {
        //add highscores into vector
        char highscore[60];
        while (fgets(highscore, 60, file_highscores)) {
            int get_highscore = (int)atof(highscore);
            top_10_highscores.push_back(get_highscore);
        }

        for (int i = 0; i < 10; i++)
        {
            if (top_10_highscores[0] == 0) {
                top_10_highscores[0] = score;
                break;
            }
            else {
                if (score > top_10_highscores[i]) {
                    top_10_highscores[9] = score;
                }
            }
        }
        fclose(file_highscores);

        std::sort(top_10_highscores.begin(), top_10_highscores.end(), std::greater<int>());

        const char* filename = "Assets/MainMenu/HighScore/Highscore.txt";
        FILE* file_highscores;
        errno_t file_check_highscores_update = fopen_s(&file_highscores, filename, "w");

        if (file_check_highscores_update != 0) {
            printf("No highscores.txt to update.");
            exit(0);
        }
        else {
            for (int i = 0; i < 10; i++) {
                char string_highscore[60];
                snprintf(string_highscore, 60, "%d\n", (top_10_highscores[i]));
                fputs(string_highscore, file_highscores);
            }
            fclose(file_highscores);
        }
    }
}
