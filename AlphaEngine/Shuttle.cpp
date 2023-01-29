#include "AEEngine.h"
#include "Shuttle.h"
#include "Planet.h"
#include "WaveManager.h"
#include <cmath>

Shuttles shuttle;
AEGfxTexture* shuttle_tex;
std::vector<Shuttles> shuttle_vector;

void Shuttles::load()
{
	shuttle_tex = AEGfxTextureLoad("Assets/test-player.png");
}

void Shuttles::init()
{
	srand(5);
}

void Shuttles::update(f64 frame_time)
{
	for (size_t i{}; i < shuttle_vector.size(); i++)
	{
		if (shuttle_vector[i].active)
		{
			shuttle_vector[i].position.x += shuttle_vector[i].speed * shuttle_vector[i].vector.x * frame_time;
			shuttle_vector[i].position.y += shuttle_vector[i].speed * shuttle_vector[i].vector.y * frame_time;

			if (shuttle_vector[i].lifespan <= 0)
			{
				shuttle_vector[i].active = false;
			}
			shuttle_vector[i].lifespan -= frame_time;
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
			AEMtx33Trans(&shuttle_vector[i].translate, shuttle_vector[i].position.x, shuttle_vector[i].position.y);
			AEMtx33Concat(&shuttle_vector[i].transform, &shuttle_vector[i].rotate, &shuttle_vector[i].scale);
			AEMtx33Concat(&shuttle_vector[i].transform, &shuttle_vector[i].translate, &shuttle_vector[i].transform);

			AEGfxSetTransform(shuttle_vector[i].transform.m);
			// Actually drawing the mesh
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

void Shuttles::free()
{
	shuttle_vector.clear();
}

void Shuttles::unload()
{
	AEGfxTextureUnload(shuttle_tex);
}

void Shuttles::spawn(int planet_id)
{
	Shuttles* new_shuttle{ new Shuttles };

	AEMtx33Scale(&new_shuttle->scale, 20.f, 20.f);
	AEMtx33Rot(&new_shuttle->rotate, PI / 4);

	new_shuttle->position.x = planet_vector[planet_id].shuttle_spawn_pos.x;
	new_shuttle->position.y = planet_vector[planet_id].shuttle_spawn_pos.y;

	//AEVec2Sub(&new_shuttle->vector, &planet_vector[planet_id].position, &new_shuttle->position);
	new_shuttle->vector.x = 1;
	new_shuttle->vector.y = -1;

	new_shuttle->lifespan = 2;
	new_shuttle->speed = 100;

	new_shuttle->active = true;

	shuttle_vector.push_back(*new_shuttle);
}