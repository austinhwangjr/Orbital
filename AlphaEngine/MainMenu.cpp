/*-------------------------------------------------- Main Menu --------------------------------------------------*/
#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"


AEGfxTexture* startTex;

AEGfxVertexList* pMesh1;

//// test variables
//f32 planet_x, planet_y;
//f32 secondplanet_x, secondplanet_y;
//
//f32 radius;
//f32 angle;
//f32 player_speed_scale;
//
//AEVec2 player_pos;

//bool free_fly;
//bool can_leave_orbit;

void main_menu::load()
{
	startTex = AEGfxTextureLoad("Assets/start_test.png");
}

void main_menu::init()
{
	/*main_menu::load();*/

	pMesh1 = 0;
	//Quad Mesh Code Start
	AEGfxMeshStart();

	//AEGfxSetBlendColor(0, 0, 0, 1);

	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//AEGfxTriAdd( //First Triangle
	//	-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
	//	0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
	//	-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	//AEGfxTriAdd( //Second Triangle
	//	0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
	//	0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
	//	-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
	//Quad Mesh Code End
	pMesh1 = AEGfxMeshEnd();
}

void main_menu::update()
{
	AESysFrameStart();
	AEInputUpdate();

	if (AEInputCheckTriggered(AEVK_R))
		next_state = GS_RESTART;


	// check if forcing the application to quit
	if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
		next_state = GS_QUIT;
}

void main_menu::draw()
{
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
	AEGfxTextureSet(startTex, 0, 0);

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
	AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);

	AESysFrameEnd();
}

void main_menu::free()
{
	AEGfxMeshFree(pMesh1);
}

void main_menu::unload()
{
	AEGfxTextureUnload(startTex);

}

//void DrawSprite(AEGfxTexture* texture, float posX, float posY, float degree, float alpha, float xPixelSize, float yPixelSize)
//{
//	if (!texture)
//		return;
//
//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);                  //Start Rendering
//	AEMtx33 transform = { 0 };
//	AEGfxSetTransform(transform.m);
//	
//	AEGfxTextureSet(startTex, 0, 0);                         //Set Texture
//	AEGfxSetTransparency(alpha);                            //Set Alpha
//	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);              //Set Blend Colors
//
//	AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);             //Draw Mesh & Sprite
//
//	//AE_ASSERT_MESG(pMesh1, "ERROR: CANT MAKE %s MESH!", pMesh1);
//}
//
//void DrawMapSprite(float dt)
//{
//
//	DrawSprite(startTex, g_windowWidth /2, g_windowHeight /2, 0, 1, (1080), (1920 * 0.5));
//}
//
