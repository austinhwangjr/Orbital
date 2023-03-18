/*-------------------------------------------------- Main Menu --------------------------------------------------*/
#include "AEEngine.h"
#include <iostream>
#include "Global.h"
#include "GameStateManager.h"
#include "MainMenu.h"
#include "MenuButtons.h"
#include "Graphics.h"
#include "HowToPlay.h"
#include "AudioManager.h"
#include "Player.h"
#include "Planet.h"

AEGfxTexture* TexMMBackground = nullptr;
AEGfxTexture* TexTitle = nullptr;

int flag=0;
AEGfxVertexList* pMeshMMBackground;
AEGfxVertexList* pMeshMM;
AEGfxVertexList* pMeshObj;

Player MMplayer;
Planets MMplanet;

AEGfxTexture* MMtexplayer;
AEGfxTexture* MMtexplanet;
AEGfxTexture* MMorbit_tex;

f64 MMframe_time{}, MMtotal_time{};

// class declaration 
Menu_Button menuButtons;
Rendering createMesh;
Rendering RenderMMBackground;

void main_menu::load()
{
    TexMMBackground = AEGfxTextureLoad("Assets/MainMenu/mm_Background.png");
    TexTitle =  AEGfxTextureLoad("Assets/MainMenu/mm_Title.png");

    menuButtons.load("Assets/MainMenu/mm_StartButton.png",
        "Assets/MainMenu/mm_StartButtonHover.png",
        "Assets/MainMenu/mm_HowToPlayButton.png",
        "Assets/MainMenu/mm_HowToPlayButtonHover.png",
        "Assets/MainMenu/mm_HighScore.png",
        "Assets/MainMenu/mm_HighScoreHover.png",
        "Assets/MainMenu/mm_InProgressButton.png",
        "Assets/MainMenu/mm_InProgressButtonHover.png",
        "Assets/MainMenu/mm_CreditsButton.png",
        "Assets/MainMenu/mm_CreditsButtonHover.png",
        "Assets/MainMenu/mm_ExitButton.png",
        "Assets/MainMenu/mm_ExitButtonHover.png");
    
    AudioManager::LoadSound("Assets/BGM/cinescifi.wav", true);

    MMtexplayer = AEGfxTextureLoad("Assets/MainLevel/ml_Spaceship.png");
    MMtexplanet = AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture.png");
    MMorbit_tex = AEGfxTextureLoad("Assets/MainLevel/ml_OrbitRing.png");
}

void main_menu::init()
{
    // Set the camera position to (0, 0) for the background mesh
    AEGfxSetCamPosition(0.f, 0.f);

    // Create a background mesh
    createMesh.BackgroundMesh(pMeshMMBackground);

    // Create a square mesh for the buttons
    createMesh.SquareMesh(pMeshMM);
    createMesh.IGSquareMesh(pMeshObj);
    menuButtons.init();

    // debugging logs
    AE_ASSERT_MESG(pMeshMM, "Error: Failed to create pMeshMM in MainMenu.cpp!");
    AE_ASSERT_MESG(pMeshMMBackground, "Error: Failed to create pMeshMMBackground in MainMenu.cpp!");

    std::cout << std::endl;
    std::cout << "------------------------- MainMenu Initialised -------------------------" << std::endl << std::endl;
    AudioManager::PlayBGM("Assets/BGM/cinescifi.wav", 0.25f);
    MMplayer.init();
}

void main_menu::update()
{
    //std::cout << "GameState: " << current_state << std::endl;

    MMframe_time = AEFrameRateControllerGetFrameTime();
    MMtotal_time += MMframe_time;


    // Player is in free-flying mode
    if (MMplayer.state == PLAYER_FLY) {
        flag = 0;
        if (AEInputCheckCurr(AEVK_W)) {
            AEVec2 added;
            AEVec2Set(&added, AECos(MMplayer.direction), AESin(MMplayer.direction));

            // Find the velocity according to the acceleration
            AEVec2Scale(&added, &added, MMplayer.mov_speed * static_cast<f32>(MMplayer.mov_speed_level + 1) / 2.f);
            MMplayer.velocity.x = MMplayer.velocity.x + added.x * static_cast<f32>(MMframe_time);
            MMplayer.velocity.y = MMplayer.velocity.y + added.y * static_cast<f32>(MMframe_time);

            // Limit player's speed
            AEVec2Scale(&MMplayer.velocity, &MMplayer.velocity, 0.99f);
        }

        if (AEInputCheckCurr(AEVK_S)) {
            AEVec2 added;
            AEVec2Set(&added, -AECos(MMplayer.direction), -AESin(MMplayer.direction));

            // Find the velocity according to the decceleration
            AEVec2Scale(&added, &added, MMplayer.mov_speed * static_cast<f32>(MMplayer.mov_speed_level + 1) / 2.f);
            MMplayer.velocity.x = MMplayer.velocity.x + added.x * static_cast<f32>(MMframe_time);
            MMplayer.velocity.y = MMplayer.velocity.y + added.y * static_cast<f32>(MMframe_time);

            // Limit player's speed
            AEVec2Scale(&MMplayer.velocity, &MMplayer.velocity, 0.99f);
        }

        if (AEInputCheckCurr(AEVK_A)) {
            MMplayer.direction += MMplayer.rot_speed * static_cast<f32>(MMframe_time);
            MMplayer.direction = AEWrap(MMplayer.direction, -PI, PI);
        }

        if (AEInputCheckCurr(AEVK_D)) {
            MMplayer.direction -= MMplayer.rot_speed * static_cast<f32>(MMframe_time);
            MMplayer.direction = AEWrap(MMplayer.direction, -PI, PI);
        }

        if (AEVec2Distance(&MMplanet.position, &MMplayer.position) <= (MMplanet.size / 2 + MMplanet.orbit_range)) {
            MMplayer.direction = static_cast<f32>(atan2(MMplayer.position.y - MMplanet.position.y, MMplayer.position.x - MMplanet.position.x));
            MMplayer.state = PLAYER_ORBIT;
        }

        // =======================
        // Update player position
        // =======================

        MMplayer.position.x = MMplayer.position.x + MMplayer.velocity.x * static_cast<f32>(MMframe_time);
        MMplayer.position.y = MMplayer.position.y + MMplayer.velocity.y * static_cast<f32>(MMframe_time);

    }
    if (MMplayer.state == PLAYER_ORBIT) {
        // ================
        // Check for input
        // ================
        if (AEInputCheckCurr(AEVK_A) ) {
            MMplayer.direction += (MMplayer.rot_speed / 2) * static_cast<f32>(MMframe_time);

            MMplayer.position.x = MMplanet.position.x + (static_cast<f32>(MMplanet.size) / 2 + MMplanet.orbit_range) * AECos(MMplayer.direction);
            MMplayer.position.y = MMplanet.position.y + (static_cast<f32>(MMplanet.size) / 2 + MMplanet.orbit_range) * AESin(MMplayer.direction);
        }

        if (AEInputCheckCurr(AEVK_D) ) {
            MMplayer.direction -= (MMplayer.rot_speed / 2) * static_cast<f32>(MMframe_time);

            MMplayer.position.x = MMplanet.position.x + (static_cast<f32>(MMplanet.size) / 2 + MMplanet.orbit_range) * AECos(MMplayer.direction);
            MMplayer.position.y = MMplanet.position.y + (static_cast<f32>(MMplanet.size) / 2 + MMplanet.orbit_range) * AESin(MMplayer.direction);
        }

        if (AEInputCheckPrev(AEVK_W))
            MMplayer.can_leave_orbit = false;
        else
            MMplayer.can_leave_orbit = true;

        if (AEInputCheckCurr(AEVK_W) && MMplayer.can_leave_orbit) {
            AEVec2Zero(&MMplayer.velocity);
            MMplayer.state = PLAYER_FLY;
        }

        MMplayer.position.x=AEWrap(MMplayer.position.x, AEGfxGetWinMinX() , AEGfxGetWinMaxX());
        MMplayer.position.y=AEWrap(MMplayer.position.y, AEGfxGetWinMinY() , AEGfxGetWinMaxY() );
    }


    MMplanet.position.x =  -AEGetWindowWidth() / 2;
    MMplanet.position.y = - AEGetWindowHeight() / 2;
    MMplanet.size = 1200;
   
    MMplanet.direction += PLANET_ROT_SPEED * static_cast<f32>(MMframe_time);

    AEMtx33Scale(&MMplanet.scale, MMplanet.size, MMplanet.size);
    
    AEMtx33Rot(&MMplanet.rotate, MMplanet.direction);
    AEMtx33Trans(&MMplanet.translate, MMplanet.position.x, MMplanet.position.y);
    AEMtx33Concat(&MMplanet.transform, &MMplanet.rotate, &MMplanet.scale);

    AEMtx33Concat(&MMplanet.transform, &MMplanet.translate, &MMplanet.transform);

    // Update the transform matrix with new direction
    AEVec2 added;
    AEVec2Set(&added, AECos(MMplanet.direction), AESin(MMplanet.direction));
    AEMtx33Rot(&MMplanet.rotate, MMplanet.direction);
    AEMtx33Concat(&MMplanet.transform, &MMplanet.rotate, &MMplanet.scale);
    AEMtx33Concat(&MMplanet.transform, &MMplanet.translate, &MMplanet.transform);

    // Update orbit range transform matrix
    AEMtx33Scale(&MMplanet.orbit_scale, (MMplanet.size + MMplanet.orbit_range * 2), (MMplanet.size + MMplanet.orbit_range * 2));
    AEMtx33Rot(&MMplanet.orbit_rotate, 0.f);
    AEMtx33Trans(&MMplanet.orbit_translate, MMplanet.position.x, MMplanet.position.y);
    AEMtx33Concat(&MMplanet.orbit_transform, &MMplanet.orbit_rotate, &MMplanet.orbit_scale);
    AEMtx33Concat(&MMplanet.orbit_transform, &MMplanet.orbit_translate, &MMplanet.orbit_transform);




    // =========================================
    // Calculate the matrix for player and beam
    // =========================================

    AEMtx33 scale, rot, trans;

    // Player
    AEMtx33Scale(&scale, MMplayer.size, MMplayer.size);
    AEMtx33Rot(&rot, MMplayer.direction + PI / 2);
    AEMtx33Trans(&trans, MMplayer.position.x, MMplayer.position.y);
    AEMtx33Concat(&MMplayer.player_transform, &rot, &scale);
    AEMtx33Concat(&MMplayer.player_transform, &trans, &MMplayer.player_transform);

   
    menuButtons.update();
}

void main_menu::draw()
{
    // Clear the screen
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    // Draw the background mesh
    RenderMMBackground.RenderSprite(TexMMBackground, 0.f, 0.f, 800.f, 450.f, pMeshMMBackground);
    RenderMMBackground.RenderSprite(TexTitle, 0.f, 0.f, 800.f, 450.f, pMeshMMBackground);

    // Draw orbit ring first
    AEGfxTextureSet(MMorbit_tex, 0, 0);
    AEGfxSetTransform(MMplanet.orbit_transform.m);
    AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);

    // Draw planet sprite
    AEGfxTextureSet(MMtexplanet, 0, 0);
    AEGfxSetTransform(MMplanet.transform.m);
    AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);


    AEGfxTextureSet(MMtexplayer, 0, 0);
    AEGfxSetTransform(MMplayer.player_transform.m);
    AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);

    // Draw the menu buttons using pMesh1
    menuButtons.draw(pMeshMM);
}

void main_menu::free()
{
    AEGfxMeshFree(pMeshMMBackground);
    AEGfxMeshFree(pMeshMM);
    AEGfxMeshFree(pMeshObj);
}

void main_menu::unload()
{
    menuButtons.unload();
    AEGfxTextureUnload(MMtexplayer);
    AEGfxTextureUnload(MMtexplanet);
    AEGfxTextureUnload(MMorbit_tex);
    AEGfxTextureUnload(TexTitle);
    AEGfxTextureUnload(TexMMBackground); // unload the texture for the background image
}
