#include "AEEngine.h"
#include "Debris.h"
#include <cmath>
#include "Planet.h"
extern std::vector<Planets> planet_vector;//Planets* planet_array; // planet array access
//enum { num_stone=20 };
float planet_radius = 50.f;
float space = 20.f;
extern int planet_count;

static f64 elapsed_time{};


struct Debris* debris_array;
AEMtx33* debris_draw_array;

AEGfxTexture* debrisTex;

double speed = 0.125f;
void Debris::load()
{
	debrisTex = AEGfxTextureLoad("Assets/Debris.png");
}

void Debris::init()
{

	float planet_radius = 50.f;
	float space = 20.f;
	
}

Debris* create_debris(Planets planet, int total_debris) {
	Debris* arr{ new Debris[total_debris] };
	for (int i = 0; i < total_debris; i++) {
		arr[i].id = i + 1;
		arr[i].angle = i*30;
		arr[i].scale_x = 15.f;
		arr[i].scale_y = 15.f;
		//arr[i].turning_angle = 0.f + (i * 10);
		arr[i].position.x = planet.position.x + (planet_radius + space) * AECos(AEDegToRad(arr[i].angle));
		arr[i].position.y = planet.position.y + (planet_radius + space) * AESin(AEDegToRad(arr[i].angle));
		arr[i].turning_speed = speed;
		arr[i].dist_from_planet = 70.f;
	}



	/*
	for (int i = 0; i < total_debris; i++) {
		AEMtx33Scale(&debris_scale, arr[i].scale_x, arr[i].scale_y);
		AEMtx33Rot(&debris_rotate, arr[i].angle);
		AEMtx33Trans(&debris_translate, arr[i].position.x, arr[i].position.y);
		AEMtx33Concat(&debris_transform, &debris_rotate, &debris_scale);
		AEMtx33Concat(&debris_transform, &debris_translate, &debris_transform);
		planet.debris_draw_array = debris_transform;
	}
	*/
	return arr;

}

AEMtx33* draw_debris(Planets planet, int total_debris) {
	AEMtx33* arr{ new AEMtx33[total_debris] };

	return arr;
}

void Debris::update(f64 frame_time)
{

	


	for (int j = 0; j < planet_count; j++) {
		for (int i = 0; i < planet_vector[j].max_debris; i++) {
			
			planet_vector[j].debris_arr[i].angle -= 3.f;


			planet_vector[j].debris_arr[i].position.x = planet_vector[j].position.x + (space+planet_radius) * AECos(AEDegToRad(planet_vector[j].debris_arr[i].angle));
			planet_vector[j].debris_arr[i].position.y = planet_vector[j].position.y + (space + planet_radius) * AESin(AEDegToRad(planet_vector[j].debris_arr[i].angle));

		}
	}
	
}


void Debris::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(debrisTex, 0, 0);
	for (int j = 0; j < planet_count; j++) {
		for (int i = 0; i < planet_vector[j].max_debris; i++) {


			AEMtx33Scale(&planet_vector[j].debris_arr[i].scale, planet_vector[j].debris_arr[i].scale_x, planet_vector[j].debris_arr[i].scale_y);

			AEMtx33Rot(&planet_vector[j].debris_arr[i].rotate, AEDegToRad(planet_vector[j].debris_arr[i].angle));

			AEMtx33Trans(&planet_vector[j].debris_arr[i].translate, planet_vector[j].debris_arr[i].position.x, planet_vector[j].debris_arr[i].position.y);

			AEMtx33Concat(&planet_vector[j].debris_arr[i].transform, &planet_vector[j].debris_arr[i].rotate, &planet_vector[j].debris_arr[i].scale);

			AEMtx33Concat(&planet_vector[j].debris_arr[i].transform, &planet_vector[j].debris_arr[i].translate, &planet_vector[j].debris_arr[i].transform);

			planet_vector[j].debris_draw_array[i] = planet_vector[j].debris_arr[i].transform;

			AEGfxSetTransform(planet_vector[j].debris_draw_array[i].m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}


void Debris::free()
{
	
}

void Debris::unload()
{
	AEGfxTextureUnload(debrisTex);
}