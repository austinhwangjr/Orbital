#include "AEEngine.h"
#include "Debris.h"
#include <cmath>
#include "Planet.h"
extern Planets* planet_array; // planet array access
//enum { num_stone=20 };
float planet_radius = 50.f;
float space = 20.f;


static f64 elapsed_time{};


struct Debris* debris_array;
AEMtx33* debris_draw_array;

AEGfxTexture* debrisTex;

AEMtx33 debris_scale{};
AEMtx33 debris_rotate{};
AEMtx33 debris_translate{};
AEMtx33 debris_transform{};

void Debris::load()
{
	debrisTex = AEGfxTextureLoad("Assets/Debris.png");
}

void Debris::init()
{

	float planet_radius = 50.f;
	float space = 20.f;
	double speed = 0.125f;
}
/*
void Debris::create_debris(std::vector <Debris> debris_array, int total_debris){
		for (int i = 0; i < total_debris; i++) {
			debris_array[i].id = i + 1;
			debris_array[i].angle = 1;
			debris_array[i].scale_x = 15.f;
			debris_array[i].scale_y = 15.f;
			debris_array[i].angle = 1;
			debris_array[i].turning_angle = 0.f + (i * 50);
			debris_array[i].position.x = planet_array[i].position.x + planet_radius + space + (i * 5);
			debris_array[i].position.y = planet_array[i].position.y + planet_radius + space - (i * 5);
			debris_array[i].turning_speed = speed;
			debris_array[i].dist_from_planet = 70.f;
		}

		for (int i = 0; i < total_debris; i++) {
			AEMtx33Scale(&debris_scale, debris_array[i].scale_x, debris_array[i].scale_x);
			AEMtx33Rot(&debris_rotate, debris_array[i].angle);
			AEMtx33Trans(&debris_translate, debris_array[i].position.x, debris_array[i].position.y);
			AEMtx33Concat(&debris_transform, &debris_rotate, &debris_scale);
			AEMtx33Concat(&debris_transform, &debris_translate, &debris_transform);
			debris_draw_array[i] = debris_transform;
		}
}
*/
/*

void Debris::update(f64 frame_time)
{

	for (int j = 0; j < max_planet; j++) {
		for (int i = 0; i < planet_array[j].max_debris; i++) {

			planet_array[j].debris_array[i].turning_angle -= planet_array[j].debris_array[i].turning_speed;


			planet_array[j].debris_array[i].position.x = (planet_array[j].debris_array[i].dist_from_planet) * AECos(AEDegToRad(planet_array[j].debris_array[i].turning_angle));
			planet_array[j].debris_array[i].position.y = (planet_array[j].debris_array[i].dist_from_planet) * AESin(AEDegToRad(planet_array[j].debris_array[i].turning_angle));



			AEMtx33Scale(&debris_scale, planet_array[j].debris_array[i].scale_x, planet_array[j].debris_array[i].scale_x);
			AEMtx33Rot(&debris_rotate, planet_array[j].debris_array[i].angle);
			AEMtx33Trans(&debris_translate, planet_array[j].debris_array[i].position.x, planet_array[j].debris_array[i].position.y);
			AEMtx33Concat(&debris_transform, &debris_rotate, &debris_scale);
			AEMtx33Concat(&debris_transform, &debris_translate, &debris_transform);
			debris_draw_array[i] = debris_transform;

		}
	}

}

void Debris::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(debrisTex, 0, 0);
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < planet_array[j].max_debris; i++) {
			AEGfxSetTransform(debris_draw_array[i].m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}
*/

void Debris::free()
{
	
}

void Debris::unload()
{
	AEGfxTextureUnload(debrisTex);
}