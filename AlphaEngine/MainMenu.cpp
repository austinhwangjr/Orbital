/******************************************************************************/
/*!
\file		MainMenu.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the main menu.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
 /******************************************************************************/
 /*-------------------------------------------------- Main Menu --------------------------------------------------*/
#include "pch.h"
#include "gch.h"
#include "Global.h"
#include "MainMenu.h"

main_menu::MainMenuState main_menu::currentState = MENU;

AEGfxTexture* TexMMBackground = nullptr;
AEGfxTexture* TexTitle = nullptr;

static bool isOrbitingPlanet = false; // flag for is orbiting
// Variables
float MManimationDuration = 1.0f; // Duration of the animation in seconds
bool MMbeam_active = false;
AEGfxVertexList* pMeshMMBackground;
AEGfxVertexList* pMeshMM;
AEGfxVertexList* pMeshObj;
static float MMPLANET_SIZE;						// Planet base size (radius)	
static float PLANET_ROT_SPEED;					// Planet rotation speed (radians)
static int	 SHUTTLE_SPAWN_TIME_MAX;			// Maximum time shuttles will spawn
static int   SHUTTLE_SPAWN_TIME_MIN;			// Minimum time shuttles will spawn
static int	 DEBRIS_MAX;						// Maximum number of debris on a planet
static int	 DEBRIS_MIN;						// Minimum number of debris on a planet
static float SHUTTLE_MAX_LIFESPAN;		        // Maximum life time for a shuttle before escaping (expiring)
static float SHUTTLE_MAX_ACCEL;			        // Maximum acceleration for a shuttle

static float SPEED_DEBRIS;
static int	 MAX_DEBRIS_SIZE;
static int	 MIN_DEBRIS_SIZE;
static float SHUTTLE_HEIGHT;
static int	 MAX_BUFFER;
static int	 MIN_BUFFER;
static float EXPLOSION_WIDTH;
static float EXPLOSION_HEIGHT;
static int   OUTERRIM_TO_DEBRIS;

Player MMplayer;
Planets MMplanet;
Debris MMdebris;
PlayerProj MMplayer_proj;
std::vector<Shuttles> MMshuttle_vector;
std::vector<PlayerProj> MMProj_vector;

AEGfxTexture* MMtexplayer;
AEGfxTexture* MMtexplanet;
AEGfxTexture* MMorbit_tex;
AEGfxTexture* MMtexdebris;
AEGfxTexture* MMtexbeam;
AEGfxTexture* MMshuttle_tex;
AEGfxTexture* MMexplosion_tex;
AEGfxTexture* MMorbit_halo_tex;
AEGfxTexture* MMplayer_proj_tex;

AEGfxTexture* MM_Keys_W;
AEGfxTexture* MM_Keys_W_ACTIVE;
AEGfxTexture* MM_Keys_A;
AEGfxTexture* MM_Keys_A_ACTIVE;
AEGfxTexture* MM_Keys_S;
AEGfxTexture* MM_Keys_S_ACTIVE;
AEGfxTexture* MM_Keys_D;
AEGfxTexture* MM_Keys_D_ACTIVE;

AEGfxTexture* MM_Keys_Spacebar;
AEGfxTexture* MM_Keys_Spacebar_ACTIVE;

AEGfxTexture* MM_LMB;
AEGfxTexture* MM_LMB_ACTIVE;

//bools for keys
bool wKeyPressed = false;
bool aKeyPressed = false;
bool sKeyPressed = false;
bool dKeyPressed = false;
bool spacebarActivated = false;
bool LMB_KeyPressed = false;

float w_ButtonWidth = 75.f;
float w_ButtonHeight = 75.f;
float a_ButtonWidth = 75.f;
float a_ButtonHeight = 75.f;
float s_ButtonWidth = 75.f;
float s_ButtonHeight = 75.f;
float d_ButtonWidth = 75.f;
float d_ButtonHeight = 75.f;

// WASD
float w_ButtonX = -655.f;
float w_ButtonY = -180.f;
float a_ButtonX = -750.f;
float a_ButtonY = -275.f;
float s_ButtonX = -655.f;
float s_ButtonY = -275.f;
float d_ButtonX = -560.f;
float d_ButtonY = -275.f;

//WASD ACTIVATED
float w_ButtonWidthHover = 90.f;
float w_ButtonHeightHover = 90.f;
float a_ButtonWidthHover = 90.f;
float a_ButtonHeightHover = 90.f;
float s_ButtonWidthHover = 90.f;
float s_ButtonHeightHover = 90.f;
float d_ButtonWidthHover = 90.f;
float d_ButtonHeightHover = 90.f;

float w_OriginalButtonWidth = 75.f;
float w_OriginalButtonHeight = 75.f;

// SPACEBAR
float spacebar_ButtonX = -655.f;
float spacebar_ButtonY = -375.f;
float spacebar_ButtonWidth = 225.f;
float spacebar_ButtonHeight = 75.f;

// SPACEBAR ACTIVATED
float spacebar_ButtonWidthActivated = 250.f;
float spacebar_ButtonHeightActivated = 90.f;

float spacebar_OriginalWidth = 225.f;
float spacebar_OriginalHeight = 75.f;

f32 MMtotal_time = 0.f;

// MOUSE
float mouseTexX = -380.f;
float mouseTexY = -275.f;
float mouseWidth = 200.f;
float mouseHeight = 300.f;

// class declaration 
Menu_Button menuButtons;
Rendering createMesh;
Rendering RenderMMBackground;

//IMPORT DATA VECTOR
std::map<std::string, f32>  MMPlayerDataMap;
std::vector<Data>           MMPlayerData;
std::map<std::string, f32> 	MMPlanetDataMap;
std::vector<Data> 			MMPlanetData;
std::map<std::string, f32> 	MMShuttleDataMap;
std::vector<Data> 			MMShuttleData;
std::map<std::string, f32> 	MMDebrisDataMap;
std::vector<Data> 			MMDebrisData;
std::map<std::string, f32> 	MMProjDataMap;
std::vector<Data> 			MMProjData;

void main_menu::load()
{
    TexMMBackground = AEGfxTextureLoad("Assets/MainMenu/mm_Background.png");
    TexTitle = AEGfxTextureLoad("Assets/MainMenu/mm_Title.png");

    menuButtons.load("Assets/MainMenu/mm_StartButton.png",
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
    MMplayer_proj_tex = AEGfxTextureLoad("Assets/MainLevel/ml_Debris.png");

    ImportDataFromFile("Assets/GameObjectData/PlayerData.txt", MMPlayerData, MMPlayerDataMap);
    ImportDataFromFile("Assets/GameObjectData/PlanetData.txt", MMPlanetData, MMPlanetDataMap);
    ImportDataFromFile("Assets/GameObjectData/ShuttleData.txt", MMShuttleData, MMShuttleDataMap);
    ImportDataFromFile("Assets/GameObjectData/DebrisData.txt", MMDebrisData, MMDebrisDataMap);
    ImportDataFromFile("Assets/GameObjectData/PlayerProjectileData.txt", MMProjData, MMProjDataMap);


    MM_Keys_W = AEGfxTextureLoad("Assets/MainMenu/mm_W.png");
    MM_Keys_W_ACTIVE = AEGfxTextureLoad("Assets/MainMenu/mm_W_Hover.png");
    MM_Keys_A = AEGfxTextureLoad("Assets/MainMenu/mm_A.png");
    MM_Keys_A_ACTIVE = AEGfxTextureLoad("Assets/MainMenu/mm_A_Hover.png");
    MM_Keys_S = AEGfxTextureLoad("Assets/MainMenu/mm_S.png");
    MM_Keys_S_ACTIVE = AEGfxTextureLoad("Assets/MainMenu/mm_S_Hover.png");
    MM_Keys_D = AEGfxTextureLoad("Assets/MainMenu/mm_D.png");
    MM_Keys_D_ACTIVE = AEGfxTextureLoad("Assets/MainMenu/mm_D_Hover.png");

    MM_Keys_Spacebar = AEGfxTextureLoad("Assets/MainMenu/mm_Spacebar.png");
    MM_Keys_Spacebar_ACTIVE = AEGfxTextureLoad("Assets/MainMenu/mm_SpacebarActivated.png");

    MM_LMB = AEGfxTextureLoad("Assets/MainMenu/mm_Mouse.png");
    MM_LMB_ACTIVE = AEGfxTextureLoad("Assets/MainMenu/mm_MouseActivated.png");

    AudioManager::LoadSound("Assets/BGM/one-last-time-141289.mp3", true);

    AudioManager::LoadSound("Assets/BGM/9mm-pistol-shot-6349.mp3", false);
}

void main_menu::init()
{
    if (!AudioManager::isBGMPlaying)
    {
        AudioManager::PlayBGM("Assets/BGM/one-last-time-141289.mp3", 0.5f);
    }
    // Set the camera position to (0, 0) for the background mesh
    AEGfxSetCamPosition(0.f, 0.f);

    // Create a background mesh
    createMesh.BackgroundMesh(pMeshMMBackground);

    // Create a square mesh for the buttons
    createMesh.MainMenuSquareMesh(pMeshMM);
    createMesh.GameLevelSquareMesh(pMeshObj);
    menuButtons.init();

    // debugging logs
    AE_ASSERT_MESG(pMeshMM, "Error: Failed to create pMeshMM in MainMenu.cpp!");
    AE_ASSERT_MESG(pMeshMMBackground, "Error: Failed to create pMeshMMBackground in MainMenu.cpp!");

    std::cout << std::endl;
    std::cout << "------------------------- MainMenu Initialised -------------------------" << std::endl << std::endl;

    //--------------------Player--------------------
    MMplayer.state = PLAYER_FLY;

    MMplayer.position.x = MMPlayerDataMap["position.x"];
    MMplayer.position.y = MMPlayerDataMap["position.y"];

    MMplayer.velocity.x = MMPlayerDataMap["velocity.x"];
    MMplayer.velocity.y = MMPlayerDataMap["velocity.y"];

    MMplayer.size = MMPlayerDataMap["size"];

    MMplayer.mov_speed = MMPlayerDataMap["mov_speed"];
    MMplayer.rot_speed = MMPlayerDataMap["rot_speed"] * PI;

    MMplayer.shortest_distance = MMPlayerDataMap["shortest_distance"];

    MMplayer.direction = MMPlayerDataMap["direction"];

    MMplayer.current_capacity = static_cast<int>(MMPlayerDataMap["current_capacity"]);
    MMplayer.max_capacity = static_cast<int>(MMPlayerDataMap["max_capacity"]);

    MMplayer.can_leave_orbit = true;

    MMplayer.timer = MMPlayerDataMap["timer"];
    MMplayer.max_timer = MMPlayerDataMap["max_timer"];

    //--------------------Score-keeping--------------------
    MMplayer.score = static_cast<int>(MMPlayerDataMap["score"]);
    MMplayer.credits = static_cast<int>(MMPlayerDataMap["credits"]);

    //--------------------Upgrade Levels--------------------
    MMplayer.mov_speed_level = static_cast<int>(MMPlayerDataMap["mov_speed_level"]);
    MMplayer.capacity_level = static_cast<int>(MMPlayerDataMap["capacity_level"]);
    MMplayer.space_station_count = static_cast<int>(MMPlayerDataMap["space_station_count"]);
    MMplayer.beam_level = static_cast<int>(MMPlayerDataMap["beam_level"]);

    //--------------------Tractor Beam--------------------
    MMplayer.beam_pos.x = MMPlayerDataMap["beam_pos.x"];
    MMplayer.beam_pos.y = MMPlayerDataMap["beam_pos.y"];

    MMplayer.beam_width = MMplayer.size * 0.6f;
    MMplayer.beam_height = MMplayer.beam_width * 2.f;

    //--------------------Planet Halo--------------------
    MMplanet.halo_scale_lerp = MMPlanetDataMap["Halo_Indicator"];

    //--------------------Debris Variables--------------------
    SPEED_DEBRIS = MMDebrisDataMap["Speed_Of_Debris"];
    MAX_DEBRIS_SIZE = static_cast<int>(MMDebrisDataMap["Maximum_Debris_Size"]);
    MIN_DEBRIS_SIZE = static_cast<int>(MMDebrisDataMap["Minimum_Debris_Size"]);
    MAX_BUFFER = static_cast<int>(MMDebrisDataMap["Maximum_Buffer"]);
    MIN_BUFFER = static_cast<int>(MMDebrisDataMap["Minimum_Buffer"]);
    EXPLOSION_WIDTH = MMDebrisDataMap["Explosion_Width"];
    EXPLOSION_HEIGHT = MMDebrisDataMap["Explosion_Height"];
    OUTERRIM_TO_DEBRIS = static_cast<int>(MMDebrisDataMap["Distance_To_Planet"]);

    //--------------------Planet Variables--------------------

    PLANET_ROT_SPEED = MMPlanetDataMap["Planet_Rotation_Speed"] * PI;

    SHUTTLE_SPAWN_TIME_MAX = static_cast<int>(MMPlanetDataMap["Maximum_Time_Shuttle_Spawn"]);
    SHUTTLE_SPAWN_TIME_MIN = static_cast<int>(MMPlanetDataMap["Minimum_Time_Shuttle_Spawn"]);
    DEBRIS_MAX = static_cast<int>(MMPlanetDataMap["Maximum_Debris"]);
    DEBRIS_MIN = static_cast<int>(MMPlanetDataMap["Minimum_Debris"]);

    SHUTTLE_MAX_LIFESPAN = MMShuttleDataMap["Shuttle_Lifespan"];
    SHUTTLE_MAX_ACCEL = MMShuttleDataMap["Shuttle_Acceleration"];
    SHUTTLE_HEIGHT = MMShuttleDataMap["Shuttle_Height"];

    //--------------------Projectile--------------------
    MMplayer_proj.position.x = MMProjDataMap["Position_X"];
    MMplayer_proj.position.y = MMProjDataMap["Position_Y"];

    //--------------------Debris--------------------

    MMplanet.max_debris = rand() % (DEBRIS_MAX - DEBRIS_MIN) + DEBRIS_MIN;      // Randomize debris count on planet spawn
    MMplanet.debris_vector = MM_create_debris(MMplanet.position.x, MMplanet.position.y, MMplanet.size, MMplanet.max_debris);
    MMplanet.position.x = g_windowWidth / 2.f;
    MMplanet.position.y = -g_windowHeight / 2.f;
    MMplanet.size = MMPlanetDataMap["MainMenu_Planet_Size"];
    MMplanet.shuttle_timer = MMShuttleDataMap["Shuttle_Timer"];
    MMplanet.shuttle_time_to_spawn = static_cast<f32>(rand() % (SHUTTLE_SPAWN_TIME_MAX - SHUTTLE_SPAWN_TIME_MIN + 1) + SHUTTLE_SPAWN_TIME_MIN);
}

void main_menu::update()
{
    MMtotal_time += g_dt;

    // =======================
    //  PLAYER MOVEMENT
    // =======================

    // =======================
    //  FREE FLYING MODE
    // =======================
    float lerpSpeed = 0.15f;
    if (MMplayer.state == PLAYER_FLY)
    {
        if (AEInputCheckCurr(AEVK_W))
        {
            wKeyPressed = true;
            if (MMplayer.state == PLAYER_ORBIT)
            {
                wKeyPressed = false;
            }
            w_ButtonWidth = Lerp(w_ButtonWidth, w_ButtonWidthHover, lerpSpeed);
            w_ButtonHeight = Lerp(w_ButtonHeight, w_ButtonHeightHover, lerpSpeed);

            AEVec2 acceleration;
            AEVec2Set(&acceleration, AECos(MMplayer.direction), AESin(MMplayer.direction));

            // Find the velocity according to the acceleration
            AEVec2Scale(&acceleration, &acceleration, MMplayer.mov_speed * static_cast<f32>(MMplayer.mov_speed_level + 3) / 2.f);
            MMplayer.velocity.x = MMplayer.velocity.x + acceleration.x * g_dt;
            MMplayer.velocity.y = MMplayer.velocity.y + acceleration.y * g_dt;

            // Limit player's speed
            AEVec2Scale(&MMplayer.velocity, &MMplayer.velocity, 0.99f);
        }
        else
        {
            wKeyPressed = false;
            w_ButtonWidth = Lerp(w_ButtonWidth, w_OriginalButtonWidth, lerpSpeed);
            w_ButtonHeight = Lerp(w_ButtonHeight, w_OriginalButtonHeight, lerpSpeed);
        }

        if (AEInputCheckCurr(AEVK_S))
        {
            sKeyPressed = true;
            if (MMplayer.state == PLAYER_ORBIT)
            {
                sKeyPressed = false;
            }
            s_ButtonWidth = Lerp(s_ButtonWidth, s_ButtonWidthHover, lerpSpeed);
            s_ButtonHeight = Lerp(s_ButtonHeight, s_ButtonHeightHover, lerpSpeed);

            AEVec2 acceleration;
            AEVec2Set(&acceleration, -AECos(MMplayer.direction), -AESin(MMplayer.direction));

            // Find the velocity according to the decceleration
            AEVec2Scale(&acceleration, &acceleration, MMplayer.mov_speed * (MMplayer.mov_speed_level + 1) / 2.f);
            MMplayer.velocity.x = MMplayer.velocity.x + acceleration.x * g_dt;
            MMplayer.velocity.y = MMplayer.velocity.y + acceleration.y * g_dt;

            // Limit player's speed
            AEVec2Scale(&MMplayer.velocity, &MMplayer.velocity, 0.99f);
        }
        else
        {
            sKeyPressed = false;
            s_ButtonWidth = Lerp(s_ButtonWidth, w_OriginalButtonWidth, lerpSpeed);
            s_ButtonHeight = Lerp(s_ButtonHeight, w_OriginalButtonHeight, lerpSpeed);
        }

        if (AEInputCheckCurr(AEVK_A))
        {
            aKeyPressed = true;
            if (MMplayer.state == PLAYER_ORBIT)
            {
                aKeyPressed = false;
            }
            a_ButtonWidth = Lerp(a_ButtonWidth, a_ButtonWidthHover, lerpSpeed);
            a_ButtonHeight = Lerp(a_ButtonHeight, a_ButtonHeightHover, lerpSpeed);

            MMplayer.direction += MMplayer.rot_speed * g_dt;
            MMplayer.direction = AEWrap(MMplayer.direction, -PI, PI);
        }
        else
        {
            aKeyPressed = false;
            a_ButtonWidth = Lerp(a_ButtonWidth, w_OriginalButtonWidth, lerpSpeed);
            a_ButtonHeight = Lerp(a_ButtonHeight, w_OriginalButtonHeight, lerpSpeed);
        }

        if (AEInputCheckCurr(AEVK_D))
        {
            dKeyPressed = true;
            if (MMplayer.state == PLAYER_ORBIT)
            {
                dKeyPressed = false;
            }
            d_ButtonWidth = Lerp(d_ButtonWidth, d_ButtonWidthHover, lerpSpeed);
            d_ButtonHeight = Lerp(d_ButtonHeight, d_ButtonHeightHover, lerpSpeed);

            MMplayer.direction -= MMplayer.rot_speed * g_dt;
            MMplayer.direction = AEWrap(MMplayer.direction, -PI, PI);
        }
        else
        {
            dKeyPressed = false;
            d_ButtonWidth = Lerp(d_ButtonWidth, w_OriginalButtonWidth, lerpSpeed);
            d_ButtonHeight = Lerp(d_ButtonHeight, w_OriginalButtonHeight, lerpSpeed);
        }

        // Mouse click update tentative
        if (AEInputCheckCurr(AEVK_LBUTTON))
        {
            LMB_KeyPressed = true;
        }
        else
        {
            LMB_KeyPressed = false;
        }

        if (AEVec2Distance(&MMplanet.position, &MMplayer.position) <= (MMplanet.size / 2 + MMplanet.orbit_range))
        {
            MMplayer.direction = static_cast<f32>(atan2(MMplayer.position.y - MMplanet.position.y, MMplayer.position.x - MMplanet.position.x));
            MMplayer.state = PLAYER_ORBIT;
        }

        // =======================
        // Update Player Position
        // =======================

        MMplayer.position.x = MMplayer.position.x + MMplayer.velocity.x * g_dt;
        MMplayer.position.y = MMplayer.position.y + MMplayer.velocity.y * g_dt;
    }

    if (MMplayer.state == PLAYER_TRANSIT)
    {
        // ================
        // Check for input
        // ================

        if (AEInputCheckCurr(AEVK_W)) {
            AEVec2 added;
            AEVec2Set(&added, AECos(MMplayer.direction), AESin(MMplayer.direction));

            // Find the velocity according to the acceleration
            AEVec2Scale(&added, &added, MMplayer.mov_speed / 2.f);
            MMplayer.velocity.x = MMplayer.velocity.x + added.x * g_dt;
            MMplayer.velocity.y = MMplayer.velocity.y + added.y * g_dt;

            // Limit player's speed
            AEVec2Scale(&MMplayer.velocity, &MMplayer.velocity, 0.99f);

            // Add to timer. Change to flying state after 1s
            MMplayer.timer += g_dt;
            if (MMplayer.timer >= MMplayer.max_timer) {
                // Change state and reset timer
                MMplayer.state = PLAYER_FLY;
                MMplayer.timer = 0.f;
            }
        }

        else
        {
            // Move player back to orbit
            AEVec2 diff;
            AEVec2Sub(&diff, &MMplanet.position, &MMplayer.position);
            AEVec2Normalize(&diff, &diff);
            AEVec2Scale(&diff, &diff, MMplayer.mov_speed * g_dt);
            AEVec2Add(&MMplayer.position, &MMplayer.position, &diff);

            MMplayer.timer -= g_dt;

            // Debris to rotate around planet when in orbit range
            if (AEVec2Distance(&MMplanet.position, &MMplayer.position) <= (MMplanet.size / 2 + MMplanet.orbit_range)) {
                // Change state and reset timer
                MMplayer.state = PLAYER_ORBIT;
                MMplayer.timer = 0.f;
            }
        }

        // =======================
        // Update player position
        // =======================

        MMplayer.position.x = MMplayer.position.x + MMplayer.velocity.x * g_dt;
        MMplayer.position.y = MMplayer.position.y + MMplayer.velocity.y * g_dt;
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
            aKeyPressed = true;
            if (MMplayer.state == PLAYER_FLY)
            {
                aKeyPressed = false;
            }
            a_ButtonWidth = Lerp(a_ButtonWidth, a_ButtonWidthHover, lerpSpeed);
            a_ButtonHeight = Lerp(a_ButtonHeight, a_ButtonHeightHover, lerpSpeed);

            MMplayer.direction += (MMplayer.rot_speed / 2) * g_dt;

            MMplayer.position.x = MMplanet.position.x + (MMplanet.size / 2 + MMplanet.orbit_range) * AECos(MMplayer.direction);
            MMplayer.position.y = MMplanet.position.y + (MMplanet.size / 2 + MMplanet.orbit_range) * AESin(MMplayer.direction);
        }
        else
        {
            aKeyPressed = false;
            a_ButtonWidth = Lerp(a_ButtonWidth, w_OriginalButtonWidth, lerpSpeed);
            a_ButtonHeight = Lerp(a_ButtonHeight, w_OriginalButtonHeight, lerpSpeed);
        }

        if (AEInputCheckCurr(AEVK_D) && MMplayer.position.x <= AEGfxGetWinMaxX() - MMplayer.size)
        {
            dKeyPressed = true;
            if (MMplayer.state == PLAYER_FLY)
            {
                dKeyPressed = false;
            }
            d_ButtonWidth = Lerp(d_ButtonWidth, d_ButtonWidthHover, lerpSpeed);
            d_ButtonHeight = Lerp(d_ButtonHeight, d_ButtonHeightHover, lerpSpeed);

            MMplayer.direction -= (MMplayer.rot_speed / 2) * g_dt;

            MMplayer.position.x = MMplanet.position.x + (MMplanet.size / 2 + MMplanet.orbit_range) * AECos(MMplayer.direction);
            MMplayer.position.y = MMplanet.position.y + (MMplanet.size / 2 + MMplanet.orbit_range) * AESin(MMplayer.direction);
        }
        else
        {
            dKeyPressed = false;
            d_ButtonWidth = Lerp(d_ButtonWidth, w_OriginalButtonWidth, lerpSpeed);
            d_ButtonHeight = Lerp(d_ButtonHeight, w_OriginalButtonHeight, lerpSpeed);
        }

        if (AEInputCheckPrev(AEVK_W))
        {
            wKeyPressed = true;
            MMplayer.can_leave_orbit = false;
        }
        else
        {
            MMplayer.can_leave_orbit = true;
            wKeyPressed = false;
        }

        if (AEInputCheckPrev(AEVK_S))
        {
            sKeyPressed = true;
            s_ButtonWidth = Lerp(s_ButtonWidth, s_ButtonWidthHover, lerpSpeed);
            s_ButtonHeight = Lerp(s_ButtonHeight, s_ButtonHeightHover, lerpSpeed);
        }
        else
        {
            sKeyPressed = false;
            s_ButtonWidth = Lerp(s_ButtonWidth, w_OriginalButtonWidth, lerpSpeed);
            s_ButtonHeight = Lerp(s_ButtonHeight, w_OriginalButtonHeight, lerpSpeed);
        }



        if (AEInputCheckCurr(AEVK_W) && MMplayer.can_leave_orbit) {
            AEVec2Zero(&MMplayer.velocity);
            MMplayer.state = PLAYER_TRANSIT;
        }


        // tractor beam update
        if (AEInputCheckCurr(AEVK_SPACE))
        {
            spacebarActivated = true;
            MMbeam_active = true;

            spacebar_ButtonWidth = Lerp(spacebar_ButtonWidth, spacebar_ButtonWidthActivated, lerpSpeed);
            spacebar_ButtonHeight = Lerp(spacebar_ButtonHeight, spacebar_ButtonHeightActivated, lerpSpeed);
        }
        else
        {
            spacebarActivated = false;
            MMbeam_active = false;
            spacebar_ButtonWidth = Lerp(spacebar_ButtonWidth, spacebar_OriginalWidth, lerpSpeed);
            spacebar_ButtonHeight = Lerp(spacebar_ButtonHeight, spacebar_OriginalHeight, lerpSpeed);
        }

        // Mouse click update tentative
        if (AEInputCheckCurr(AEVK_LBUTTON))
        {
            LMB_KeyPressed = true;
        }
        else
        {
            LMB_KeyPressed = false;
        }

        // ================================
        // Update player and beam position
        // ================================

        if (AEInputCheckCurr(AEVK_W) && MMplayer.can_leave_orbit) {
            MMplayer.position.x += AECos(MMplayer.direction);
            MMplayer.position.y += AESin(MMplayer.direction);
        }
        else {
            MMplayer.position.x = MMplanet.position.x + (MMplanet.size / 2.f + MMplanet.orbit_range) * AECos(MMplayer.direction);
            MMplayer.position.y = MMplanet.position.y + (MMplanet.size / 2.f + MMplanet.orbit_range) * AESin(MMplayer.direction);
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


    // =========================
    // PROJECTILE
    // =========================

    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {

        MMplayer_proj.size = MMProjDataMap["Size"];
        MMplayer_proj.speed = MMProjDataMap["Speed"];
        MMplayer_proj.velocity.x = MMProjDataMap["Velocity_X"];
        MMplayer_proj.velocity.y = MMProjDataMap["Velocity_Y"];
        MMplayer_proj.position = MMplayer.position;
        AEVec2Sub(&MMplayer_proj.velocity, &g_mouseWorld, &MMplayer.position);
        AEVec2Normalize(&MMplayer_proj.velocity, &MMplayer_proj.velocity);
        AEVec2Scale(&MMplayer_proj.velocity, &MMplayer_proj.velocity, MMplayer_proj.speed);

        MMplayer_proj.is_delete = static_cast<int>(MMProjDataMap["Delete_flag"]);

        MMProj_vector.push_back(MMplayer_proj);

        AudioManager::PlayOnce("Assets/BGM/9mm-pistol-shot-6349.mp3", 0.05f);
        
    }

    // =====================================
    // Update position of player projectile
    // =====================================

    for (int i = 0; i < MMProj_vector.size(); ++i) {
        PlayerProj& player_proj = MMProj_vector[i];

        player_proj.position.x += player_proj.velocity.x * g_dt;
        player_proj.position.y += player_proj.velocity.y * g_dt;
    }


    // ======================================================
    // Delete player projectile if outside screen boundaries
    // ======================================================
    for (int i = 0; i < MMProj_vector.size(); ++i) {
        PlayerProj& player_proj = MMProj_vector[i];
        if (player_proj.position.x > AEGfxGetWinMaxX() || player_proj.position.x < AEGfxGetWinMinX()) {
            if (player_proj.position.y > AEGfxGetWinMaxY() || player_proj.position.y < AEGfxGetWinMinY()) {
                player_proj.is_delete = 1;
            }
        }
    }

    // ===================================
    // Update player projectile instances
    // ===================================

    //Erase projectile upon collision
    for (int i = 0; i < MMProj_vector.size(); i++) {
        if (MMProj_vector[i].is_delete == 1) {
            MMProj_vector.erase(MMProj_vector.begin() + i);
        }
    }

    // ===========================================
    // Calculate the matrix for player projectile
    // ===========================================

    AEMtx33 projscale, projrot, projtrans;

    for (int i = 0; i < MMProj_vector.size(); ++i) {
        PlayerProj& player_proj = MMProj_vector[i];

        // Space station
        AEMtx33Scale(&projscale, player_proj.size, player_proj.size);
        AEMtx33Rot(&projrot, 0);
        AEMtx33Trans(&projtrans, player_proj.position.x, player_proj.position.y);
        AEMtx33Concat(&player_proj.transform, &projrot, &projscale);
        AEMtx33Concat(&player_proj.transform, &projtrans, &player_proj.transform);
    }


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
    MMplanet.shuttle_timer += g_dt;

    MMplanet.direction += PLANET_ROT_SPEED * g_dt;


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
            debris.angle -= AEDegToRad(0.125f) * debris.turning_speed * g_dt;
            debris.position.x = MMplanet.position.x + ((MMplanet.size / 2) + debris.distance) * AECos(debris.angle);
            debris.position.y = MMplanet.position.y + ((MMplanet.size / 2) + debris.distance) * AESin(debris.angle);
        }
    }

    // =============================================
    // GAME LOGIC FOR ORBIT INDICATOR AND ITS MATRIX
    // =============================================
    if (MMplayer.state == PLAYER_ORBIT)
    {
        // Update the Lerp value for the halo scale
        MMplanet.halo_scale_lerp += (1.0f - MMplanet.halo_scale_lerp) * 10.f * g_dt;
    }
    else
    {
        // Update the Lerp value for the halo scale
        if (MMplanet.halo_scale_lerp > 0.f)
        {
            MMplanet.halo_scale_lerp -= MMplanet.halo_scale_lerp * g_dt;
        }
    }

    MMplanet.halo_size = MMplanet.size + 60.f;

    // Use the Lerp value to scale the halo
    AEMtx33Scale(&scale, MMplanet.halo_size * MMplanet.halo_scale_lerp, MMplanet.halo_size * MMplanet.halo_scale_lerp);
    AEMtx33Rot(&rot, 0);
    AEMtx33Trans(&trans, MMplanet.position.x, MMplanet.position.y);
    AEMtx33Concat(&MMplanet.orbit_halo_transform, &rot, &scale);
    AEMtx33Concat(&MMplanet.orbit_halo_transform, &trans, &MMplanet.orbit_halo_transform);


    // =======================================
    // COLLISION CHECK WITH SHUTTLE AND DEBRIS
    // =======================================

    for (size_t i{}; i < MMshuttle_vector.size(); i++) {
        for (int k = 0; k < MMplanet.debris_vector.size(); ++k) {
            if (MMshuttle_vector[i].active) {
                if (AEVec2Distance(&MMshuttle_vector[i].position, &MMplanet.debris_vector[k].position) <= ((SHUTTLE_HEIGHT - 80) / 2 + MMplanet.debris_vector[k].size / 2)) { // if collided
                    MMplanet.debris_vector[k].active = false;
                    MMshuttle_vector[i].active = false;
                    MMplanet.debris_vector[k].explosion.is_draw = 1;
                    MMplanet.debris_vector[k].explosion.position = MMplanet.debris_vector[k].position;
                    MMspawn_debris_shuttle(MMshuttle_vector[i].position, 4);
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
                explosion.timer += g_dt;
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
            AEVec2 acceleration{};

            // Shuttle accelerating
            AEVec2Add(&acceleration, &acceleration, &MMshuttle_vector[i].direction);
            AEVec2Scale(&acceleration, &acceleration, MMshuttle_vector[i].acceleration * g_dt);
            AEVec2Add(&MMshuttle_vector[i].velocity, &acceleration, &MMshuttle_vector[i].velocity);

            // Limiting shuttle velocity
            AEVec2Scale(&MMshuttle_vector[i].velocity, &MMshuttle_vector[i].velocity, 0.99f);

            // Update shuttle position
            MMshuttle_vector[i].position.x += MMshuttle_vector[i].velocity.x * g_dt;
            MMshuttle_vector[i].position.y += MMshuttle_vector[i].velocity.y * g_dt;

            AEMtx33Trans(&MMshuttle_vector[i].translate, MMshuttle_vector[i].position.x, MMshuttle_vector[i].position.y);
            AEMtx33Concat(&MMshuttle_vector[i].transform, &MMshuttle_vector[i].rotate, &MMshuttle_vector[i].scale);
            AEMtx33Concat(&MMshuttle_vector[i].transform, &MMshuttle_vector[i].translate, &MMshuttle_vector[i].transform);

            // If shuttle escapes
            if (MMshuttle_vector[i].lifespan <= 0)
            {
                MMshuttle_vector[i].active = false;
                MMspawn_debris_shuttle(MMshuttle_vector[i].position, 3);
            }
            MMshuttle_vector[i].lifespan -= g_dt;
        }
    }

    // ================
    //  MENU BUTTONS
    // ================
    if (currentState == MENU)
    {
        menuButtons.update();
    }
    else if (currentState == HOW_TO_PLAY)
    {
        // Check if the window close button has been clicked
        if (AEInputCheckTriggered(AEVK_Q) || 0 == AESysDoesWindowExist())
        {
            // If the window close button has been clicked, set the game state to quit
            currentState = MENU;
            transition::isTransitionActive = true;
            transition::resetTimer();
        }
    }

    AudioManager::Update();
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


        AEGfxTextureSet(MMorbit_halo_tex, 0, 0);
        AEGfxSetTransform(MMplanet.orbit_halo_transform.m);
        AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);

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

        // ====================
        //  DRAWING PROJECTILE
        // ====================
        for (int i = 0; i < MMProj_vector.size(); ++i) {

            PlayerProj& player_proj = MMProj_vector[i];

            AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);

            AEGfxTextureSet(MMplayer_proj_tex, 0, 0);
            AEGfxSetTransform(player_proj.transform.m);
            AEGfxMeshDraw(pMeshObj, AE_GFX_MDM_TRIANGLES);
        }

        // =====================
        //  DRAWING TRACTOR BEAM
        // =====================
        if (AEInputCheckCurr(AEVK_SPACE) && MMplayer.state == PLAYER_ORBIT)
        {
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

        if (currentState == MENU)
        {
            // ====================
            //  DRAWING MENU BUTTONS
            // ====================
            menuButtons.draw(pMeshMM);
        }

        else if (currentState == HOW_TO_PLAY)
        {
            if (wKeyPressed)
            {
                RenderMMBackground.RenderSprite(MM_Keys_W_ACTIVE, w_ButtonX, w_ButtonY, w_ButtonWidth, w_ButtonHeight, pMeshMM);
            }
            else
            {
                RenderMMBackground.RenderSprite(MM_Keys_W, w_ButtonX, w_ButtonY, w_ButtonWidth, w_ButtonHeight, pMeshMM);
            }

            if (aKeyPressed)
            {
                RenderMMBackground.RenderSprite(MM_Keys_A_ACTIVE, a_ButtonX, a_ButtonY, a_ButtonWidth, a_ButtonHeight, pMeshMM);
            }
            else
            {
                RenderMMBackground.RenderSprite(MM_Keys_A, a_ButtonX, a_ButtonY, a_ButtonWidth, a_ButtonHeight, pMeshMM);
            }

            if (sKeyPressed)
            {
                RenderMMBackground.RenderSprite(MM_Keys_S_ACTIVE, s_ButtonX, s_ButtonY, s_ButtonWidth, s_ButtonHeight, pMeshMM);
            }
            else
            {
                RenderMMBackground.RenderSprite(MM_Keys_S, s_ButtonX, s_ButtonY, s_ButtonWidth, s_ButtonHeight, pMeshMM);
            }

            if (dKeyPressed)
            {
                RenderMMBackground.RenderSprite(MM_Keys_D_ACTIVE, d_ButtonX, d_ButtonY, d_ButtonWidth, d_ButtonHeight, pMeshMM);
            }
            else
            {
                RenderMMBackground.RenderSprite(MM_Keys_D, d_ButtonX, d_ButtonY, d_ButtonWidth, d_ButtonHeight, pMeshMM);
            }

            if (spacebarActivated)
            {
                RenderMMBackground.RenderSprite(MM_Keys_Spacebar_ACTIVE, spacebar_ButtonX, spacebar_ButtonY, spacebar_ButtonWidth, spacebar_ButtonHeight, pMeshMM);
            }
            else
            {
                RenderMMBackground.RenderSprite(MM_Keys_Spacebar, spacebar_ButtonX, spacebar_ButtonY, spacebar_ButtonWidth, spacebar_ButtonHeight, pMeshMM);
            }

            if (LMB_KeyPressed)
            {
                RenderMMBackground.RenderSprite(MM_LMB_ACTIVE, mouseTexX, mouseTexY, mouseWidth, mouseHeight, pMeshMM);
            }
            else
            {
                RenderMMBackground.RenderSprite(MM_LMB, mouseTexX, mouseTexY, mouseWidth, mouseHeight, pMeshMM);
            }
        }
    }
}

void main_menu::free()
{
    menuButtons.free();

    MMplanet.debris_vector.clear();
    AEGfxMeshFree(pMeshMMBackground);
    AEGfxMeshFree(pMeshMM);
    AEGfxMeshFree(pMeshObj);
    MMPlayerData.clear();
    MMPlayerDataMap.clear();
    MMPlanetData.clear();
    MMPlanetDataMap.clear();
    MMShuttleData.clear();
    MMShuttleDataMap.clear();

    MMDebrisData.clear();
    MMDebrisDataMap.clear();

    MMProjData.clear();
    MMProjDataMap.clear();

    AudioManager::UnloadAllSounds();
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
    AEGfxTextureUnload(MMplayer_proj_tex);

    AEGfxTextureUnload(MM_Keys_W_ACTIVE);
    AEGfxTextureUnload(MM_Keys_W);
    AEGfxTextureUnload(MM_Keys_A_ACTIVE);
    AEGfxTextureUnload(MM_Keys_A);
    AEGfxTextureUnload(MM_Keys_S_ACTIVE);
    AEGfxTextureUnload(MM_Keys_S);
    AEGfxTextureUnload(MM_Keys_D_ACTIVE);
    AEGfxTextureUnload(MM_Keys_D);
    AEGfxTextureUnload(MM_Keys_Spacebar);
    AEGfxTextureUnload(MM_Keys_Spacebar_ACTIVE);
    AEGfxTextureUnload(MM_LMB_ACTIVE);
    AEGfxTextureUnload(MM_LMB);

}

// ===========================
//  SPAWN SHUTTLE IN MAIN MENU
// ===========================
void MMspawn_shuttle()
{
    Shuttles new_shuttle{};

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

std::vector<Debris> MM_create_debris(f32 planet_x, f32 planet_y, double size, int total_debris) {
    std::vector<Debris> new_debris_vector;
    for (int i = 0; i < total_debris; i++)
    {
        Debris new_debris;

        new_debris.id = i + 1;
        new_debris.angle = i * (2 * PI / static_cast<f32>(total_debris));
        new_debris.size = static_cast<f32>(rand() % (MAX_DEBRIS_SIZE - MIN_DEBRIS_SIZE) + MIN_DEBRIS_SIZE);
        new_debris.position.x = static_cast<f32>(planet_x + ((size / 2) + 20) * AECos(AEDegToRad(new_debris.angle)));
        new_debris.position.y = static_cast<f32>(planet_y + ((size / 2) + 20) * AESin(AEDegToRad(new_debris.angle)));
        new_debris.turning_speed = SPEED_DEBRIS;
        new_debris.active = true;
        new_debris.state = ORBIT_AROUND_PLANET;
        new_debris.distance = static_cast<f32>(OUTERRIM_TO_DEBRIS);

        new_debris.scale = { 0 };
        new_debris.rotate = { 0 };
        new_debris.translate = { 0 };
        new_debris.transform = { 0 };

        new_debris.explosion.height = EXPLOSION_HEIGHT;
        new_debris.explosion.width = EXPLOSION_WIDTH;
        new_debris.explosion.transform = { 0 };
        new_debris.explosion.total_time = 1;

        new_debris_vector.push_back(new_debris);
    }

    return new_debris_vector;
}

void MMspawn_debris_shuttle(AEVec2 position, int num_of_debris) {
    for (int i = 0; i < num_of_debris; i++) {
        Debris new_debris;
        new_debris.size = static_cast<f32>(rand() % (MAX_DEBRIS_SIZE - MIN_DEBRIS_SIZE) + MIN_DEBRIS_SIZE);
        new_debris.angle = static_cast<f32>(rand() % static_cast<int>(new_debris.size));
        new_debris.position.x = position.x + (-(rand() % (MAX_BUFFER - MIN_BUFFER)) + new_debris.size);
        new_debris.position.y = position.y + (-(rand() % (MAX_BUFFER - MIN_BUFFER)) + new_debris.size);
        new_debris.id = static_cast<int>(MMplanet.debris_vector.size() + 1);
        new_debris.turning_speed = SPEED_DEBRIS;
        new_debris.active = true;
        new_debris.state = MOVE_TOWARDS_PLANET;

        new_debris.scale = { 0 };
        new_debris.rotate = { 0 };
        new_debris.translate = { 0 };
        new_debris.transform = { 0 };
        new_debris.distance = static_cast<f32>(OUTERRIM_TO_DEBRIS + (rand() % MIN_BUFFER));

        //EXPLOSION
        new_debris.explosion.height = EXPLOSION_HEIGHT;
        new_debris.explosion.width = EXPLOSION_WIDTH;
        new_debris.explosion.transform = { 0 };
        new_debris.explosion.total_time = 1;
        MMplanet.debris_vector.push_back(new_debris);
    }

}
