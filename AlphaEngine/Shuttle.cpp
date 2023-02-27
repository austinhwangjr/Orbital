/* Start Header ************************************************************************/
/*!
\file		Shuttle.cpp
\author		Tan Rui An Ryan, ruianryan.t, 2200600
\par		ruianryan.t\@digipen.edu
\date		Feb 27, 2023
\brief		This file contains the behaviour for the Shuttle game object

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "AEEngine.h"
#include "Shuttle.h"
#include "WaveManager.h"
#include <cmath>

AEGfxTexture* shuttle_tex;
std::vector<Shuttles> shuttle_vector;

extern WaveManager wave_manager;

void Shuttles::load()
{
	shuttle_tex = AEGfxTextureLoad("Assets/test-shuttle.png");
}

void Shuttles::init()
{

}

void Shuttles::update(f64 frame_time, Player& player)
{
	for (size_t i{}; i < shuttle_vector.size(); i++)
	{
		if (shuttle_vector[i].active)
		{
			AEVec2 added{};

			// Shuttle accelerating
			AEVec2Add(&added, &added, &shuttle_vector[i].direction);
			AEVec2Scale(&added, &added, shuttle_vector[i].acceleration * static_cast<f32>(frame_time));
			AEVec2Add(&shuttle_vector[i].velocity, &added, &shuttle_vector[i].velocity);

			// Limiting shuttle velocity
			AEVec2Scale(&shuttle_vector[i].velocity, &shuttle_vector[i].velocity, 0.99f);

			// Update shuttle position
			shuttle_vector[i].position.x += shuttle_vector[i].velocity.x * static_cast<f32>(frame_time);
			shuttle_vector[i].position.y += shuttle_vector[i].velocity.y * static_cast<f32>(frame_time);
			
			AEMtx33Trans(&shuttle_vector[i].translate, shuttle_vector[i].position.x, shuttle_vector[i].position.y);
			AEMtx33Concat(&shuttle_vector[i].transform, &shuttle_vector[i].rotate, &shuttle_vector[i].scale);
			AEMtx33Concat(&shuttle_vector[i].transform, &shuttle_vector[i].translate, &shuttle_vector[i].transform);

			// If shuttle escapes
			if (shuttle_vector[i].lifespan <= 0)
			{
				wave_manager.shuttle_has_escaped = true;
				wave_manager.shuttle_left_planet++;
				player.credits += SHUTTLE_VALUE;
				shuttle_vector[i].active = false;
				//spawn_debris(2, shuttle_vector[i].planet_id);
				spawn_debris_shuttle(shuttle_vector[i].position, shuttle_vector[i].planet_id, 3);
			}
			shuttle_vector[i].lifespan -= static_cast<f32>(frame_time);
		}
	}

	
}

void Shuttles::draw(AEGfxVertexList* pMesh)
{
	// Set the texture to pTex 
	AEGfxTextureSet(shuttle_tex, 0, 0);

	for (size_t i{}; i < shuttle_vector.size(); i++)
	{
		if (shuttle_vector[i].active)
		{
			if (shuttle_vector[i].lifespan <= SHUTTLE_MAX_LIFESPAN / 2.f)
			{
				AEGfxSetTransparency(shuttle_vector[i].lifespan / (SHUTTLE_MAX_LIFESPAN / 2.0f));
			}
			else
			{
				AEGfxSetTransparency(1.f);
			}
			AEGfxSetTransform(shuttle_vector[i].transform.m);
			// Actually drawing the mesh
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	AEGfxSetTransparency(1.0f);
}

void Shuttles::free()
{
	shuttle_vector.clear();
}

void Shuttles::unload()
{
	AEGfxTextureUnload(shuttle_tex);
}

void Shuttles::spawn(int const& planet_id)
{
	Shuttles new_shuttle;

	new_shuttle.lifespan = SHUTTLE_MAX_LIFESPAN;
	new_shuttle.acceleration = SHUTTLE_MAX_ACCEL;

	new_shuttle.active = true;

	AEVec2Zero(&new_shuttle.velocity);

	new_shuttle.position.x = planet_vector[planet_id].shuttle_spawn_pos.x;
	new_shuttle.position.y = planet_vector[planet_id].shuttle_spawn_pos.y;

	f32 rand_angle = AERandFloat() * (2 * PI);
	new_shuttle.direction.x = cos(rand_angle);
	new_shuttle.direction.y = sin(rand_angle);

	new_shuttle.planet_id = planet_id;

	AEMtx33Scale(&new_shuttle.scale, 20.f, 20.f);
	AEMtx33Rot(&new_shuttle.rotate, PI / 2 + rand_angle);

	shuttle_vector.push_back(new_shuttle);
}