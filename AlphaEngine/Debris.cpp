#include "AEEngine.h"
#include "Debris.h"
#include "Planet.h"
#include <cmath>

Debris debris;

extern std::vector<Planets> planet_vector;

//enum { num_stone=20 };
float planet_radius = 50.f;
float space = 20.f;
extern int planet_count;

static f64 elapsed_time{};

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

void Debris::update(f64 frame_time)
{
	for (int j = 0; j < planet_vector.size(); j++) {
		for (size_t i = 0; i < planet_vector[j].debris_vector.size(); i++) {
			planet_vector[j].debris_vector[i].angle -= 1.f;

			planet_vector[j].debris_vector[i].position.x = planet_vector[j].position.x + (space + planet_radius) * AECos(AEDegToRad(planet_vector[j].debris_vector[i].angle));
			planet_vector[j].debris_vector[i].position.y = planet_vector[j].position.y + (space + planet_radius) * AESin(AEDegToRad(planet_vector[j].debris_vector[i].angle));

		}
		/*for (int i = 0; i < planet_vector[j].max_debris; i++) {
			
			planet_vector[j].debris_vector[i].angle -= 3.f;


			planet_vector[j].debris_vector[i].position.x = planet_vector[j].position.x + (space+planet_radius) * AECos(AEDegToRad(planet_vector[j].debris_vector[i].angle));
			planet_vector[j].debris_vector[i].position.y = planet_vector[j].position.y + (space + planet_radius) * AESin(AEDegToRad(planet_vector[j].debris_vector[i].angle));

		}*/
	}	
}

void Debris::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(debrisTex, 0, 0);
	for (size_t j = 0; j < planet_vector.size(); j++) {
		for (size_t i = 0; i < planet_vector[j].debris_vector.size(); i++) {

			/*AEMtx33Scale(&planet_vector[j].debris_vector[i].scale, planet_vector[j].debris_vector[i].scale_x, planet_vector[j].debris_vector[i].scale_y);

			AEMtx33Rot(&planet_vector[j].debris_vector[i].rotate, AEDegToRad(planet_vector[j].debris_vector[i].angle));

			AEMtx33Trans(&planet_vector[j].debris_vector[i].translate, planet_vector[j].debris_vector[i].position.x, planet_vector[j].debris_vector[i].position.y);

			AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].rotate, &planet_vector[j].debris_vector[i].scale);

			AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].translate, &planet_vector[j].debris_vector[i].transform);

			planet_vector[j].debris_draw_array[i] = planet_vector[j].debris_vector[i].transform;

			AEGfxSetTransform(planet_vector[j].debris_draw_array[i].m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/
			if (planet_vector[j].debris_vector[i].active)
			{
				//AEMtx33 scale = { 0 };
				AEMtx33Scale(&planet_vector[j].debris_vector[i].scale, planet_vector[j].debris_vector[i].scale_x, planet_vector[j].debris_vector[i].scale_y);

				//AEMtx33 rotate = { 0 };
				AEMtx33Rot(&planet_vector[j].debris_vector[i].rotate, AEDegToRad(planet_vector[j].debris_vector[i].angle));

				//AEMtx33 translate = { 0 };
				AEMtx33Trans(&planet_vector[j].debris_vector[i].translate, planet_vector[j].debris_vector[i].position.x, planet_vector[j].debris_vector[i].position.y);

				//AEMtx33 transform = { 0 };
				AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].rotate, &planet_vector[j].debris_vector[i].scale);
				AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].translate, &planet_vector[j].debris_vector[i].transform);

				AEGfxSetTransform(planet_vector[j].debris_vector[i].transform.m);

				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
		/*for (int i = 0; i < planet_vector[j].max_debris; i++) {


			AEMtx33Scale(&planet_vector[j].debris_vector[i].scale, planet_vector[j].debris_vector[i].scale_x, planet_vector[j].debris_vector[i].scale_y);

			AEMtx33Rot(&planet_vector[j].debris_vector[i].rotate, AEDegToRad(planet_vector[j].debris_vector[i].angle));

			AEMtx33Trans(&planet_vector[j].debris_vector[i].translate, planet_vector[j].debris_vector[i].position.x, planet_vector[j].debris_vector[i].position.y);

			AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].rotate, &planet_vector[j].debris_vector[i].scale);

			AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].translate, &planet_vector[j].debris_vector[i].transform);

			planet_vector[j].debris_draw_array[i] = planet_vector[j].debris_vector[i].transform;

			AEGfxSetTransform(planet_vector[j].debris_draw_array[i].m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}*/
	}
}

void Debris::free()
{

}

void Debris::unload()
{
	AEGfxTextureUnload(debrisTex);
}

std::vector<Debris> Debris::create_debris(f32 planet_x, f32 planet_y, int total_debris) {
	std::vector<Debris> debris_vector;
	for (int i = 0; i < total_debris; i++)
	{
		Debris new_debris;

		new_debris.id = i + 1;
		new_debris.angle = i * 30;
		new_debris.scale_x = 15.f;
		new_debris.scale_y = 15.f;
		new_debris.dist_from_planet = planet_radius + space;
		new_debris.position.x = planet_x + new_debris.dist_from_planet * AECos(AEDegToRad(new_debris.angle));
		new_debris.position.y = planet_y + new_debris.dist_from_planet * AESin(AEDegToRad(new_debris.angle));
		new_debris.turning_speed = speed;
		new_debris.active = true;

		new_debris.scale = { 0 };
		new_debris.rotate = { 0 };
		new_debris.translate = { 0 };
		new_debris.transform = { 0 };

		debris_vector.push_back(new_debris);
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
	return debris_vector;
}

//AEMtx33* draw_debris(Planets planet, int total_debris) {
//	AEMtx33* arr{ new AEMtx33[total_debris] };
//
//	return arr;
//}