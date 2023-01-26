#include "AEEngine.h"
#include "Planet.h"
#include <cmath>

int x_max, y_max;

f32* trans_x_array;
f32* trans_y_array;

int planet_count{};
Planets* planet_array;

f64 elapsed_time{};

void planet::load()
{

}

void planet::init()
{
	srand(5);

	elapsed_time = 0.0;
	planet_array = new Planets[max_planet];
	trans_x_array = new f32[max_planet];
	trans_y_array = new f32[max_planet];
	planet_count = 0;
	x_max = 1400, y_max = 700;
}

void planet::update(f64 frame_time)
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
			if (abs(pow(trans_y_array[i] - newPlanet.position.y, 2) + pow(trans_x_array[i] - newPlanet.position.x, 2)) < pow(300, 2))
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

		trans_x_array[planet_count] = newPlanet.position.x;
		trans_y_array[planet_count] = newPlanet.position.y;
		
		if (planet_count < 10) planet_count++;
		elapsed_time = 0;
	}
}

void planet::draw(AEGfxVertexList* pMesh)
{
	for (int i{}; i < planet_count; i++)
	{
		AEGfxSetTransform(planet_array[i].transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void planet::free()
{

}

void planet::unload()
{
	delete[] planet_array;
	delete[] trans_x_array;
	delete[] trans_y_array;
}