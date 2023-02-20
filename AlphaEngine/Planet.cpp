#include "AEEngine.h"
#include "Planet.h"
#include "WaveManager.h"
#include <cmath>

AEGfxTexture* planet_tex;
std::vector<Planets> planet_vector;

extern WaveManager wave_manager;
extern Shuttles shuttle;
extern Debris debris;

void Planets::load()
{
	planet_tex = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void Planets::init()
{
	srand(5);
}

void Planets::update(f64 frame_time)
{
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		if (!planet_vector[i].wave_complete)
		{
			if (planet_vector[i].shuttle_timer > planet_vector[i].shuttle_time_to_spawn)
			{
				planet_vector[i].shuttle_timer = 0.0;
				planet_vector[i].shuttle_time_to_spawn = static_cast<f64>(rand() % TIME_TO_SPAWN + MIN_SHUTTLE_TIME);
			}
			planet_vector[i].shuttle_timer += frame_time;

			if (planet_vector[i].shuttle_timer >= planet_vector[i].shuttle_time_to_spawn)
			{
				shuttle.Shuttles::spawn(planet_vector[i].id);
				planet_vector[i].current_shuttle--;
			}
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

	new_planet.max_shuttle = shuttle_randomize_amount;
	new_planet.current_shuttle = new_planet.max_shuttle;
	new_planet.wave_complete = false;

	new_planet.max_debris = 10;

	new_planet.size = PLANET_SIZE * (new_planet.max_shuttle / 2.0f);

// SETTING POSITION / TRANSFORM FOR PLANETS---------------------------------------------------------------------------------------------------
	AEMtx33Scale(&new_planet.scale, new_planet.size, new_planet.size);
	AEMtx33Rot(&new_planet.rotate, PI / 4);

	AEVec2Set(&new_planet.position,
		static_cast<f32>(rand() % (static_cast<int>(AEGfxGetWinMaxX()) + 1) - AEGfxGetWinMaxX() / 2),
		static_cast<f32>(rand() % (static_cast<int>(AEGfxGetWinMaxY()) + 1) - AEGfxGetWinMaxY() / 2));

	// Re-randomize new planet position if too close to another planet
	for (int i{}; i < wave_manager.planet_count; i++)
	{
		//std::cout << "Distance" << AEVec2Distance(&planet_vector[i].position, &new_planet.position) << '\n';
		//std::cout << "Radius" << (planet_vector[i].size + new_planet.size) << '\n';
		if (AEVec2Distance(&planet_vector[i].position, &new_planet.position) < (planet_vector[i].size + new_planet.size))
		{
			AEVec2Set(&new_planet.position,
				static_cast<f32>(rand() % (static_cast<int>(AEGfxGetWinMaxX()) + 1) - AEGfxGetWinMaxX() / 2),
				static_cast<f32>(rand() % (static_cast<int>(AEGfxGetWinMaxY()) + 1) - AEGfxGetWinMaxY() / 2));
			(1 == wave_manager.planet_count) ? i-- : i = 0;
		}
	}

	AEMtx33Trans(&new_planet.translate, new_planet.position.x, new_planet.position.y);
	AEMtx33Concat(&new_planet.transform, &new_planet.rotate, &new_planet.scale);
	AEMtx33Concat(&new_planet.transform, &new_planet.translate, &new_planet.transform);
// SETTING POSITION / TRANSFORM FOR PLANETS---------------------------------------------------------------------------------------------------

	new_planet.shuttle_timer = 0.0;
	new_planet.shuttle_time_to_spawn = static_cast<f64>(rand() % TIME_TO_SPAWN + MIN_SHUTTLE_TIME);
	new_planet.shuttle_spawn_pos.x = new_planet.position.x;
	new_planet.shuttle_spawn_pos.y = new_planet.position.y;

	new_planet.debris_vector = debris.Debris::create_debris(new_planet.position.x, new_planet.position.y, new_planet.size, new_planet.max_debris);

	planet_vector.push_back(new_planet);
}