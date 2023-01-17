// ---------------------------------------------------------------------------
// includes

#include "GameObjects.h"

#include "pch.h"
#include "GameStateManager.h"
#include "Systems.h"
#include "Input.h"



// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	system_call::init(hInstance, nCmdShow);																		// Initialize the system

	//gsm::init(GS_MAINMENU);
	gsm::init(GS_MAINLEVEL);																					// Initialize the Game State Manager (GSM) with Level1 as the initial game state

	// While the current game state is not equal to the quit state
	while (current_state != GS_QUIT)
	{
		// If the current game state is not equal to the restart state
		if (current_state != GS_RESTART)
		{
			gsm::update();       // Call Update() to update game state manager
			fpLoad();           // Call Load() to load current game state
		}
		else
		{
			next_state = previous_state;    // Set next game state to be equal to previous game state
			current_state = previous_state; // Set current game state to be equal to previous game state
		}

		fpInit();         // Initialize current game state

		// The game loop (while the next game state is equal to the current state)
		while (next_state == current_state)
		{
			input_handle();     // Update input status
			fpUpdate();         // Update current game state
			fpDraw();           // Render current game state
		}

		fpFree();               // Free current game state

		// If next game state is not equal to restart game state
		if (next_state != GS_RESTART)
		{
			fpUnload();         // Unload() current game state
		}

		previous_state = current_state;     // Set previous game state to be equal to current game state
		current_state = next_state;         // Set current game state to be equal to next game state
	}

	system_call::unload();              // Systems exit (terminate)













	

	

	//TEST START
	/*// create mesh
	// Pointer to Mesh
	AEGfxVertexList * pMesh = 0;
	
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
	pMesh = AEGfxMeshEnd();*/

	// load texture
	/*AEGfxTexture* pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	AEGfxTexture* playerTex = AEGfxTextureLoad("Assets/test-player.png");*/

	/*f32 planet_x = 100.f, planet_y = 100.f;
	f32 secondplanet_x = 200.f, secondplanet_y = 250.f;

	f32 radius = 75.f;
	f32 angle = 0.f;
	f32 player_speed_scale = 5.f;

	AEVec2 player_pos;
	player_pos.x = planet_x;
	player_pos.y = planet_y;

	bool free_fly = false;
	bool can_leave_orbit = true;*/


	// TEST END

	// Game Loop
	/*while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Your own update logic goes here
		
		AEVec2 planet_pos;
		planet_pos.x = 1;
		planet_pos.y = 0;

		if (player_pos.x <= AEGfxGetWinMinX() - 10) player_pos.x = AEGfxGetWinMinX() - 10;
		else if (player_pos.x >= AEGfxGetWinMaxX() + 10) player_pos.x = AEGfxGetWinMaxX() + 10;
		if (player_pos.y <= AEGfxGetWinMinY() - 10) player_pos.y = AEGfxGetWinMinY() - 10;
		else if (player_pos.y >= AEGfxGetWinMaxY() + 10) player_pos.y = AEGfxGetWinMaxY() + 10;

		if (free_fly == false) {
			player_pos.x = radius * AECos(AEDegToRad(angle));
			player_pos.y = radius * AESin(AEDegToRad(angle));

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

				player_pos.x *= 1.6f;
				player_pos.y *= 1.6f;

				free_fly = true;
			}
		}

		else if (free_fly) {

			AEVec2 player_dir_vector;
			player_dir_vector.x = AECos(AEDegToRad(angle));
			player_dir_vector.y = AESin(AEDegToRad(angle));
			AEVec2Normalize(&player_dir_vector, &player_dir_vector);

			if (AEInputCheckCurr(AEVK_W)) {
				player_pos.x += player_dir_vector.x * player_speed_scale;
				player_pos.y += player_dir_vector.y * player_speed_scale;
			}

			if (AEInputCheckCurr(AEVK_A)) {

				angle += player_speed_scale;
			}

			if (AEInputCheckCurr(AEVK_S)) {
				player_pos.x -= player_dir_vector.x * player_speed_scale;
				player_pos.y -= player_dir_vector.y * player_speed_scale;
			}

			if (AEInputCheckCurr(AEVK_D)) {

				angle -= player_speed_scale;
			}

			if (AEVec2Distance(&planet_pos, &player_pos) <= radius) {
				angle = AERadToDeg(atan2(player_pos.y, player_pos.x));
				free_fly = false;
			}
		}


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
		
		// Create a scale matrix that scales by 100 x and y 
		AEMtx33 scale = { 0 }; 
		AEMtx33Scale(&scale, 100.f, 100.f);

		// Create a rotation matrix that rotates by 45 degrees 
		AEMtx33 rotate = { 0 }; 
		AEMtx33Rot(&rotate, PI/4);
		
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
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/

		/*--------------------------------------------------------------------------*/
		/*// 'Player' Orbit around test planet
		AEGfxTextureSet(playerTex, 0, 0);
		AEMtx33Scale(&scale, 20.f, 20.f);
		AEMtx33Rot(&rotate, AEDegToRad(angle) + PI / 2);
		AEMtx33Trans(&translate, player_pos.x, player_pos.y);

		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/
		/*--------------------------------------------------------------------------*/

		// TEST END

		// Informing the system about the loop's end
		/*AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}*/

	// free mesh (TEST)
	/*AEGfxMeshFree(pMesh);

	// free texture (TEXT)
	AEGfxTextureUnload(pTex);
	AEGfxTextureUnload(playerTex);*/

	// free the system
	//AESysExit();
}