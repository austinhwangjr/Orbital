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
#include "Debris.h"
#include "Shuttle.h"
#include "Easing.h"

#define MM_SHUTTLE_SIZE 20
AEGfxTexture* TexMMBackground = nullptr;
AEGfxTexture* TexTitle = nullptr;

static bool isOrbitingPlanet = false; // flag for is orbiting
// Variables
float MManimationDuration = 1.0f; // Duration of the animation in seconds
bool MMbeam_active = false;
AEGfxVertexList* pMeshMMBackground;
AEGfxVertexList* pMeshMM;
AEGfxVertexList* pMeshObj;

Player MMplayer;
Planets MMplanet;
Debris MMdebris;
std::vector<Shuttles> MMshuttle_vector;

AEGfxTexture* MMtexplayer;
AEGfxTexture* MMtexplanet;
AEGfxTexture* MMorbit_tex;
AEGfxTexture* MMtexdebris;
AEGfxTexture* MMtexbeam;
AEGfxTexture* MMshuttle_tex;
AEGfxTexture* MMexplosion_tex;
AEGfxTexture* MMorbit_halo_tex;

f64 MMframe_time = 0.f;
f64 MMtotal_time = 0.f;

// class declaration 
Menu_Button menuButtons;
Rendering createMesh;
Rendering RenderMMBackground;


void main_menu::load()
{
    TexMMBackground = AEGfxTextureLoad("Assets/MainMenu/mm_Background.png");
    TexTitle =  AEGfxTextureLoad("Assets/MainMenu/mm_Title.png");

    menuButtons.load(   "Assets/MainMenu/mm_StartButton.png",
                        "Assets/MainMenu/mm_HowToPlayButton.png",
                        "Assets/MainMenu/mm_HighScore.png",
                        "Assets/MainMenu/mm_OptionsButton.png",
                        "Assets/MainMenu/mm_CreditsButton.png",
                        "Assets/MainMenu/mm_ExitButton.png",


                        "Assets/MainMenu/mm_StartButtonHover.png",
                        "Assets/MainMenu/mm_HowToPlayButtonHover.png",
                        "Assets/MainMenu/mm_HighScoreHover.png",
                        "Assets/MainMenu/mm_OptionsButtonHover.png",
                        "Assets/MainMenu/mm_CreditsButtonHover.png",
                        "Assets/MainMenu/mm_ExitButtonHover.png",
                        "Assets/MainMenu/squareTexture.png");


    MMtexplayer = AEGfxTextureLoad("Assets/MainLevel/ml_Spaceship2.png");
    MMtexplanet = AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture4.png");
    MMorbit_tex = AEGfxTextureLoad("Assets/MainLevel/ml_OrbitRing.png");
    MMtexdebris = AEGfxTextureLoad("Assets/MainLevel/ml_Debris.png");
    MMtexbeam = AEGfxTextureLoad("Assets/MainLevel/ml_TractorBeam.png");
    MMshuttle_tex = AEGfxTextureLoad("Assets/MainLevel/ml_Shuttle.png");
    MMexplosion_tex = AEGfxTextureLoad("Assets/MainLevel/ml_Explosion.png");
    MMorbit_halo_tex = AEGfxTextureLoad("Assets/MainLevel/neonCircle.png");


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

    MMplayer.init();

    MMplayer.position.x = 0.0f;
    MMplayer.position.y = 0.0f;

    //DEBRIS
    MMplanet.max_debris = rand() % (DEBRIS_MAX - DEBRIS_MIN) + DEBRIS_MIN;												// Randomize debris count on planet spawn
    MMplanet.debris_vector = MM_create_debris(MMplanet.position.x, MMplanet.position.y, MMplanet.size, MMplanet.max_debris);

    MMplanet.position.x = static_cast<f32>(AEGetWindowWidth() / 2);
    MMplanet.position.y = static_cast<f32>( - AEGetWindowHeight() / 2);
    MMplanet.size = 1200;
    MMplanet.shuttle_timer = 0.0;																													// Zero out shuttle timer on spawn
    MMplanet.shuttle_time_to_spawn = static_cast<f32>(rand() % (SHUTTLE_SPAWN_TIME_MAX - SHUTTLE_SPAWN_TIME_MIN + 1) + SHUTTLE_SPAWN_TIME_MIN);	// Randomize value for timer to reach before spawning
    
}

void main_menu::update()
{
    MMframe_time = AEFrameRateControllerGetFrameTime();
    MMtotal_time += MMframe_time;

    // ================
    //  MENU BUTTONS
    // ================
    menuButtons.update();

    // =======================
    //  PLAYER MOVEMENT
    // =======================

    // =======================
    //  FREE FLYING MODE
    // =======================
    if (MMplayer.state == PLAYER_FLY)
    {
        if (AEInputCheckCurr(AEVK_W))
        {
            AEVec2 added;
            AEVec2Set(&added, AECos(MMplayer.direction), AESin(MMplayer.direction));

            // Find the velocity according to the acceleration
            AEVec2Scale(&added, &added, MMplayer.mov_speed * static_cast<f32>(MMplayer.mov_speed_level + 3) / 2.f);
            MMplayer.velocity.x = MMplayer.velocity.x + added.x * static_cast<f32>(MMframe_time);
            MMplayer.velocity.y = MMplayer.velocity.y + added.y * static_cast<f32>(MMframe_time);

            // Limit player's speed
            AEVec2Scale(&MMplayer.velocity, &MMplayer.velocity, 0.99f);
        }

        if (AEInputCheckCurr(AEVK_S))
        {
            AEVec2 added;
            AEVec2Set(&added, -AECos(MMplayer.direction), -AESin(MMplayer.direction));

            // Find the velocity according to the decceleration
            AEVec2Scale(&added, &added, MMplayer.mov_speed * static_cast<f32>(MMplayer.mov_speed_level + 1) / 2.f);
            MMplayer.velocity.x = MMplayer.velocity.x + added.x * static_cast<f32>(MMframe_time);
            MMplayer.velocity.y = MMplayer.velocity.y + added.y * static_cast<f32>(MMframe_time);

            // Limit player's speed
            AEVec2Scale(&MMplayer.velocity, &MMplayer.velocity, 0.99f);
        }

        if (AEInputCheckCurr(AEVK_A))
        {
            MMplayer.direction += MMplayer.rot_speed * static_cast<f32>(MMframe_time);
            MMplayer.direction = AEWrap(MMplayer.direction, -PI, PI);
        }

        if (AEInputCheckCurr(AEVK_D))
        {
            MMplayer.direction -= MMplayer.rot_speed * static_cast<f32>(MMframe_time);
            MMplayer.direction = AEWrap(MMplayer.direction, -PI, PI);
        }

        if (AEVec2Distance(&MMplanet.position, &MMplayer.position) <= (MMplanet.size / 2 + MMplanet.orbit_range))
        {
            MMplayer.direction = static_cast<f32>(atan2(MMplayer.position.y - MMplanet.position.y, MMplayer.position.x - MMplanet.position.x));
            MMplayer.state = PLAYER_ORBIT;
        }

        // =======================
        // Update Player Position
        // =======================

        MMplayer.position.x = MMplayer.position.x + MMplayer.velocity.x * static_cast<f32>(MMframe_time);
        MMplayer.position.y = MMplayer.position.y + MMplayer.velocity.y * static_cast<f32>(MMframe_time);
    }

    // =======================
    //  ORBIT MODE
    // =======================
    if (MMplayer.state == PLAYER_ORBIT)
    {
        // ================
        // Check for Input
        // ================
        if (AEInputCheckCurr(AEVK_A) && MMplayer.position.y >= AEGfxGetWinMinY() + MMplayer.size)
        {
            MMplayer.direction += (MMplayer.rot_speed / 2) * static_cast<f32>(MMframe_time);

            MMplayer.position.x = MMplanet.position.x + (static_cast<f32>(MMplanet.size) / 2 + MMplanet.orbit_range) * AECos(MMplayer.direction);
            MMplayer.position.y = MMplanet.position.y + (static_cast<f32>(MMplanet.size) / 2 + MMplanet.orbit_range) * AESin(MMplayer.direction);
        }

        if (AEInputCheckCurr(AEVK_D) && MMplayer.position.x <= AEGfxGetWinMaxX() - MMplayer.size)
        {
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


        // Draw tractor beam
        if (AEInputCheckCurr(AEVK_SPACE))
            MMbeam_active = true;
        else
            MMbeam_active = false;

    // ================================
    // Update player and beam position
    // ================================

        if (AEInputCheckCurr(AEVK_W) && MMplayer.can_leave_orbit) {
            MMplayer.position.x += AECos(MMplayer.direction);
            MMplayer.position.y += AESin(MMplayer.direction);
        }
        else {
            MMplayer.position.x = MMplanet.position.x + (static_cast<f32>(MMplanet.size) / 2.f + MMplanet.orbit_range) * AECos(MMplayer.direction);
            MMplayer.position.y = MMplanet.position.y + (static_cast<f32>(MMplanet.size) / 2.f + MMplanet.orbit_range) * AESin(MMplayer.direction);
        }

        if (MMbeam_active) {
            MMplayer.beam_pos.x = MMplayer.position.x - AECos(MMplayer.direction) * ((MMplayer.beam_height + MMplayer.size) / 2);
            MMplayer.beam_pos.y = MMplayer.position.y - AESin(MMplayer.direction) * ((MMplayer.beam_height + MMplayer.size) / 2);
        }

        // ================================
        // Check for beam-debris collision
        // ================================

        if (MMbeam_active) {
            // Check for collision between tractor beam and debris
            for (int i = 0; i < MMplanet.debris_vector.size(); ++i) {
                Debris& debris = MMplanet.debris_vector[i];

                if (debris.active) {
                    // Debris to move towards player when in contact with beam	
                    if (
                        AEVec2Distance(&MMplayer.beam_pos, &debris.position) <= MMplayer.beam_height / 2) {
                        debris.state = MOVE_TOWARDS_PLAYER;
                    }
                    // Beam active, but not colliding with player beam or drone beam
                    else if (debris.state == MOVE_TOWARDS_PLAYER && debris.state != MOVE_TOWARDS_DRONE)
                        debris.state = MOVE_TOWARDS_PLANET;

                    if (AEVec2Distance(&MMplayer.position, &debris.position) <= (MMplayer.size + debris.size) / 2) {
                        // Debris to be destroyed when in contact with player
                        MMplayer.current_capacity++;
                        debris.active = false;
                    }
                }
            }
        }
        else {
            for (int i = 0; i < MMplanet.debris_vector.size(); ++i) {
                Debris& debris = MMplanet.debris_vector[i];
                // Beam no longer active, if debris was moving towards player, make it move towards planet
                if (debris.state == MOVE_TOWARDS_PLAYER)
                    debris.state = MOVE_TOWARDS_PLANET;
            }
        }

    }


    // ============================
    //  WRAPPING OF PLAYER POSITION 
    // ============================
    MMplayer.position.x = AEWrap(MMplayer.position.x, AEGfxGetWinMinX(), AEGfxGetWinMaxX());
    MMplayer.position.y = AEWrap(MMplayer.position.y, AEGfxGetWinMinY(), AEGfxGetWinMaxY());


    // ============================
    //  SPAWN SHUTTLE
    // ============================
    if (MMplanet.shuttle_timer >= MMplanet.shuttle_time_to_spawn)
    {
        MMspawn_shuttle();
        MMplanet.shuttle_timer = 0.0;			// Reset shuttle timer
        MMplanet.shuttle_time_to_spawn = static_cast<f32>(rand() % (SHUTTLE_SPAWN_TIME_MAX - SHUTTLE_SPAWN_TIME_MIN) + SHUTTLE_SPAWN_TIME_MIN);	// Randomize time_to_spawn
    }

    // ===================================
    //  UPDATE SHUTTLE TIMER AND DIRECTION
    // ===================================
    MMplanet.shuttle_timer += static_cast<f32>(MMframe_time);
   
    MMplanet.direction += PLANET_ROT_SPEED * static_cast<f32>(MMframe_time);


    // =========================================
    // Calculate the matrix for Planet
    // =========================================
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

    // Tractor beam
    AEMtx33Scale(&scale, MMplayer.beam_width, MMplayer.beam_height);
    AEMtx33Rot(&rot, MMplayer.direction + PI / 2);
    AEMtx33Trans(&trans, MMplayer.beam_pos.x, MMplayer.beam_pos.y);
    AEMtx33Concat(&MMplayer.beam_transform, &rot, &scale);
    AEMtx33Concat(&MMplayer.beam_transform, &trans, &MMplayer.beam_transform);

    
    // =========================================
    // ERASE DEBRIS IF IT IS NOT ACTIVE
    // =========================================
    for (size_t i = 0; i < MMplanet.debris_vector.size(); i++) {
        if (MMplanet.debris_vector[i].active == false && MMplanet.debris_vector[i].explosion.is_draw == 0) {
            MMplanet.debris_vector.erase(MMplanet.debris_vector.begin() + i);
        }
    }


    // =========================================
    // GAME LOGIC FOR DEBRIS
    // =========================================
    for (size_t i = 0; i < MMplanet.debris_vector.size(); i++) {
        Debris& debris = MMplanet.debris_vector[i];

        if (debris.state == MOVE_TOWARDS_PLAYER && MMplayer.state == PLAYER_ORBIT) {
            // Move debris towards player
            AEVec2 diff;
            AEVec2Sub(&diff, &MMplayer.position, &debris.position);
            AEVec2Normalize(&diff, &diff);
            AEVec2Scale(&diff, &diff, (MMplayer.beam_level + 1) * 0.4f);
            AEVec2Add(&debris.position, &debris.position, &diff);
        }
       
        if (debris.state == MOVE_TOWARDS_PLANET) {
            // Move debris back to orbit
            AEVec2 diff;
            AEVec2Sub(&diff, &MMplanet.position, &debris.position);
            AEVec2Normalize(&diff, &diff);
            AEVec2Add(&debris.position, &debris.position, &diff);

            // Debris to rotate around planet when in orbit range
            if (AEVec2Distance(&MMplanet.position, &debris.position) <= (MMplanet.size / 2.0 + debris.distance)) {
                debris.angle = static_cast<f32>(atan2(debris.position.y - MMplanet.position.y, debris.position.x - MMplanet.position.x));
                debris.state = ORBIT_AROUND_PLANET;
            }
        }

        if (debris.state == ORBIT_AROUND_PLANET) {
            // Orbit around planet
            debris.angle -= AEDegToRad(0.125f) * debris.turning_speed * static_cast<f32>(MMframe_time);
            debris.position.x = MMplanet.position.x + ((MMplanet.size / 2) + debris.distance) * AECos(debris.angle);
            debris.position.y = MMplanet.position.y + ((MMplanet.size / 2) + debris.distance) * AESin(debris.angle);
        }
    }

    // =============================================
    // GAME LOGIC FOR ORBIT INDICATOR AND ITS MATRIX
    // =============================================
    if (MMplayer.state == PLAYER_ORBIT)
    {
        // Check if the spaceship just started orbiting a planet
        if (!isOrbitingPlanet)
        {
            MMplayer.halo_scale_lerp = 0; // Reset the Lerp value for halo scale
            isOrbitingPlanet = true;
        }

        // Calculate progress based on the elapsed time and animation duration
        float progress = static_cast<float>(MMframe_time / MManimationDuration);
        if (progress > 1.0f)
        {
            progress = 1.0f; // Clamp progress to 1.0f to prevent overshooting
        }

        // Use the EaseInOutBack easing function for smooth interpolation
        float easedProgress = EaseInOutBack(0, 1, progress);

        // Update the Lerp value for the halo scale
        MMplayer.halo_scale_lerp += (1.0f - MMplayer.halo_scale_lerp) * 0.5f;

        f32 val{ MMplanet.size + 100.f };

        // Use the Lerp value to scale the halo
        AEMtx33Scale(&scale, val * MMplayer.halo_scale_lerp, val * MMplayer.halo_scale_lerp);
        AEMtx33Trans(&trans, MMplanet.position.x, MMplanet.position.y);
        AEMtx33Concat(&MMplayer.orbit_halo_transform, &rot, &scale);
        AEMtx33Concat(&MMplayer.orbit_halo_transform, &trans, &MMplayer.orbit_halo_transform);
    }
    else
    {
        // The spaceship is not orbiting a planet, so set the flag to false
        isOrbitingPlanet = false;
    }


    // =======================================
    // COLLISION CHECK WITH SHUTTLE AND DEBRIS
    // =======================================
    
    for (size_t i{}; i < MMshuttle_vector.size(); i++) {
        for (int k = 0; k < MMplanet.debris_vector.size(); ++k) {
            if (MMshuttle_vector[i].active) {
                if (AEVec2Distance(&MMshuttle_vector[i].position, &MMplanet.debris_vector[k].position) <= (MM_SHUTTLE_SIZE / 2 + MMplanet.debris_vector[k].size / 2)) { // if collided
                    MMplanet.debris_vector[k].active = false;
                    MMshuttle_vector[i].active = false;
                    MMplanet.debris_vector[k].explosion.is_draw = 1;
                    MMplanet.debris_vector[k].explosion.position = MMplanet.debris_vector[k].position;
                    
                    break;
                }
            }
            else {
                break;
            }
        }
    }

    // =======================================
    // EXPLOPSION LOGIC AND MATRIX
    // =======================================
    for (size_t i = 0; i < MMplanet.debris_vector.size(); i++) {
        Explosion& explosion = MMplanet.debris_vector[i].explosion;

        if (explosion.is_draw == 1) {

            if (explosion.timer <= explosion.total_time) {
                explosion.timer += static_cast<f32>(MMframe_time);
                AEMtx33Scale(&scale, explosion.width, explosion.height);
                AEMtx33Rot(&rot, 0);
                AEMtx33Trans(&trans, explosion.position.x, explosion.position.y);
                AEMtx33Concat(&explosion.transform, &rot, &scale);
                AEMtx33Concat(&explosion.transform, &trans, &explosion.transform);
            }
            else {
                explosion.timer = 0;
                explosion.is_draw = 0;
            }
        }
    }


    // =======================================
    // calculate the matrix for DEBRIS
    // =======================================
    
    for (size_t i = 0; i < MMplanet.debris_vector.size(); i++) {
        if (MMplanet.debris_vector[i].active)
        {
            AEMtx33Scale(&MMplanet.debris_vector[i].scale, MMplanet.debris_vector[i].size, MMplanet.debris_vector[i].size);

            AEMtx33Rot(&MMplanet.debris_vector[i].rotate, AEDegToRad(MMplanet.debris_vector[i].angle));

            AEMtx33Trans(&MMplanet.debris_vector[i].translate, MMplanet.debris_vector[i].position.x, MMplanet.debris_vector[i].position.y);

            AEMtx33Concat(&MMplanet.debris_vector[i].transform, &MMplanet.debris_vector[i].rotate, &MMplanet.debris_vector[i].scale);
            AEMtx33Concat(&MMplanet.debris_vector[i].transform, &MMplanet.debris_vector[i].translate, &MMplanet.debris_vector[i].transform);

        }
    }
    

    // =======================================
    // GAME LOGIC FOR SHUTTLE AND ITS MATRIX
    // =======================================
    for (size_t i{}; i < MMshuttle_vector.size(); i++)
    {
        if (MMshuttle_vector[i].active)
        {
            AEVec2 added{};

            // Shuttle accelerating
            AEVec2Add(&added, &added, &MMshuttle_vector[i].direction);
            AEVec2Scale(&added, &added, MMshuttle_vector[i].acceleration * static_cast<f32>(MMframe_time));
            AEVec2Add(&MMshuttle_vector[i].velocity, &added, &MMshuttle_vector[i].velocity);

            // Limiting shuttle velocity
            AEVec2Scale(&MMshuttle_vector[i].velocity, &MMshuttle_vector[i].velocity, 0.99f);

            // Update shuttle position
            MMshuttle_vector[i].position.x += MMshuttle_vector[i].velocity.x * static_cast<f32>(MMframe_time);
            MMshuttle_vector[i].position.y += MMshuttle_vector[i].velocity.y * static_cast<f32>(MMframe_time);

            AEMtx33Trans(&MMshuttle_vector[i].translate, MMshuttle_vector[i].position.x, MMshuttle_vector[i].position.y);
            AEMtx33Concat(&MMshuttle_vector[i].transform, &MMshuttle_vector[i].rotate, &MMshuttle_vector[i].scale);
            AEMtx33Concat(&MMshuttle_vector[i].transform, &MMshuttle_vector[i].translate, &MMshuttle_vector[i].transform);

            // If shuttle escapes
            if (MMshuttle_vector[i].lifespan <= 0)
            {
                MMshuttle_vector[i].active = false;
                MMspawn_debris_shuttle(MMshuttle_vector[i].position, 3);
            }
            MMshuttle_vector[i].lifespan -= static_cast<f32>(MMframe_time);
        }
    }


}

void main_menu::draw()
{
    if (MMtotal_time >= 1.5f)
    {
        // Clear the screen
        AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

        // Draw the background mesh
        RenderMMBackground.RenderSprite(TexMMBackground, 0.f, 0.f, 800.f, 450.f, pMeshMMBackground);
        RenderMMBackground.RenderSprite(TexTitle, -250.f, 50.f, 800.f, 450.f, pMeshMMBackground);

        // ====================
        //  DRAWING ORBIT RING
        // ====================
        AEGfxTextureSet(MMorbit_tex, 0, 0);
        AEGfxSetTransform(MMplanet.orbit_transform.m);
        AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);

        if (MMplayer.state == PLAYER_ORBIT) {
            AEGfxTextureSet(MMorbit_halo_tex, 0, 0);
            AEGfxSetTransform(MMplayer.orbit_halo_transform.m);
            AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);
        }

        // ====================
        //  DRAWING PLANET
        // ====================
        AEGfxTextureSet(MMtexplanet, 0, 0);
        AEGfxSetTransform(MMplanet.transform.m);
        AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);

        // ====================
        //  DRAWING PLAYER
        // ====================
        AEGfxTextureSet(MMtexplayer, 0, 0);
        AEGfxSetTransform(MMplayer.player_transform.m);
        AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);

        // =====================
        //  DRAWING TRACTOR BEAM
        // =====================
        if (AEInputCheckCurr(AEVK_SPACE) && MMplayer.state == PLAYER_ORBIT) {
            AEGfxTextureSet(MMtexbeam, 0, 0);
            AEGfxSetTransform(MMplayer.beam_transform.m);
            AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);
        }


        // ====================
        //  DRAWING DEBRIS 
        // ====================
        for (size_t i = 0; i < MMplanet.debris_vector.size(); i++) {
            if (MMplanet.debris_vector[i].active)
            {
                AEGfxTextureSet(MMtexdebris, 0, 0);
                AEGfxSetTransform(MMplanet.debris_vector[i].transform.m);
                AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);
            }
        }

        // ====================
        //  DRAWING EXOLOSION
        // ====================
        for (size_t i = 0; i < MMplanet.debris_vector.size(); i++) {

            Explosion& explosion = MMplanet.debris_vector[i].explosion;
            if (explosion.is_draw) {

                if (explosion.timer <= explosion.total_time) {
                    AEGfxSetTransparency(explosion.total_time - explosion.timer);

                    AEGfxTextureSet(MMexplosion_tex, 0, 0);
                    AEGfxSetTransform(explosion.transform.m);
                    AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);
                }
                else {
                    AEGfxSetTransparency(0.f);
                }
            }
        }


        // ====================
        //  DRAWING SHUTTLE
        // ====================
        for (size_t i{}; i < MMshuttle_vector.size(); i++)
        {
            AEGfxTextureSet(MMshuttle_tex, 0, 0);
            if (MMshuttle_vector[i].active)
            {
                if (MMshuttle_vector[i].lifespan <= SHUTTLE_MAX_LIFESPAN / 2.f)
                {
                    AEGfxSetTransparency(MMshuttle_vector[i].lifespan / (SHUTTLE_MAX_LIFESPAN / 2.0f));
                }
                else
                {
                    AEGfxSetTransparency(1.f);
                }
                AEGfxSetTransform(MMshuttle_vector[i].transform.m);
                // Actually drawing the mesh
                AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);
            }
        }

        AEGfxSetTransparency(1.0f);

        // ====================
        //  DRAWING MENU BUTTONS
        // ====================
        menuButtons.draw(pMeshMM);
    }
}

void main_menu::free()
{
    MMplanet.debris_vector.clear();
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
    AEGfxTextureUnload(MMtexdebris);
    AEGfxTextureUnload(MMtexbeam);
    AEGfxTextureUnload(TexTitle);
    AEGfxTextureUnload(MMshuttle_tex);
    AEGfxTextureUnload(MMexplosion_tex);
    AEGfxTextureUnload(MMorbit_halo_tex);
    AEGfxTextureUnload(TexMMBackground); // unload the texture for the background image


}

// ===========================
//  SPAWN SHUTTLE IN MAIN MENU
// ===========================
void MMspawn_shuttle()
{
    Shuttles new_shuttle;

    new_shuttle.lifespan = SHUTTLE_MAX_LIFESPAN;
    new_shuttle.acceleration = SHUTTLE_MAX_ACCEL;

    new_shuttle.active = true;

    AEVec2Zero(&new_shuttle.velocity);

    new_shuttle.position.x = MMplanet.position.x - 250;
    new_shuttle.position.y = MMplanet.position.y + 250;

    f32 rand_angle = 45 + 90;
    new_shuttle.direction.x = cos(rand_angle);
    new_shuttle.direction.y = sin(rand_angle);

    AEMtx33Scale(&new_shuttle.scale, 50.f, 100.f);
    AEMtx33Rot(&new_shuttle.rotate, PI / 2 + rand_angle);

    MMshuttle_vector.push_back(new_shuttle);
}
