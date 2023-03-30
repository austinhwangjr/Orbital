/******************************************************************************/
/*!
\file		Shuttle.cpp
\author		Tan Rui An Ryan
\par		email: ruianryan.t\@digipen.edu
\date		March 28, 2023
\brief		This file contains the definition of functions for the shuttle.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "Shuttle.h"
#include "WaveManager.h"
#include <cmath>
#include "Data.h"

static float SHUTTLE_MAX_LIFESPAN;		// Maximum life time for a shuttle before escaping (expiring)
static float SHUTTLE_MAX_ACCEL;			// Maximum acceleration for a shuttle
static int	 SHUTTLE_VALUE;				// Credit value for a shuttle
static int	 SHUTTLE_WIDTH;				// Shuttle Width
static int	 SHUTTLE_HEIGHT;			// Shuttle Height

AEGfxTexture* shuttle_tex;
std::vector<Shuttles> shuttle_vector;


// IMPORT DATA VECTOR
std::map<std::string, f32> 	ShuttleDataMap;
std::vector<Data> 			ShuttleData;


extern WaveManager wave_manager;

void Shuttles::load()
{
	shuttle_tex = AEGfxTextureLoad("Assets/MainLevel/ml_Shuttle.png");
	ImportDataFromFile("Assets/GameObjectData/ShuttleData.txt", ShuttleData, ShuttleDataMap);
}

void Shuttles::init()
{
	SHUTTLE_MAX_LIFESPAN = ShuttleDataMap["Shuttle_Lifespan"];
	SHUTTLE_MAX_ACCEL	 = ShuttleDataMap["Shuttle_Acceleration"];
	SHUTTLE_VALUE		 = static_cast<int>(ShuttleDataMap["Shuttle_Value"]);
	SHUTTLE_WIDTH		 = ShuttleDataMap["Shuttle_Width"];
	SHUTTLE_HEIGHT		 = ShuttleDataMap["Shuttle_Height"];
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

	if (next_state != GS_RESTART) {
		ShuttleData.clear();
		ShuttleDataMap.clear();
	}
}

void Shuttles::unload()
{
	AEGfxTextureUnload(shuttle_tex);
}

void Shuttles::spawn(int const& planet_id, f32 const& rand_angle)
{
	Shuttles new_shuttle;

	new_shuttle.lifespan = SHUTTLE_MAX_LIFESPAN;
	new_shuttle.acceleration = SHUTTLE_MAX_ACCEL;

	new_shuttle.active = true;

	AEVec2Zero(&new_shuttle.velocity);

	new_shuttle.position.x = planet_vector[planet_id].shuttle_spawn_pos.x;
	new_shuttle.position.y = planet_vector[planet_id].shuttle_spawn_pos.y;

	new_shuttle.direction.x = AECos(rand_angle);
	new_shuttle.direction.y = AESin(rand_angle);

	new_shuttle.planet_id = planet_id;

	AEMtx33Scale(&new_shuttle.scale, SHUTTLE_WIDTH, SHUTTLE_HEIGHT);
	AEMtx33Rot(&new_shuttle.rotate, PI / 2 + rand_angle);

	shuttle_vector.push_back(new_shuttle);
}