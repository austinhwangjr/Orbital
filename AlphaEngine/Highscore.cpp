#include "Highscore.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include "Input.h"
#include "GameStateManager.h"


std::vector<int> top_10_highscores;
extern s8 font_id;



void HighScore::load() {
    
}

void HighScore::init() {

}

void HighScore::update() {
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
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(1.f);
    AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    
    std::string s = std::to_string(top_10_highscores[0]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.8, 0.5f, 1.f, 0.41f, 0.70f);

    s = std::to_string(top_10_highscores[1]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.75, 0.5f, 1.f, 0.41f, 0.70f);

    s = std::to_string(top_10_highscores[2]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.7, 0.5f, 1.f, 0.41f, 0.70f);

    s = std::to_string(top_10_highscores[3]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.65, 0.5f, 1.f, 0.41f, 0.70f);

    s = std::to_string(top_10_highscores[4]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.6, 0.5f, 1.f, 0.41f, 0.70f);

    s = std::to_string(top_10_highscores[5]);
    AEGfxPrint(font_id, const_cast<s8*>(s.c_str()), -0.3f, 0.55, 0.5f, 1.f, 0.41f, 0.70f);
    
    


}


void HighScore::free() {
    top_10_highscores.clear();
}


void HighScore::unload() {
   
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