/******************************************************************************/
/*!
\file		Planet.cpp
\author		Tan Rui An Ryan
\par		email: ruianryan.t\@digipen.edu
\date		March 28, 2023
\brief		This file contains the definition of functions for the planet.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include <iostream>
#include <cmath>
#include "AEEngine.h"
#include "Planet.h"
#include "Drone.h"
#include "SpaceStation.h"
#include "WaveManager.h"
#include "Easing.h"
#include "Data.h"

//Textures
AEGfxTexture* planet_tex;
AEGfxTexture* orbit_tex;
AEGfxTexture* runway_tex;
AEGfxTexture* orbit_halo_tex;

std::vector<Planets> planet_vector;
std::vector<Planets::Runway> runway_vector;
std::vector<AEGfxTexture*> planet_textures;

extern WaveManager wave_manager;
extern Shuttles shuttle;
extern Debris debris;
extern std::vector<SpaceStation> space_station_vector;
extern std::vector<std::vector<Drone>> drone_vector_all;

static float PLANET_SIZE;						// Planet base size (radius)	
static float PLANET_ROT_SPEED;					// Planet rotation speed (radians)
static float PLANET_SPAWN_BUFFER;				// Planet spawn distance buffer modifier
static int	 SHUTTLE_SPAWN_TIME_MAX;			// Maximum time shuttles will spawn
static int   SHUTTLE_SPAWN_TIME_MIN;			// Minimum time shuttles will spawn
static int	 DEBRIS_MAX;						// Maximum number of debris on a planet
static int	 DEBRIS_MIN;						// Minimum number of debris on a planet
static int   DRONES_MAX;						// Maximum number of drones on a planet
static float RUNWAY_LIFESPAN;					// Time taken for runway arrow to reset
static float RUNWAY_MAX_ACCEL;					// Maximum acceleration value for runway arrow


// IMPORT DATA VECTOR
std::map<std::string, f32> 	PlanetDataMap;
std::vector<Data> 			PlanetData;


/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void Planets::load()
{
	orbit_tex = AEGfxTextureLoad("Assets/MainLevel/ml_OrbitRing.png");
	runway_tex = AEGfxTextureLoad("Assets/MainLevel/ml_arrow.png");
	orbit_halo_tex = AEGfxTextureLoad("Assets/MainLevel/neonCircle.png");
	ImportDataFromFile("Assets/GameObjectData/PlanetData.txt", PlanetData, PlanetDataMap);
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void Planets::init()
{
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture0.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture1.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture2.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture3.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture4.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture5.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture6.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture7.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture8.png"));
	planet_textures.push_back(AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture9.png"));

	PLANET_SIZE					= PlanetDataMap["Planet_Size"];
	PLANET_ROT_SPEED			= PlanetDataMap["Planet_Rotation_Speed"];
	PLANET_SPAWN_BUFFER			= PlanetDataMap["Planet_Spawn_Buffer"];
	SHUTTLE_SPAWN_TIME_MAX		= static_cast<int>(PlanetDataMap["Maximum_Time_Shuttle_Spawn"]);
	SHUTTLE_SPAWN_TIME_MIN		= static_cast<int>(PlanetDataMap["Minimum_Time_Shuttle_Spawn"]);
	DEBRIS_MAX					= static_cast<int>(PlanetDataMap["Maximum_Debris"]);
	DEBRIS_MIN					= static_cast<int>(PlanetDataMap["Minimum_Debris"]);
	DRONES_MAX					= static_cast<int>(PlanetDataMap["Maximum_Drones"]);
	RUNWAY_LIFESPAN				= PlanetDataMap["Runway_Lifespan"];
	RUNWAY_MAX_ACCEL			= PlanetDataMap["Runway_Acceleration"];


	// ============
	// Planet Halo
	// ============
	halo_scale_lerp = 0.f;
}

/******************************************************************************/
/*!
	Update Planet
*/
/******************************************************************************/
void Planets::update(f64 frame_time)
{
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		// Only spawn shuttle if wave is completed and planet is not being added (camera transition) and player has orbitted at least once
		if (!planet_vector[i].wave_complete && !wave_manager.planet_adding && wave_manager.first_contact)
		{
			if (planet_vector[i].shuttle_timer <= 0.0)
			{
				AEVec2Zero(&runway_vector[i].velocity);
				planet_vector[i].shuttle_direction = AERandFloat() * (2 * PI);	// Randomize shuttle direction
			}

			// Update shuttle timer
			if (!wave_manager.capacity_full)
			{
				planet_vector[i].shuttle_timer += static_cast<f32>(frame_time);
			}

			// Update runway timer
			runway_vector[i].lifespan -= static_cast<f32>(frame_time);

			// Spawn shuttle when timer exceeds time_to_spawn, next shuttle will have randomized time_to_spawn
			if (planet_vector[i].shuttle_timer >= planet_vector[i].shuttle_time_to_spawn)
			{
				shuttle.spawn(planet_vector[i].id, planet_vector[i].shuttle_direction);	// Spawn shuttle
				planet_vector[i].current_shuttle--;										// Decrement current_shuttle
				planet_vector[i].shuttle_timer = 0.0;									// Reset shuttle timer
				planet_vector[i].shuttle_time_to_spawn = static_cast<f32>(rand() % (SHUTTLE_SPAWN_TIME_MAX - SHUTTLE_SPAWN_TIME_MIN) + SHUTTLE_SPAWN_TIME_MIN);	// Randomize time_to_spawn

				// Resetting runway
				runway_vector[i].position = planet_vector[i].position;
				AEVec2Zero(&runway_vector[i].velocity);
				runway_vector[i].lifespan = RUNWAY_LIFESPAN;
			}
		}

		// Rotate the planet
		planet_vector[i].direction += PLANET_ROT_SPEED * static_cast<f32>(frame_time);

		// Update the transform matrix with new direction
		AEVec2 added;
		AEVec2Set(&added, AECos(planet_vector[i].direction), AESin(planet_vector[i].direction));
		AEMtx33Rot(&planet_vector[i].rotate, planet_vector[i].direction);
		AEMtx33Concat(&planet_vector[i].transform, &planet_vector[i].rotate, &planet_vector[i].scale);
		AEMtx33Concat(&planet_vector[i].transform, &planet_vector[i].translate, &planet_vector[i].transform);

		// Update orbit range transform matrix
		AEMtx33Scale(&planet_vector[i].orbit_scale, (planet_vector[i].size + orbit_range * 2), (planet_vector[i].size + orbit_range * 2));
		AEMtx33Rot(&planet_vector[i].orbit_rotate, 0.f);
		AEMtx33Trans(&planet_vector[i].orbit_translate, planet_vector[i].position.x, planet_vector[i].position.y);
		AEMtx33Concat(&planet_vector[i].orbit_transform, &planet_vector[i].orbit_rotate, &planet_vector[i].orbit_scale);
		AEMtx33Concat(&planet_vector[i].orbit_transform, &planet_vector[i].orbit_translate, &planet_vector[i].orbit_transform);


		AEVec2Zero(&added);
		// Runway accelerating
		AEVec2Set(&added, AECos(planet_vector[i].shuttle_direction), AESin(planet_vector[i].shuttle_direction));
		AEVec2Scale(&added, &added, RUNWAY_MAX_ACCEL * static_cast<f32>(frame_time));
		AEVec2Add(&runway_vector[i].velocity, &added, &runway_vector[i].velocity);

		// Limiting runway velocity
		AEVec2Scale(&runway_vector[i].velocity, &runway_vector[i].velocity, 0.99f);

		if (wave_manager.planet_adding)
		{
			// Do not move if transitioning
			runway_vector[i].position = planet_vector[i].position;
		}
		else
		{
			// Update runway position
			runway_vector[i].position.x += runway_vector[i].velocity.x * static_cast<f32>(frame_time);
			runway_vector[i].position.y += runway_vector[i].velocity.y * static_cast<f32>(frame_time);
		}

		// Resetting runway
		if (runway_vector[i].lifespan <= 0)
		{
			runway_vector[i].position = planet_vector[i].position;
			AEVec2Zero(&runway_vector[i].velocity);
			runway_vector[i].lifespan = RUNWAY_LIFESPAN;
		}

		AEMtx33Rot(&runway_vector[i].rotate, planet_vector[i].shuttle_direction - PI);
		AEMtx33Trans(&runway_vector[i].translate, runway_vector[i].position.x, runway_vector[i].position.y);
		AEMtx33Concat(&runway_vector[i].transform, &runway_vector[i].rotate, &runway_vector[i].scale);
		AEMtx33Concat(&runway_vector[i].transform, &runway_vector[i].translate, &runway_vector[i].transform);
	}


	if (player.state == PLAYER_ORBIT)
	{
		// Update the Lerp value for the halo scale
		halo_scale_lerp += (1.0f - halo_scale_lerp) * 0.1f;
	}
	else
	{
		// Update the Lerp value for the halo scale
		if (halo_scale_lerp > 0.f)
		{
			halo_scale_lerp -= halo_scale_lerp * 0.01f;
		}
	}

	AEMtx33 scale, rot, trans;
	halo_size = player.current_planet.size + 60.f;

	// Use the Lerp value to scale the halo
	AEMtx33Scale(&scale, halo_size* halo_scale_lerp, halo_size* halo_scale_lerp);
	AEMtx33Rot(&rot, 0);
	AEMtx33Trans(&trans, player.current_planet.position.x, player.current_planet.position.y);
	AEMtx33Concat(&orbit_halo_transform, &rot, &scale);
	AEMtx33Concat(&orbit_halo_transform, &trans, &orbit_halo_transform);

}

/******************************************************************************/
/*!
	Draw Planet
*/
/******************************************************************************/
void Planets::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(orbit_halo_tex, 0, 0);
	AEGfxSetTransform(orbit_halo_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	for (int i{}; i < wave_manager.planet_count; i++)
	{
		AEGfxSetTransparency(1.f);

		// Draw orbit ring first
		AEGfxTextureSet(orbit_tex, 0, 0);
		AEGfxSetTransform(planet_vector[i].orbit_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// Draw planet sprite
		AEGfxTextureSet(planet_textures[planet_vector[i].texture_index], 0, 0);
		AEGfxSetTransform(planet_vector[i].transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		if (!wave_manager.planet_adding && wave_manager.first_contact)
		{
			// Draw runway sprite
			AEGfxTextureSet(runway_tex, 0, 0);
			AEGfxSetTransform(runway_vector[i].transform.m);
			if (runway_vector[i].lifespan <= RUNWAY_LIFESPAN)
			{
				AEGfxSetTransparency(runway_vector[i].lifespan / (RUNWAY_LIFESPAN));
			}
			else
			{
				AEGfxSetTransparency(1.f);
			}
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

/******************************************************************************/
/*!
	Clean Object Instances
*/
/******************************************************************************/
void Planets::free()
{
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		planet_vector[i].debris_vector.clear();
	}
	planet_vector.clear();
	runway_vector.clear();

	if (next_state != GS_RESTART) {
		PlanetData.clear();
		PlanetDataMap.clear();
	}
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void Planets::unload()
{
	for (AEGfxTexture* texture : planet_textures)
	{
		AEGfxTextureUnload(texture);
	}
	planet_textures.clear(); // to change!!
	AEGfxTextureUnload(orbit_tex);
	AEGfxTextureUnload(runway_tex);
	AEGfxTextureUnload(orbit_halo_tex);
}

/******************************************************************************/
/*!
	Additional Functions
*/
/******************************************************************************/
// Spawn planet
void Planets::spawn(int shuttle_randomize_amount)
{
	Planets new_planet;

	new_planet.id = wave_manager.planet_count;
	new_planet.wave_complete = false;

	new_planet.max_shuttle = shuttle_randomize_amount;
	new_planet.current_shuttle = new_planet.max_shuttle;

	// SETTING POSITION / TRANSFORM FOR PLANETS---------------------------------------------------------------------------------------------------
	new_planet.size = PLANET_SIZE * (new_planet.max_shuttle / 2.f);
	AEMtx33Scale(&new_planet.scale, new_planet.size, new_planet.size);

	new_planet.direction = PI / 4;
	AEMtx33Rot(&new_planet.rotate, direction);

	// rand()%(max-min) + min;
	AEVec2Set(&new_planet.position,
		static_cast<f32>(rand() % static_cast<int>(get_max_x() - get_min_x() + AEGetWindowWidth()) + (get_min_x() - AEGetWindowWidth())),
		static_cast<f32>(rand() % static_cast<int>(get_max_y() - get_min_y() + AEGetWindowHeight()) + (get_min_y() - AEGetWindowHeight())));
	check_spawn(new_planet);

	AEMtx33Trans(&new_planet.translate, new_planet.position.x, new_planet.position.y);
	AEMtx33Concat(&new_planet.transform, &new_planet.rotate, &new_planet.scale);

	AEMtx33Concat(&new_planet.transform, &new_planet.translate, &new_planet.transform);

	// SETTING POSITION / TRANSFORM FOR PLANETS---------------------------------------------------------------------------------------------------

	new_planet.shuttle_timer = 0.0;																													// Zero out shuttle timer on spawn
	new_planet.shuttle_time_to_spawn = static_cast<f32>(rand() % (SHUTTLE_SPAWN_TIME_MAX - SHUTTLE_SPAWN_TIME_MIN + 1) + SHUTTLE_SPAWN_TIME_MIN);	// Randomize value for timer to reach before spawning
	if (0 == new_planet.id)
	{
		new_planet.current_shuttle = new_planet.max_shuttle = 1;
		new_planet.shuttle_time_to_spawn /= 2; // Double time for tutorial
	}
	new_planet.shuttle_spawn_pos.x = new_planet.position.x;
	new_planet.shuttle_spawn_pos.y = new_planet.position.y;

	// DEBRIS STUFF-------------------------------------------------------------------------------------------------------------------------------
	new_planet.max_debris = rand() % (DEBRIS_MAX - DEBRIS_MIN) + DEBRIS_MIN;												// Randomize debris count on planet spawn
	new_planet.max_debris /= ((shuttle_randomize_amount / 2) < ((SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN) / 2)) ? 2 : 1;		// Limiting debris count on smaller planets
	new_planet.debris_vector = debris.create_debris(new_planet.position.x, new_planet.position.y, new_planet.size, new_planet.max_debris);

	// DRONE STUFF-------------------------------------------------------------------------------------------------------------------------------

		// Set new planet's current drone counter to 0
	new_planet.current_drones = 0;

	// For tutorial purposes, use the 4th planet sprite for clarity
	new_planet.texture_index = (0 == new_planet.id) ? 4 : rand() % planet_textures.size();

	// Add new drone vector to primary drone vector
	std::vector<Drone> drone_vector;
	drone_vector_all.push_back(drone_vector);
	planet_vector.push_back(new_planet);
}

// Check spawn position of planet
void Planets::check_spawn(Planets& new_planet)
{
	bool planet_check = false;
	while (!planet_check)
	{
		// Re-randomize new planet position if too close to another planet
		for (int i{}; i < wave_manager.planet_count; i++)
		{
			if (AEVec2Distance(&planet_vector[i].position, &new_planet.position) < PLANET_SPAWN_BUFFER * (planet_vector[i].size + new_planet.size))
			{
				AEVec2Set(&new_planet.position,
					static_cast<f32>(rand() % static_cast<int>(get_max_x() - get_min_x() + AEGetWindowWidth() + new_planet.size) + (get_min_x() - AEGetWindowWidth() - new_planet.size)),
					static_cast<f32>(rand() % static_cast<int>(get_max_y() - get_min_y() + AEGetWindowHeight() + new_planet.size) + (get_min_y() - AEGetWindowHeight() - new_planet.size)));
				(1 == wave_manager.planet_count) ? i-- : i = 0;
			}
		}
		planet_check = true;

		// Re-randomize new planet position if too close to space station
		for (size_t i{}; i < space_station_vector.size(); i++)
		{
			if (AEVec2Distance(&space_station_vector[i].position, &new_planet.position) < PLANET_SPAWN_BUFFER * (new_planet.orbit_range + new_planet.size))
			{
				AEVec2Set(&new_planet.position,
					static_cast<f32>(rand() % static_cast<int>(get_max_x() - get_min_x() + AEGetWindowWidth() + new_planet.size) + (get_min_x() - AEGetWindowWidth() - new_planet.size)),
					static_cast<f32>(rand() % static_cast<int>(get_max_y() - get_min_y() + AEGetWindowHeight() + new_planet.size) + (get_min_y() - AEGetWindowHeight() - new_planet.size)));
				(1 == static_cast<int>(space_station_vector.size())) ? i-- : i = 0;

				planet_check = false;
			}
		}
	}
}

// Add runway for shuttle
void Planets::add_runway(AEVec2 const& planet_pos)
{
	Planets::Runway new_runway;

	new_runway.size = 35.f;
	new_runway.direction = 0.f;
	new_runway.lifespan = RUNWAY_LIFESPAN;
	AEVec2Set(&new_runway.position, planet_pos.x, planet_pos.y);
	AEVec2Set(&new_runway.velocity, 0.f, 0.f);
	AEMtx33Scale(&new_runway.scale, new_runway.size, new_runway.size);
	AEMtx33Rot(&new_runway.rotate, shuttle_direction);
	AEMtx33Trans(&new_runway.translate, 0.f, 0.f);
	AEMtx33Concat(&new_runway.transform, &new_runway.rotate, &new_runway.scale);
	AEMtx33Concat(&new_runway.transform, &new_runway.translate, &new_runway.transform);

	runway_vector.push_back(new_runway);
}

// Return planet at furthest right x position
f32 Planets::get_max_x()
{
	f32 max_x{};
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		max_x = (planet_vector[i].position.x > max_x) ? planet_vector[i].position.x : max_x;
	}
	return max_x;
}

// Return planet at furthest top y position
f32 Planets::get_max_y()
{
	f32 max_y{};
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		max_y = (planet_vector[i].position.y > max_y) ? planet_vector[i].position.y : max_y;
	}
	return max_y;
}

// Return planet at furthest left x position
f32 Planets::get_min_x()
{
	f32 min_x{};
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		min_x = (planet_vector[i].position.x < min_x) ? planet_vector[i].position.x : min_x;
	}
	return min_x;
}

// Return planet at furthest bottom y position
f32 Planets::get_min_y()
{
	f32 min_y{};
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		min_y = (planet_vector[i].position.y < min_y) ? planet_vector[i].position.y : min_y;
	}
	return min_y;
}