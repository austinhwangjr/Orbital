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
#include "PlayerProj.h"
#include "Shuttle.h"
#include "Debris.h"
#include "Camera.h"
#include "WaveManager.h"
#include "Global.h"
#include "Graphics.h"
#include "PauseMenu.h"
#include "PauseMenuButtons.h"

//yy debugging and cleaning code
AEGfxTexture* TexMLBackground = nullptr;
AEGfxTexture* TexHeadsUpDisplay = nullptr;

AEGfxVertexList* pMeshMLBackground;				// Background Mesh
AEGfxVertexList* pMeshML;						// Object square mesh

// class declearation 
Rendering RenderMLBackground;
Rendering createMesh1;
Player player{};
Planets planet{};
PlayerUI player_ui{};
Drone drone{};
SpaceStation space_station{};
PlayerProj player_proj{};
Camera camera{};
Debris debris{};
Shuttles shuttle{};
WaveManager wave_manager{};
pause_menu pause{};

//PauseMenu pause_menu;
//PauseMenuButtons pause_menu_buttons;

// test variables (who did this? -yy)
f64 total_time{}, frame_time{};

bool is_paused = false;

namespace main_level
{
	// Keep track of the previous and current game states
	GS_STATES previous_state = GS_MAINLEVEL;
	GS_STATES current_state = GS_MAINLEVEL;
}

// ----------------------------------------------------------------------------
// This function loads all necessary assets in Level1
// It should be called once before the start of the level
// It loads assets like textures, meshes and music files, etc�
// ----------------------------------------------------------------------------
void main_level::load()
{
	TexMLBackground = AEGfxTextureLoad("MainMenu/mm_Background.png");
	//TexHeadsUpDisplay = AEGfxTextureLoad("Assets/MainLevel/ml_HeadsUpDisplay.png");

	pause.load();

	// load texture
	planet.load();
	player.load();
	space_station.load();
	player_ui.load();
	drone.load();
	
	player_proj.load();
	debris.load();
	shuttle.load();
	wave_manager.load();
}

// ----------------------------------------------------------------------------
// This function loads all game objects in Level1
// It should be called once after loading the level
// It loads game objects (e.g. player, enemy, ...)
// ----------------------------------------------------------------------------
void main_level::init()
{
	srand(69);

	// Set the camera position to (0, 0) for the background mesh
	AEGfxSetCamPosition(0.f, 0.f);

	createMesh1.BackgroundMesh(pMeshMLBackground);

	createMesh1.IGSquareMesh(pMeshML);

	pause.init();

	planet.init();
	player.init();
	space_station.init();
	camera.init(player);
	player_ui.init();
	drone.init(player);
	
	player_proj.init();
	shuttle.init();
	debris.init();


	AE_ASSERT_MESG(pMeshML, "Error: Failed to create pMeshML in MainLevel.cpp!");

	std::cout << std::endl;
	std::cout << "------------------------- Main Level Initialised -------------------------" << std::endl;

	wave_manager.init();
	total_time = 0.0f;
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

	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		if (!is_paused)
		{
			previous_state = current_state;
			
		}
		else
		{
			current_state = previous_state;
		}
		is_paused = !is_paused;
	}

	if (!is_paused)
	{
		planet.update(frame_time);
		player.update(frame_time);
		space_station.update(frame_time, player, player_ui);
		player_proj.update(frame_time, player, player_ui);
		camera.update(frame_time, player);
		player_ui.update(frame_time, player);
		drone.update(frame_time, player, player_ui);
		
		shuttle.update(frame_time, player);
		debris.update(frame_time);
		wave_manager.update(frame_time);
	}
	else if (is_paused)
	{
		pause.update();
	}

	// Testing
	//if (AEInputCheckTriggered(AEVK_R))
	//	next_state = GS_RESTART;
	//

	if (AEInputCheckTriggered(AEVK_F11))
	{
		// If the window close button has been clicked, set the game state to quit
		Global_ToggleScreen();
		std::cout << "Toggling Screen " << std::endl;
	}

	if (AEInputCheckTriggered(AEVK_T))
		next_state = GS_QUIT;

	//// check if forcing the application to quit
	//if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
	//{
	//	next_state = GS_MAINMENU;
	//}
}

// ----------------------------------------------------------------------------
// This function renders game objects in Level1
// It should be called once after updating the level
// It renders game objects (e.g. player, enemy, ...)
// ----------------------------------------------------------------------------
void main_level::draw()
{
	// Set the background to black.
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	RenderMLBackground.RenderSprite(TexMLBackground, 0.f, 0.f, 4800.f, 2700.f, pMeshMLBackground);		//drawbackground for Mainlevel

	// Tell the engine to get ready to draw something with texture. 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the tint to white, so that the sprite can 
	// display the full range of colors (default is black). 
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND 
	// This will allow transparency. 
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);


		planet.draw(pMeshML);
		player.draw(pMeshML);
		space_station.draw(pMeshML, player_ui);
		player_ui.draw(pMeshML, player);
		player_proj.draw(pMeshML);
		debris.draw(pMeshML);
		shuttle.draw(pMeshML);
		wave_manager.draw();

		drone.draw(pMeshML, player_ui);

	if (is_paused)
	{
		pause.draw(camera.position);
	}
}

// ----------------------------------------------------------------------------
// This function cleans the game objects instances in Level1
// It should be called once after the game loop ends
// ----------------------------------------------------------------------------
void main_level::free()
{
	planet.free();
	player.free();
	space_station.free();
	player_ui.free();
	drone.free();

	player_proj.free();
	shuttle.free();
	debris.free();

	wave_manager.free();

	if (is_paused)
	{
		pause.free();
	}

		pause.free();

	AEGfxMeshFree(pMeshML);
	AEGfxMeshFree(pMeshMLBackground);
}

// ----------------------------------------------------------------------------
// This function frees the memory allocated in Level1
// It should be called once when the state is to be terminated
// ----------------------------------------------------------------------------
void main_level::unload()
{
	planet.unload();
	player.unload();
	space_station.unload();
	drone.unload();
	
	player_ui.unload();
	player_proj.unload();
	shuttle.unload();
	debris.unload();
	wave_manager.unload();
	pause.unload();

	AEGfxTextureUnload(TexMLBackground); // unload the texture for the background image

}