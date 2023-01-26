#include "AEEngine.h"
#include "Planet.h"
#include <cmath>

AEGfxTexture* pTex;
Planets* planet_array;

int x_max, y_max;
int planet_count{};

f64 elapsed_time{};

void Planets::load()
{
	pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void Planets::init()
{
	srand(5);

	elapsed_time = 0.0;
	planet_array = new Planets[max_planet]{};
	planet_count = 0;
	x_max = 1400, y_max = 700;
}

void Planets::update(f64 frame_time)
{
	elapsed_time += frame_time;

	if (elapsed_time >= 2 && planet_count < 10)
	{
		Planets newPlanet;

		AEMtx33Scale(&newPlanet.scale, 100.f, 100.f);
		AEMtx33Rot(&newPlanet.rotate, PI / 4);

		newPlanet.position.x = static_cast<f32>(rand() % (x_max + 1) - x_max / 2);
		newPlanet.position.y = static_cast<f32>(rand() % (y_max + 1) - y_max / 2);

		for (int i{}; i < planet_count; i++)
		{
			if (abs(pow(planet_array[i].position.y - newPlanet.position.y, 2) + pow(planet_array[i].position.x - newPlanet.position.x, 2)) < pow(300, 2))
			{
				newPlanet.position.x = static_cast<f32>(rand() % (x_max + 1) - x_max / 2);
				newPlanet.position.y = static_cast<f32>(rand() % (y_max + 1) - y_max / 2);
				i = 0;
			}
		}

		AEMtx33Trans(&newPlanet.translate, newPlanet.position.x, newPlanet.position.y);
		AEMtx33Concat(&newPlanet.transform, &newPlanet.rotate, &newPlanet.scale);
		AEMtx33Concat(&newPlanet.transform, &newPlanet.translate, &newPlanet.transform);

		planet_array[planet_count] = newPlanet;
		
		if (planet_count < 10) planet_count++;
		elapsed_time = 0;
	}
}

void Planets::draw(AEGfxVertexList* pMesh)
{
	// Set the texture to pTex 
	AEGfxTextureSet(pTex, 0, 0);
	for (int i{}; i < planet_count; i++)
	{
		AEGfxSetTransform(planet_array[i].transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Planets::free()
{

}

void Planets::unload()
{
	AEGfxTextureUnload(pTex);
	delete[] planet_array;
}