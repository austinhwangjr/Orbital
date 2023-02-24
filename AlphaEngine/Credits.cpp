#include "AEEngine.h"
#include <iostream>
#include "Global.h"
#include "GameStateManager.h"
#include "MainMenu.h"
#include "MenuButtons.h"
#include "Graphics.h"
#include "Credits.h"

AEGfxTexture* TexCreditsBackground = nullptr;
AEGfxVertexList* pMeshCreditsBackground;

Rendering createMesh2;
Rendering RenderCreditsBackground;

void Credits::load()
{
	TexCreditsBackground = AEGfxTextureLoad("Assets/Credits.png");
}

void Credits::init()
{

	createMesh2.BackgroundMesh(pMeshCreditsBackground);

	AE_ASSERT_MESG(pMeshCreditsBackground, "Error: Failed to create pMeshCreditsBackground in MainMenu.cpp!");
}

void Credits::update()
{

}

void Credits::draw()
{
	RenderCreditsBackground.RenderSprite(TexCreditsBackground, 0.f, 0.f, 800.f, 450.f, pMeshCreditsBackground);

}
void Credits::free()
{
	AEGfxMeshFree(pMeshCreditsBackground);

}
void Credits::unload()
{
	AEGfxTextureUnload(TexCreditsBackground); // unload the texture for the background image

}