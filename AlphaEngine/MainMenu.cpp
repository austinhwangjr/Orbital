/*-------------------------------------------------- Main Menu --------------------------------------------------*/
#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"

#include "MainMenu.h"
#include "Global.h"
#include "Systems.h"
#include "iostream"

static float texWidth = 100.0f;
static float texHeight = 50.0f;

AEGfxVertexList* startButtonMesh;
AEGfxTexture* startButtonTex;

void main_menu::load()
{
	startButtonTex = AEGfxTextureLoad("Assets/start_test.png");
}

void main_menu::init()
{
	startButtonMesh = 0;

	// Calculate the size of the texture in screen space
	float texWidth = 100.0f / (float)g_windowWidth * 2.0f;
	float texHeight = 50.0f / (float)g_windowHeight * -2.0f;

	// Quad Mesh Code Start
	AEGfxMeshStart();

	AEGfxTriAdd(
		-texWidth, -texHeight, 0xFFFFFFFF, 0.0f, 1.0f,
		texWidth, -texHeight, 0xFFFFFFFF, 1.0f, 1.0f,
		-texWidth, texHeight, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		texWidth, -texHeight, 0xFFFFFFFF, 1.0f, 1.0f,
		texWidth, texHeight, 0xFFFFFFFF, 1.0f, 0.0f,
		-texWidth, texHeight, 0xFFFFFFFF, 0.0f, 0.0f);

	// Quad Mesh Code End
	startButtonMesh = AEGfxMeshEnd();
}

void main_menu::update()
{
	std::cout << "GameState: " << current_state << std::endl;

	AESysFrameStart();
	AEInputUpdate();

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);

		float screenX = (mouseX / (float)g_windowWidth) * 2.0f - 1.0f;
		float screenY = (mouseY / (float)g_windowHeight) * -2.0f + 1.0f;

		if (screenX > -0.5f && screenX < 0.5f && screenY > -0.5f && screenY < 0.5f)
		{
			// Start button clicked, go to game state
			if (current_state == GS_MAINMENU) {
				next_state = GS_MAINLEVEL;
				std::cout << "GameState changed to: " << current_state << std::endl;
			}
		}
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
		next_state = GS_QUIT;
}





void main_menu::draw()
{
	// Clear the screen
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Set the render mode to texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the tint color to white
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the blend mode to blend
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	// Set the texture to startButtonTex
	AEGfxTextureSet(startButtonTex, 0, 0);

	// Create a scale matrix that scales by 100 x and y 
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, texWidth * 2, texHeight * 2);

	// Create a rotation matrix that rotates by 45 degrees 
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, 0);

	// Create a translation matrix that translates by 
	// 100 in the x-axis and 100 in the y-axis 
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, 0.f, 0.f);

	// Concat the matrices (TRS) 
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	// Set the transform
	AEGfxSetTransform(transform.m);

	// Draw the mesh
	AEGfxMeshDraw(startButtonMesh, AE_GFX_MDM_TRIANGLES);

	// Swap buffers and present the frame
	AESysFrameEnd();
}


void main_menu::free()
{
	AEGfxMeshFree(startButtonMesh);

}

void main_menu::unload()
{
	AEGfxTextureUnload(startButtonTex);
}