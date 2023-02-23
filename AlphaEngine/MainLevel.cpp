/* Start Header ************************************************************************/
/*!
\file		Level1.cpp
\author		Hwang Jing Rui, Austin, jingruiaustin.hwang, 2200601
\par		jingruiaustin.hwang\@digipen.edu
\date		Jan 14, 2023
\brief		This file contains the definition of the Load(), Initialize(), Update(), 
			Draw(), Free() and Unload() functions for Level 1.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "pch.h"
#include "GameStateManager.h"
#include "MainLevel.h"
#include <cmath>
#include <iostream>
#include "Planet.h"
#include "Player.h"
#include "PlayerUI.h"
#include "Drone.h"
#include "SpaceStation.h"
#include  "PlayerProj.h"
#include "Shuttle.h"
#include "Debris.h"
#include "Camera.h"
#include "WaveManager.h"

int g_game_running;

extern AEGfxTexture* pTex;
AEGfxTexture* starttest;
s8 fontID;

// String to print
//s8* print_string;

AEGfxVertexList* pMesh;

// test variables
f64 total_time{}, frame_time{};

//bool pause = false;

Player player;
Planets planet;
PlayerUI player_ui;
Drone drone;
SpaceStation space_station;
PlayerProj player_proj;
Camera camera;
extern Debris debris;
Shuttles shuttle;
extern WaveManager wave_manager;

// ----------------------------------------------------------------------------
// This function loads all necessary assets in Level1
// It should be called once before the start of the level
// It loads assets like textures, meshes and music files, etc�
// ----------------------------------------------------------------------------
void main_level::load()
{
	// load texture
	planet.load();
	player.load();
	player_ui.load();
	drone.load();
	space_station.load();
	player_proj.load();
	debris.load();
	shuttle.load();
	wave_manager.load();
	
	//starttest = AEGfxTextureLoad("Assets/start_test.png");

	// Font for text
	//fontID = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 50);
}

// ----------------------------------------------------------------------------
// This function loads all game objects in Level1
// It should be called once after loading the level
// It loads game objects (e.g. player, enemy, ...)
// ----------------------------------------------------------------------------
void main_level::init()
{
	// create mesh
	// Pointer to Mesh
	pMesh = 0;

	planet.init();
	player.init();
	camera.init(player);
	player_ui.init();
	drone.init(player);
	space_station.init();
	player_proj.init();
	shuttle.init();
	debris.init();
	wave_manager.init();

	total_time = 0.0f;

	// Informing the library that we're about to start adding triangles 

	AEGfxMeshStart();

	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	// Saving the mesh (list of triangles) in pMesh 
	pMesh = AEGfxMeshEnd();

	// debugging logs
	AE_ASSERT_MESG(pMesh, "Error: Failed to create pMesh in MainLevel.cpp!");
}

// ----------------------------------------------------------------------------
// This function updates game behaviour in Level1
// It should be called once in the game loop after updating the input status
// It updates game logic/behaviour
// ----------------------------------------------------------------------------

void main_level::update()
{
	// Your own update logic goes here
	frame_time = AEFrameRateControllerGetFrameTime();
	total_time += frame_time;
	
	/*if (AEInputCheckTriggered(AEVK_P)) {
		pause = !pause;
	}*/

	/*if (!pause) {
		planet.update(frame_time);
		player.update(frame_time);
		camera.update(frame_time, player);
		drone.update(frame_time, player, player_ui);
		space_station.update(frame_time, player, player_ui);
		player_proj.update(frame_time, player);
		shuttle.update(frame_time);
		debris.update(frame_time);
		wave_manager.update(frame_time);
	}
	
	else if (pause) {
		planet.update(frame_time / 4);
		player.update(frame_time / 4);
		camera.update(frame_time / 4, player);
		drone.update(frame_time / 4, player, player_ui);
		space_station.update(frame_time / 4, player, player_ui);
		player_proj.update(frame_time / 4, player);
		shuttle.update(frame_time / 4);
		debris.update(frame_time / 4);
		wave_manager.update(frame_time / 4);
	}*/
	
	planet.update(frame_time);
	player.update(frame_time);
	camera.update(frame_time, player);
	player_ui.update(player);
	drone.update(frame_time, player, player_ui);
	space_station.update(frame_time, player, player_ui);
	player_proj.update(frame_time, player);
	shuttle.update(frame_time, player);
	debris.update(frame_time);
	wave_manager.update(frame_time);
	
	

	// Testing
	/*
	if (AEInputCheckTriggered(AEVK_R))
		next_state = GS_RESTART;
	*/
	

	// check if forcing the application to quit
	if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
		next_state = GS_QUIT;
		//gGameRunning = 0;
}

// ----------------------------------------------------------------------------
// This function renders game objects in Level1
// It should be called once after updating the level
// It renders game objects (e.g. player, enemy, ...)
// ----------------------------------------------------------------------------
void main_level::draw()
{
	// Your own rendering logic goes here

	// TEST START
	// Set the background to black. 
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Tell the engine to get ready to draw something with texture. 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the tint to white, so that the sprite can 
	// display the full range of colors (default is black). 
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND 
	// This will allow transparency. 
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	planet.draw(pMesh);
	player.draw(pMesh);
	player_proj.draw(pMesh);
	debris.draw(pMesh);
	shuttle.draw(pMesh);
	wave_manager.draw(pMesh);
	player_ui.draw(pMesh, player);
	drone.draw(pMesh, player_ui);
	space_station.draw(pMesh, player_ui);
}

// ----------------------------------------------------------------------------
// This function cleans the game objects instances in Level1
// It should be called once after the game loop ends
// ----------------------------------------------------------------------------
void main_level::free()
{
	planet.free();
	player.free();
	player_ui.free();
	drone.free();
	space_station.free();
	player_proj.free();
	shuttle.free();
	debris.free();
	wave_manager.free();

	AEGfxMeshFree(pMesh);
}

// ----------------------------------------------------------------------------
// This function frees the memory allocated in Level1
// It should be called once when the state is to be terminated
// ----------------------------------------------------------------------------
void main_level::unload()
{
	planet.unload();
	player.unload();
	player_ui.unload();
	drone.unload();
	space_station.unload();
	player_proj.unload();
	shuttle.unload();
	debris.unload();
	wave_manager.unload();
}