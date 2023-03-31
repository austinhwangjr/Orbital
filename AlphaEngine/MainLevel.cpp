/******************************************************************************/
/*!
\file		MainLevel.cpp
\author 	Hwang Jin Rui, Austin
\par    	email: jinruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the main level.


Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "MainLevel.h"

//yy debugging and cleaning code
AEGfxTexture* TexMLBackground = nullptr;

AEGfxVertexList* pMeshMLBackground;				// Background Mesh
AEGfxVertexList* pMeshML;						// Object square mesh

// class declaration 
Rendering		RenderMLBackground;
Rendering		createMesh1;
Player			player{};
PlayerUI		player_ui{};
PlayerProj		player_proj{};
Planets			planet{};
Drone			drone{};
SpaceStation	space_station{};
Debris			debris{};
Shuttles		shuttle{};
Camera			camera{};
WaveManager		wave_manager{};
pause_menu		pause{};

//PauseMenu pause_menu;
//PauseMenuButtons pause_menu_buttons;

bool is_paused = false;

// ----------------------------------------------------------------------------
// This function loads all necessary assets in Level1
// It should be called once before the start of the level
// It loads assets like textures, meshes and music files, etc�
// ----------------------------------------------------------------------------
void main_level::load()
{
	TexMLBackground = AEGfxTextureLoad("Assets/MainLevel/ml_Background.png");
	
	pause.load();

	// load texture
	wave_manager.load();
	planet.load();
	player.load();
	space_station.load();
	player_ui.load();
	drone.load();
	
	player_proj.load();
	debris.load();
	shuttle.load();
	
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
}

// ----------------------------------------------------------------------------
// This function updates game behaviour in Level1
// It should be called once in the game loop after updating the input status
// It updates game logic/behaviour
// ----------------------------------------------------------------------------

void main_level::update()
{
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
		camera.update(g_dt, player);
		planet.update(g_dt);
		player.update(g_dt);
		space_station.update(g_dt, player, player_ui);
		player_proj.update(g_dt, player, player_ui);
		drone.update(g_dt, player, player_ui);

		shuttle.update(g_dt, player);
		debris.update(g_dt);
		player_ui.update(g_dt, player, wave_manager);
		wave_manager.update(g_dt);
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

	// Get camera position
	float camX = camera.position.x;
	float camY = camera.position.y;

	// Calculate the starting point for drawing the background based on the camera position
	float startX = std::floor(camX / backgroundWidth) * backgroundWidth;
	float startY = std::floor(camY / backgroundHeight) * backgroundHeight;

	// Draw the tiled background
	for (float x = startX - backgroundWidth; x < startX + g_windowWidth + backgroundWidth; x += backgroundWidth)
	{
		for (float y = startY - backgroundHeight; y < startY + g_windowHeight + backgroundHeight; y += backgroundHeight)
		{
			RenderMLBackground.RenderSprite(TexMLBackground, x, y, 4800.f, 2700.f, pMeshMLBackground);
		}
	}

	// Tell the engine to get ready to draw something with texture. 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the tint to white, so that the sprite can 
	// display the full range of colors (default is black). 
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND 
	// This will allow transparency. 
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	player.draw(pMeshML);
	planet.draw(pMeshML);

	space_station.draw(pMeshML, player_ui);
		
	player_proj.draw(pMeshML);
	debris.draw(pMeshML);
	shuttle.draw(pMeshML);
	wave_manager.draw(pMeshML);

	drone.draw(pMeshML, player_ui);
	player_ui.draw(pMeshML, player, wave_manager);

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