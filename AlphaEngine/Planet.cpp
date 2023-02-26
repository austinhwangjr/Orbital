#include "AEEngine.h"
#include "Planet.h"
#include "SpaceStation.h"
#include "WaveManager.h"
#include <cmath>

AEGfxTexture* planet_tex;
std::vector<Planets> planet_vector;

extern WaveManager wave_manager;
extern Shuttles shuttle;
extern Debris debris;
extern std::vector<SpaceStation> space_station_vector;

void Planets::load()
{
	planet_tex = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void Planets::init()
{
	srand(6);
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
				planet_vector[i].shuttle_time_to_spawn = static_cast<f64>(rand() % (SHUTTLE_SPAWN_TIME_MAX - SHUTTLE_SPAWN_TIME_MIN) + SHUTTLE_SPAWN_TIME_MIN);	// Randomize time_to_spawn
			}

			planet_vector[i].shuttle_timer += frame_time;
		}
	}
}

void Planets::draw(AEGfxVertexList* pMesh)
{
	// Set the texture to pTex 
	AEGfxTextureSet(planet_tex, 0, 0);

	for (int i{}; i < wave_manager.planet_count; i++)
	{
		AEGfxSetTransform(planet_vector[i].transform.m);
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
}

void Planets::spawn(int shuttle_randomize_amount)
{
	Planets new_planet;

	new_planet.id = wave_manager.planet_count;
	new_planet.wave_complete = false;

	new_planet.max_shuttle = shuttle_randomize_amount;
	new_planet.current_shuttle = new_planet.max_shuttle;

// SETTING POSITION / TRANSFORM FOR PLANETS---------------------------------------------------------------------------------------------------
	new_planet.size = PLANET_SIZE * (new_planet.max_shuttle / 2.0f);
	AEMtx33Scale(&new_planet.scale, new_planet.size, new_planet.size);
	AEMtx33Rot(&new_planet.rotate, PI / 4);

	// rand()%(max-min + 1) + min;
	AEVec2Set(&new_planet.position,
		static_cast<f32>(rand() % static_cast<int>(get_max_x() - get_min_x() + AEGetWindowWidth() + PLANET_SIZE) + (get_min_x() - AEGetWindowWidth() - PLANET_SIZE)),
		static_cast<f32>(rand() % static_cast<int>(get_max_y() - get_min_y() + AEGetWindowHeight() + PLANET_SIZE) + (get_min_y() - AEGetWindowHeight() - PLANET_SIZE)));
	check_spawn(new_planet);

	AEMtx33Trans(&new_planet.translate, new_planet.position.x, new_planet.position.y);
	AEMtx33Concat(&new_planet.transform, &new_planet.rotate, &new_planet.scale);

	AEMtx33Concat(&new_planet.transform, &new_planet.translate, &new_planet.transform);

// SETTING POSITION / TRANSFORM FOR PLANETS---------------------------------------------------------------------------------------------------

	new_planet.shuttle_timer = 0.0;																													// Zero out shuttle timer on spawn
	new_planet.shuttle_time_to_spawn = static_cast<f64>(rand() % (SHUTTLE_SPAWN_TIME_MAX - SHUTTLE_SPAWN_TIME_MIN + 1) + SHUTTLE_SPAWN_TIME_MIN);	// Randomize value for timer to reach before spawning
	new_planet.shuttle_spawn_pos.x = new_planet.position.x;
	new_planet.shuttle_spawn_pos.y = new_planet.position.y;

// DEBRIS STUFF-------------------------------------------------------------------------------------------------------------------------------
	new_planet.max_debris = rand() % (DEBRIS_MAX - DEBRIS_MIN) + DEBRIS_MIN;							// Randomize debris count on planet spawn
	new_planet.max_debris /= ((shuttle_randomize_amount / 2) < (SHUTTLE_SPAWN_MAX / 2)) ? 2 : 1;		// Limiting debris count on smaller planets
	new_planet.debris_vector = debris.Debris::create_debris(new_planet.position.x, new_planet.position.y, new_planet.size, new_planet.max_debris);

// DEBRIS STUFF-------------------------------------------------------------------------------------------------------------------------------

	new_planet.current_drones = 0;

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
			if (AEVec2Distance(&planet_vector[i].position, &new_planet.position) < planet_vector[i].size + new_planet.size)
			{
				AEVec2Set(&new_planet.position,
					static_cast<f32>(rand() % static_cast<int>(get_max_x() - get_min_x() + AEGetWindowWidth() + PLANET_SIZE) + (get_min_x() - AEGetWindowWidth() - PLANET_SIZE)),
					static_cast<f32>(rand() % static_cast<int>(get_max_y() - get_min_y() + AEGetWindowHeight() + PLANET_SIZE) + (get_min_y() - AEGetWindowHeight() - PLANET_SIZE)));
				(1 == wave_manager.planet_count) ? i-- : i = 0;
			}
		}
		planet_check = true;

		// Re-randomize new planet position if too close to space station
		for (size_t i{}; i < space_station_vector.size(); i++)
		{
			if (AEVec2Distance(&space_station_vector[i].position, &new_planet.position) < player.dist_from_planet + new_planet.size)
			{
				AEVec2Set(&new_planet.position,
					static_cast<f32>(rand() % static_cast<int>(get_max_x() - get_min_x() + AEGetWindowWidth() + PLANET_SIZE) + (get_min_x() - AEGetWindowWidth() - PLANET_SIZE)),
					static_cast<f32>(rand() % static_cast<int>(get_max_y() - get_min_y() + AEGetWindowHeight() + PLANET_SIZE) + (get_min_y() - AEGetWindowHeight() - PLANET_SIZE)));
				(1 == static_cast<int>(space_station_vector.size())) ? i-- : i = 0;

				planet_check = false;
			}
		}
	}
}

f32 Planets::get_max_x()
{
	f32 max_x{};
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		max_x = (planet_vector[i].position.x > max_x) ? planet_vector[i].position.x : max_x;
	}
	return max_x;
}

f32 Planets::get_max_y()
{
	f32 max_y{};
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		max_y = (planet_vector[i].position.y > max_y) ? planet_vector[i].position.y : max_y;
	}
	return max_y;
}

f32 Planets::get_min_x()
{
	f32 min_x{};
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		min_x = (planet_vector[i].position.x < min_x) ? planet_vector[i].position.x : min_x;
	}
	return min_x;
}

f32 Planets::get_min_y()
{
	f32 min_y{};
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		min_y = (planet_vector[i].position.y < min_y) ? planet_vector[i].position.y : min_y;
	}
	return min_y;
}