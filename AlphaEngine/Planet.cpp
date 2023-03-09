/* Start Header ************************************************************************/
/*!
\file		Planet.cpp
\author		Tan Rui An Ryan, ruianryan.t, 2200600
\par		ruianryan.t\@digipen.edu
\date		Feb 27, 2023
\brief		This file contains the behaviour for the Planet game object

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "AEEngine.h"
#include "Planet.h"
#include "Drone.h"
#include "SpaceStation.h"
#include "WaveManager.h"
#include <cmath>

#include <iostream>

AEGfxTexture* planet_tex;
AEGfxTexture* orbit_tex;
std::vector<Planets> planet_vector;

extern WaveManager wave_manager;
extern Shuttles shuttle;
extern Debris debris;
extern std::vector<SpaceStation> space_station_vector;
extern std::vector<std::vector<Drone>> drone_vector_all;

void Planets::load()
{
	planet_tex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	orbit_tex = AEGfxTextureLoad("Assets/orbitRing.png");
}

void Planets::init()
{

}

void Planets::update(f64 frame_time)
{
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		// Only spawn shuttle if wave is completed and planet is not being added (camera transition)
		if (!planet_vector[i].wave_complete && !wave_manager.planet_adding)
		{
			// Spawn shuttle when timer exceeds time_to_spawn, next shuttle will have randomized time_to_spawn
			if (planet_vector[i].shuttle_timer >= planet_vector[i].shuttle_time_to_spawn)
			{
				shuttle.Shuttles::spawn(planet_vector[i].id);	// Spawn shuttle
				planet_vector[i].current_shuttle--;				// Decrement current_shuttle
				planet_vector[i].shuttle_timer = 0.0;			// Reset shuttle timer
				planet_vector[i].shuttle_time_to_spawn = static_cast<f32>(rand() % (SHUTTLE_SPAWN_TIME_MAX - SHUTTLE_SPAWN_TIME_MIN) + SHUTTLE_SPAWN_TIME_MIN);	// Randomize time_to_spawn
			}
			
			// Update shuttle timer
			planet_vector[i].shuttle_timer += static_cast<f32>(frame_time);
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
	}
}

void Planets::draw(AEGfxVertexList* pMesh)
{
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		// Set the texture to planet_tex 
		AEGfxTextureSet(planet_tex, 0, 0);
		AEGfxSetTransform(planet_vector[i].transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// Set the texture to orbit_tex 
		AEGfxTextureSet(orbit_tex, 0, 0);
		AEGfxSetTransform(planet_vector[i].orbit_transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Planets::free()
{
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		planet_vector[i].debris_vector.clear();
	}
	planet_vector.clear();
}

void Planets::unload()
{
	AEGfxTextureUnload(planet_tex);
	AEGfxTextureUnload(orbit_tex);
}

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
	new_planet.shuttle_spawn_pos.x = new_planet.position.x;
	new_planet.shuttle_spawn_pos.y = new_planet.position.y;

// DEBRIS STUFF-------------------------------------------------------------------------------------------------------------------------------
	new_planet.max_debris = rand() % (DEBRIS_MAX - DEBRIS_MIN) + DEBRIS_MIN;												// Randomize debris count on planet spawn
	new_planet.max_debris /= ((shuttle_randomize_amount / 2) < ((SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN) / 2)) ? 2 : 1;		// Limiting debris count on smaller planets
	new_planet.debris_vector = debris.Debris::create_debris(new_planet.position.x, new_planet.position.y, new_planet.size, new_planet.max_debris);
	
// DRONE STUFF-------------------------------------------------------------------------------------------------------------------------------

	// Set new planet's current drone counter to 0
	new_planet.current_drones = 0;

	// Add new drone vector to primary drone vector
	std::vector<Drone> drone_vector;
	drone_vector_all.push_back(drone_vector);

	planet_vector.push_back(new_planet);
}

void Planets::check_spawn(Planets& new_planet)
{
	bool planet_check = false;
	while (!planet_check)
	{
		// Re-randomize new planet position if too close to another planet
		for (int i{}; i < wave_manager.planet_count; i++)
		{
			if (AEVec2Distance(&planet_vector[i].position, &new_planet.position) < 1.5 * (planet_vector[i].size + new_planet.size))
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
			if (AEVec2Distance(&space_station_vector[i].position, &new_planet.position) < 1.5 * (new_planet.orbit_range + new_planet.size))
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