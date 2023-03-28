/******************************************************************************/
/*!
\file		SplashScreen.cpp
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the splash screen.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include <string>
#include "pch.h"
#include "GameStateManager.h"
#include "SplashScreen.h"
#include "Global.h"

// Textures
AEGfxTexture* digipen_tex;

// Mesh
AEGfxVertexList* pMeshSC;

// Font
extern s8	  font_id_splashscreen;
std::string	  copyright;

// Temp variables
// =============
// Splashscreen
// =============

AEVec2			ss_position;
f32				ss_width;
f32				ss_height;
f32				ss_timer;
f32 			ss_max_timer;
f32				ss_flag;
f32				ss_max_alpha;
f32				ss_alpha_multiplier;
f64				ss_frame_time{};

// ==========
// Transform
// ==========
AEMtx33			ss_transform;

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void splashscreen::load()
{
	// Load textures
	digipen_tex = AEGfxTextureLoad("Assets/DigiPen_BLACK.png");

	// Create mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	pMeshSC = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshSC, "fail to create object!!");
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void splashscreen::init()
{
	// =============
	// Splashscreen
	// =============

	ss_position.x			= 0.f;
	ss_position.y			= 0.f;

	ss_width				= static_cast<f32>(AEGetWindowWidth()) * 0.8f;
	ss_height				= static_cast<f32>(AEGetWindowWidth()) * 0.4f;

	ss_timer				= 0.f;
	ss_max_timer			= 2.f;

	ss_flag				= 1;

	ss_max_alpha			= 255.f;
	ss_alpha_multiplier	= 0.f;
}

/******************************************************************************/
/*!
	Update Splashscreen
*/
/******************************************************************************/
void splashscreen::update()
{
	ss_frame_time = AEFrameRateControllerGetFrameTime();

	ss_timer += ss_frame_time;

	if (ss_alpha_multiplier <= 255 && ss_flag)
	{
		ss_alpha_multiplier += ss_timer * (ss_max_alpha / ss_max_timer);
	}
	else
	{
		if (ss_flag)
		{
			ss_flag = 0;
		}
		ss_alpha_multiplier -= ss_timer * (ss_max_alpha / ss_max_timer);
	}

	if (ss_alpha_multiplier < 0)
	{
		next_state = GS_MAINLEVEL;
	}

	// ======================================
	// Calculate the matrix for DigiPen Logo
	// ======================================

	AEMtx33 scale, rot, trans;

	AEMtx33Scale(&scale, ss_width, ss_height);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, ss_position.x, ss_position.y);
	AEMtx33Concat(&ss_transform, &rot, &scale);
	AEMtx33Concat(&ss_transform, &trans, &ss_transform);
}

/******************************************************************************/
/*!
	Draw Splashscreen
*/
/******************************************************************************/
void splashscreen::draw()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(digipen_tex, 0, 0);
	AEGfxSetTransform(ss_transform.m);
	AEGfxMeshDraw(pMeshSC, AE_GFX_MDM_TRIANGLES);

	copyright = "All content (C) 2023 DigiPen Institute of Technology Singapore. All Rights Reserved";
	AEGfxPrint(font_id_splashscreen, const_cast<s8*>(copyright.c_str()), -0.45f, -0.8f, 1.f, 1.f, 1.f, 1.f);
}

/******************************************************************************/
/*!
	Clean Object Instances
*/
/******************************************************************************/
void splashscreen::free()
{
	AEGfxMeshFree(pMeshSC);
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void splashscreen::unload()
{
	AEGfxTextureUnload(digipen_tex);
}