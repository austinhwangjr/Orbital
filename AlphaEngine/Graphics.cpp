#include "AEEngine.h"
#include "GameObjects.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "Graphics.h"
#include "SplashScreen.h"



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
