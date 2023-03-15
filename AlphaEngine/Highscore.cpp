#include "AEEngine.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include "Input.h"
#include "GameStateManager.h"
#include "Graphics.h"
#include "Highscore.h"
#include "Global.h"

std::vector<int> top_10_highscores;
extern s8 font_id;

AEGfxTexture* texReturnToMMfromHighScore = nullptr;

AEGfxVertexList* pMeshHighScore;

Rendering renderScoreboard;

static float buttonWidth3 = 200.f;
static float buttonHeight3 = 50.f;

static float returnToMMfromHighScoreX = 650.0f;
static float returnToMMfromHighScoreY = -20.0f;

// Define the positions and dimensions for each button
HighScore::Button3 buttons3[] = {
    {returnToMMfromHighScoreX, returnToMMfromHighScoreY, buttonWidth3, buttonHeight3},   // Return to main menu button
};

void HighScore::load() {
    texReturnToMMfromHighScore = AEGfxTextureLoad("Assets/MainMenu/HighScore/hs_ExitHighScore.png");
}

void HighScore::init() {
    
    renderScoreboard.SquareMesh(pMeshHighScore);
    AE_ASSERT_MESG(pMeshHighScore, "Error: Failed to create pMeshHighScore in HighScore.cpp!");

}

void HighScore::update() {

    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked

        if (Input::isButtonClicked(buttons3[0].x, buttons3[0].y, buttons3[0].width, buttons3[0].height))
        {
            next_state = GS_MAINMENU;
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
    else {
        //add highscores into vector
        char highscore[60];
        while (fgets(highscore, 60, file_highscores)) {
            int get_highscore = (int)atof(highscore);
            top_10_highscores.push_back(get_highscore);
        }
    }
    fclose(file_highscores);
     
    //if (AEInputCheckTriggered(AEVK_LBUTTON))
    //{
    //    // Check which button has been clicked
    //    int clickedButton = -1;
    //   
    //    if (Input::isButtonClicked(Bbutton.x, Bbutton.y, Bbutton.width, Bbutton.height)) {
    //        clickedButton = 0;

    //        // Set the next game state based on the button that was clicked
    //        switch (clickedButton)
    //        {
    //            case 0: next_state = GS_MAINLEVEL;  break;
    //        
    //        }
    //    }
    //   
    //}


}

void HighScore::draw() {
    
    renderScoreboard.RenderSprite(texReturnToMMfromHighScore, returnToMMfromHighScoreX, returnToMMfromHighScoreY, buttonWidth3, buttonHeight3, pMeshHighScore);

    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(1.f);
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    
    std::string s = std::to_string(top_10_highscores[0]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.6, 2.5f, 0.51f, 0.39f, 0.09f);

    s = std::to_string(top_10_highscores[1]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.45, 2.5f, 0.48f, 0.5f, 0.53f);

    s = std::to_string(top_10_highscores[2]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.3, 2.5f, 0.62f, 0.35f, 0.13f);

    s = std::to_string(top_10_highscores[3]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.15, 2.5f, 1.f, 1.f, 1.f);

    s = std::to_string(top_10_highscores[4]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0, 2.5f, 1.f, 1.f, 1.f);

    s = std::to_string(top_10_highscores[5]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.15, 2.5f, 1.f, 1.f, 1.f);
    
    s = std::to_string(top_10_highscores[6]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.3, 2.5f, 1.f, 1.f, 1.f);

    s = std::to_string(top_10_highscores[7]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.45, 2.5f, 1.f, 1.f, 1.f);

    s = std::to_string(top_10_highscores[8]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.6, 2.5f, 1.f, 1.f, 1.f);

    s = std::to_string(top_10_highscores[9]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, -0.75, 2.5f, 1.f, 1.f, 1.f);


}


void HighScore::free() {
    top_10_highscores.clear();
    AEGfxMeshFree(pMeshHighScore);
}


void HighScore::unload() {
    AEGfxTextureUnload(texReturnToMMfromHighScore);

}

void putHighScore(int score)
{

    FILE* file_highscores;
    errno_t file_check_highscores_read = fopen_s(&file_highscores, "Assets/Highscore.txt", "r");

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

        const char* filename = "Highscore.txt";
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