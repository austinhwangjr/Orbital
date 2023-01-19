// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "masterlist.h"


// ---------------------------------------------------------------------------
// main


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	
	int gGameRunning = 1;

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("Orbital");

	// reset the system modules
	AESysReset();

	//TEST START
	AEGfxVertexList * pMesh = 0;
	AEGfxMeshStart();
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

	// load texture
	AEGfxTexture* pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	AEGfxTexture* playerTex = AEGfxTextureLoad("Assets/test-player.png");
	AEGfxTexture* debrisTex = AEGfxTextureLoad("Assets/Debris.png");

	f32 planet_x = 100.f, planet_y = 100.f;
	f32 secondplanet_x = 200.f, secondplanet_y = 250.f;

	f32 radius = 90.f;
	f32 angle = 0.f;
	f32 player_speed_scale = 5.f;

	
	/*--------------------------------------------------------------------------*/
	/*
	f32 debris_x = 70.f, debris_y = 70.f;		//starting position for debris spawn
	f32 debris_scale_x = 15.f;					// Scale of debris in x-axis
	f32 debris_scale_y = 15.f;					// Scale of debris in y-axis
	int debris_angle = 1;						// debris rotation angle (rotate on the spot) 
	f32 debris_turning_angle = 0.f;				// debris orbit angle (rotate around the planet)
	AEVec2 debris_pos;							// debris position
	debris_pos.x = debris_x;					
	debris_pos.y = debris_y;
	f32 debris_turning_speed = 0.125;			//debris turning speed
	f32 debris_radius = 70.f;					//debris radius from the planet 
	*/
	/*--------------------------------------------------------------------------*/

	// DEBRIS INITIALISATION

	enum { num_stone = 20};					// change the number of stone u want to produce
	struct debris array[num_stone];
	AEMtx33 debris_array[num_stone];

	double speed = 0.125f;					// Speed of rotation around the planet
	int flag = 1;
	int increase_num = 0;

	for (int i = 0; i < num_stone; i++) {
		array[i].id = i + 1;
		array[i].angle = 1;
		array[i].scale_x = 15.f;
		array[i].scale_y = 15.f;
		array[i].angle = 1;
		array[i].turning_angle = 0.f+(i*50);
		array[i].position.x = 70.f + (i * 5);
		array[i].position.y = 70.f - (i * 5);
		array[i].turning_speed = speed;   
		array[i].dist_from_planet = 70.f;
	}
	
	AEMtx33 debris_scale{};
	AEMtx33 debris_rotate{};
	AEMtx33 debris_translate{};
	AEMtx33 debris_transform{};
	for (int i = 0; i < num_stone; i++) {
		AEMtx33Scale(&debris_scale, array[i].scale_x, array[i].scale_x);
		AEMtx33Rot(&debris_rotate, array[i].angle);
		AEMtx33Trans(&debris_translate, array[i].position.x, array[i].position.y);
		AEMtx33Concat(&debris_transform, &debris_rotate, &debris_scale);
		AEMtx33Concat(&debris_transform, &debris_translate, &debris_transform);
		debris_array[i] = debris_transform;
	}
	/*--------------------------------------------------------------------------*/
	
	

	AEVec2 player_pos;
	player_pos.x = planet_x;
	player_pos.y = planet_y;

	bool free_fly = false;
	bool can_leave_orbit = true;


	// TEST END

	// Game Loop
	while (gGameRunning)
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
		AEMtx33Trans(&translate, player_pos.x, player_pos.y);

		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		/*--------------------------------------------------------------------------*/


		
		/*--------------------------------------------------------------------------*/
		// 'DEBRIS' Orbit around test planet
		for (int i = 0; i < num_stone; i++) {

			if (gGameRunning) {

				array[i].turning_angle -= array[i].turning_speed;
			}

			array[i].position.x = (array[i].dist_from_planet) * AECos(AEDegToRad(array[i].turning_angle));
			array[i].position.y = (array[i].dist_from_planet) * AESin(AEDegToRad(array[i].turning_angle));


			AEGfxTextureSet(debrisTex, 0, 0);
			AEMtx33Scale(&debris_scale, array[i].scale_x, array[i].scale_x);
			AEMtx33Rot(&debris_rotate, array[i].angle);
			AEMtx33Trans(&debris_translate, array[i].position.x, array[i].position.y);
			AEMtx33Concat(&debris_transform, &debris_rotate, &debris_scale);
			AEMtx33Concat(&debris_transform, &debris_translate, &debris_transform);
			debris_array[i] = debris_transform;
			AEGfxSetTransform(debris_array[i].m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			
		}
		/*--------------------------------------------------------------------------*/


		// TEST END

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	// free mesh (TEST)
	AEGfxMeshFree(pMesh);

	// free texture (TEXT)
	AEGfxTextureUnload(pTex);
	AEGfxTextureUnload(playerTex);
	AEGfxTextureUnload(debrisTex);
	// free the system
	AESysExit();
}



