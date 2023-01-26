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

int g_game_running;

AEGfxTexture* pTex;
AEGfxTexture* playerTex;
AEGfxTexture* starttest;
s8 fontID;

// String to print
//s8* print_string;

AEGfxVertexList* pMesh;

// test variables
Player player;
AEVec2 player_pos;
// f32 planet_x, planet_y;
f32 secondplanet_x, secondplanet_y;

f32 radius;
f32 angle;
f32 player_speed_scale;

bool free_fly;
bool can_leave_orbit;

f64* pTime;
f64 total_time{}, frame_time{};

int wave;

// ----------------------------------------------------------------------------
// This function loads all necessary assets in Level1
// It should be called once before the start of the level
// It loads assets like textures, meshes and music files, etc�
// ----------------------------------------------------------------------------
void main_level::load()
{
	// load texture
	pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	playerTex = AEGfxTextureLoad("Assets/test-player.png");
	starttest = AEGfxTextureLoad("Assets/start_test.png");

	// Font for text
	fontID = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 50);
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

	// Ryan's stuff

	planet::init();

	total_time = 0.0;
	pTime = nullptr;

	wave = 0;

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

	// variables
	//planet_x = 100.f, planet_y = 100.f;
	secondplanet_x = 200.f, secondplanet_y = 250.f;

	player.x_pos = 100.f;
	player.y_pos = 100.f;

	radius = 75.f;
	angle = 0.f;
	player_speed_scale = 5.f;

	free_fly = false;
	can_leave_orbit = true;
}

// ----------------------------------------------------------------------------
// This function updates game behaviour in Level1
// It should be called once in the game loop after updating the input status
// It updates game logic/behaviour
// ----------------------------------------------------------------------------

void add_wave() {
	total_time = 0;
	wave++;
}

void main_level::update()
{
	// Informing the system about the loop's start
	AESysFrameStart();

	// Handling Input
	AEInputUpdate();

	// Your own update logic goes here
	frame_time = AEFrameRateControllerGetFrameTime();
	total_time += frame_time;

	std::cout << total_time << '\n';

	planet::update(frame_time);

	player_pos.x = player.x_pos;
	player_pos.y = player.y_pos;

	AEVec2 planet_pos;
	planet_pos.x = 1;
	planet_pos.y = 0;

	if (player.x_pos <= AEGfxGetWinMinX() - 10) player.x_pos = AEGfxGetWinMinX() - 10;
	else if (player.x_pos >= AEGfxGetWinMaxX() + 10) player.x_pos = AEGfxGetWinMaxX() + 10;
	if (player.y_pos <= AEGfxGetWinMinY() - 10) player.y_pos = AEGfxGetWinMinY() - 10;
	else if (player.y_pos >= AEGfxGetWinMaxY() + 10) player.y_pos = AEGfxGetWinMaxY() + 10;

	if (free_fly == false) {
		player.x_pos = radius * AECos(AEDegToRad(angle));
		player.y_pos = radius * AESin(AEDegToRad(angle));

		if (AEInputCheckCurr(AEVK_A)) {

			angle += 3.f;
		}

		if (AEInputCheckCurr(AEVK_D)) {

			angle -= 3.f;
		}

		if (AEInputCheckPrev(AEVK_W)) {
			can_leave_orbit = false;
		}
		else can_leave_orbit = true;
		if (AEInputCheckCurr(AEVK_W) && can_leave_orbit) {

			player.x_pos *= 1.6f;
			player.y_pos *= 1.6f;

			free_fly = true;
		}
	}

	else if (free_fly) {

		AEVec2 player_dir_vector;
		player_dir_vector.x = AECos(AEDegToRad(angle));
		player_dir_vector.y = AESin(AEDegToRad(angle));
		AEVec2Normalize(&player_dir_vector, &player_dir_vector);

		if (AEInputCheckCurr(AEVK_W)) {
			player.x_pos += player_dir_vector.x * player_speed_scale;
			player.y_pos += player_dir_vector.y * player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_A)) {

			angle += player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_S)) {
			player.x_pos -= player_dir_vector.x * player_speed_scale;
			player.y_pos -= player_dir_vector.y * player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_D)) {

			angle -= player_speed_scale;
		}

		if (AEVec2Distance(&planet_pos, &player_pos) <= radius) {
			angle = AERadToDeg(atan2(player.y_pos, player.x_pos));
			free_fly = false;
		}
	}

	// Add new wave
	std::cout << "Time: " << total_time << ", current wave: " << wave << '\n';

	//print_string = "Time";

	if (total_time >= 5) {
		add_wave();
	}

	if (AEInputCheckTriggered(AEVK_R))
		next_state = GS_RESTART;
	

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

	// Set the texture to pTex 
	AEGfxTextureSet(pTex, 0, 0);

	planet::draw(pMesh);

	// Create a scale matrix that scales by 100 x and y 
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 100.f, 100.f);

	// Create a rotation matrix that rotates by 45 degrees 
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, PI / 4);

	// Create a translation matrix that translates by 
	// 100 in the x-axis and 100 in the y-axis 
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, 0.f, 0.f);

	// Concat the matrices (TRS) 
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	// Choose the transform to use 
	AEGfxSetTransform(transform.m);

	// Actually drawing the mesh
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	/*--------------------------------------------------------------------------*/
	// 'Player' Orbit around test planet
	AEGfxTextureSet(playerTex, 0, 0);
	AEMtx33Scale(&scale, 20.f, 20.f);
	AEMtx33Rot(&rotate, AEDegToRad(angle) + PI / 2);
	AEMtx33Trans(&translate, player.x_pos, player.y_pos);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);


	// DRAW TEXT
	//AEGfxPrint(fontID, print_string, 0.5f, 0.5f, 1.f, 1.f, 1.f, 1.f);
	/*--------------------------------------------------------------------------*/
	// TEST END

	// 'Player' Orbit around test planet
	/*AEGfxTextureSet(starttest, 0, 0);
	AEMtx33Scale(&scale, 100.f, 100.f);
	AEMtx33Rot(&rotate, AEDegToRad(angle) + PI / 2);
	AEMtx33Trans(&translate, player_pos.x, player_pos.y);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/

	// Informing the system about the loop's end
	AESysFrameEnd();
}

// ----------------------------------------------------------------------------
// This function cleans the game objects instances in Level1
// It should be called once after the game loop ends
// ----------------------------------------------------------------------------
void main_level::free()
{
	planet::free();

	AEGfxMeshFree(pMesh);
}

// ----------------------------------------------------------------------------
// This function frees the memory allocated in Level1
// It should be called once when the state is to be terminated
// ----------------------------------------------------------------------------
void main_level::unload()
{
	planet::unload();

	// free texture (TEXT)
	AEGfxTextureUnload(pTex);
	AEGfxTextureUnload(playerTex);
}