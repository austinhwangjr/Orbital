// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"



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
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	//TEST START
	// create mesh
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
	pMesh = AEGfxMeshEnd();

	// load texture
	AEGfxTexture* pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");

	f32 planet_x = 100.f, planet_y = 100.f;

	// TEST END

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Your own update logic goes here
		
		if (AEInputCheckCurr(AEVK_W)) {

			planet_y += 5.f;
		}

		if (AEInputCheckCurr(AEVK_A)) {

			planet_x -= 5.f;
		}

		if (AEInputCheckCurr(AEVK_S)) {

			planet_y -= 5.f;
		}

		if (AEInputCheckCurr(AEVK_D)) {

			planet_x += 5.f;
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
		AEMtx33Trans(&translate, planet_x, planet_y);
		
		// Concat the matrices (TRS) 
		AEMtx33 transform = { 0 }; 
		AEMtx33Concat(&transform, &rotate, &scale); 
		AEMtx33Concat(&transform, &translate, &transform);
		
		// Choose the transform to use 
		AEGfxSetTransform(transform.m);
		
		// Actually drawing the mesh
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

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

	// free the system
	AESysExit();
}