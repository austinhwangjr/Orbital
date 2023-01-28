#include "AEEngine.h"
#include "Planet.h"
#include "Shuttle.h"
#include <cmath>
#include "Debris.h"

Planets planet;
AEGfxTexture* planet_tex;
std::vector<Planets> planet_vector;

extern Shuttles shuttle;

Debris debris_init;

int x_max, y_max;
int planet_count{};

static f64 planet_spawn_time{};

void Planets::load()
{
	planet_tex = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void Planets::init()
{
	srand(5);

	planet_spawn_time = 0.0;
	planet_count = 0;
	x_max = 1400, y_max = 700;
}

void Planets::update(f64 frame_time)
{
	planet_spawn_time += frame_time;

	if (planet_spawn_time >= 2 && planet_count < 10)
	{
		Planets new_planet;

		new_planet.id = planet_count;

		AEMtx33Scale(&new_planet.scale, 100.f, 100.f);
		AEMtx33Rot(&new_planet.rotate, PI / 4);

		new_planet.position.x = static_cast<f32>(rand() % (x_max + 1) - x_max / 2);
		new_planet.position.y = static_cast<f32>(rand() % (y_max + 1) - y_max / 2);

		for (int i{}; i < planet_count; i++)
		{
			if (abs(pow(planet_vector[i].position.y - new_planet.position.y, 2) + pow(planet_vector[i].position.x - new_planet.position.x, 2)) < pow(300, 2))
			{
				new_planet.position.x = static_cast<f32>(rand() % (x_max + 1) - x_max / 2);
				new_planet.position.y = static_cast<f32>(rand() % (y_max + 1) - y_max / 2);
				i = 0;
			}
		}

		AEMtx33Trans(&new_planet.translate, new_planet.position.x, new_planet.position.y);
		AEMtx33Concat(&new_planet.transform, &new_planet.rotate, &new_planet.scale);
		AEMtx33Concat(&new_planet.transform, &new_planet.translate, &new_planet.transform);

		new_planet.shuttle_spawn_timer = 0.0;
		new_planet.shuttle_time_spawn = 2.0;
		new_planet.shuttle_spawn_pos.x = new_planet.position.x;
		new_planet.shuttle_spawn_pos.y = new_planet.position.y;

		//new_planet.max_debris = 10;
		
		new_planet.max_debris = 10;
		new_planet.debris_draw_array = draw_debris(new_planet, new_planet.max_debris);
		new_planet.debris_arr = create_debris(new_planet, new_planet.max_debris);
		
		planet_vector.push_back(new_planet);
		if (planet_count < max_planet) planet_count++;
		planet_spawn_time = 0;
	}

	for (size_t i{}; i < planet_vector.size(); i++)
	{
		if (planet_vector[i].shuttle_spawn_timer > planet_vector[i].shuttle_time_spawn)
		{
			planet_vector[i].shuttle_spawn_timer = 0.0;
		}
		planet_vector[i].shuttle_spawn_timer += frame_time;
	}

	for (Planets const &current_planet : planet_vector)
	{
		if (current_planet.shuttle_spawn_timer >= current_planet.shuttle_time_spawn)
		{
			shuttle.Shuttles::spawn(current_planet.id);
		}
	}
}

void Planets::draw(AEGfxVertexList* pMesh)
{
	// Set the texture to pTex 
	AEGfxTextureSet(planet_tex, 0, 0);

	for (int i{}; i < planet_count; i++)
	{
		AEGfxSetTransform(planet_vector[i].transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Planets::free()
{

}

void Planets::unload()
{
	AEGfxTextureUnload(planet_tex);
}