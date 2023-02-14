#include "AEEngine.h"
#include "Debris.h"
#include "Planet.h"
#include "Shuttle.h"
#include "WaveManager.h"
#include <cmath>

Debris debris;

extern std::vector<Planets> planet_vector;
extern std::vector<Shuttles> shuttle_vector;

extern WaveManager wave_manager;

std::vector<std::vector<Debris>> debris_vector_all;

//enum { num_stone=20 };
float planet_radius = 50.f;
float space = 20.f;
extern int planet_count;

static f64 elapsed_time{};

AEGfxTexture* debrisTex;

f32 collision_shuttle_x;
f32 collision_shuttle_y;

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
	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			debris_vector_all[j][i].angle -= 1.5f;

			debris_vector_all[j][i].position.x = planet_vector[j].position.x + (space + planet_radius) * AECos(AEDegToRad(debris_vector_all[j][i].angle));
			debris_vector_all[j][i].position.y = planet_vector[j].position.y + (space + planet_radius) * AESin(AEDegToRad(debris_vector_all[j][i].angle));

		}
	}

	//collision check for debris
	for (size_t i{}; i < shuttle_vector.size(); i++) {
		collision_shuttle_x = shuttle_vector[i].position.x;
		collision_shuttle_y = shuttle_vector[i].position.y;

		AEVec2 coll_shuttle_pos;
		coll_shuttle_pos.x = collision_shuttle_x;
		coll_shuttle_pos.y = collision_shuttle_y;
		for (int j = 0; j < debris_vector_all.size(); j++) {
			for (int k = 0; k < debris_vector_all[j].size(); ++k) {
				if (AEVec2Distance(&coll_shuttle_pos, &debris_vector_all[j][k].position) <= 10) {
					wave_manager.shuttle_has_collided = true;
					wave_manager.shuttle_left_planet++;
					debris_vector_all[j].erase(debris_vector_all[j].begin() + k);
					shuttle_vector.erase(shuttle_vector.begin()+i);
					break;
				}
			}
		}
	}

}

void Debris::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(debrisTex, 0, 0);
	//for (size_t j = 0; j < planet_vector.size(); j++) {
	//	for (size_t i = 0; i < planet_vector[j].debris_vector.size(); i++) {

	//		/*AEMtx33Scale(&planet_vector[j].debris_vector[i].scale, planet_vector[j].debris_vector[i].scale_x, planet_vector[j].debris_vector[i].scale_y);

	//		AEMtx33Rot(&planet_vector[j].debris_vector[i].rotate, AEDegToRad(planet_vector[j].debris_vector[i].angle));

	//		AEMtx33Trans(&planet_vector[j].debris_vector[i].translate, planet_vector[j].debris_vector[i].position.x, planet_vector[j].debris_vector[i].position.y);

	//		AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].rotate, &planet_vector[j].debris_vector[i].scale);

	//		AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].translate, &planet_vector[j].debris_vector[i].transform);

	//		planet_vector[j].debris_draw_array[i] = planet_vector[j].debris_vector[i].transform;

	//		AEGfxSetTransform(planet_vector[j].debris_draw_array[i].m);
	//		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/
	//		if (planet_vector[j].debris_vector[i].active)
	//		{
	//			//AEMtx33 scale = { 0 };
	//			AEMtx33Scale(&planet_vector[j].debris_vector[i].scale, planet_vector[j].debris_vector[i].scale_x, planet_vector[j].debris_vector[i].scale_y);

	//			//AEMtx33 rotate = { 0 };
	//			AEMtx33Rot(&planet_vector[j].debris_vector[i].rotate, AEDegToRad(planet_vector[j].debris_vector[i].angle));

	//			//AEMtx33 translate = { 0 };
	//			AEMtx33Trans(&planet_vector[j].debris_vector[i].translate, planet_vector[j].debris_vector[i].position.x, planet_vector[j].debris_vector[i].position.y);

	//			//AEMtx33 transform = { 0 };
	//			AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].rotate, &planet_vector[j].debris_vector[i].scale);
	//			AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].translate, &planet_vector[j].debris_vector[i].transform);

	//			AEGfxSetTransform(planet_vector[j].debris_vector[i].transform.m);

	//			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	//		}
	//	}
	//	/*for (int i = 0; i < planet_vector[j].max_debris; i++) {


	//		AEMtx33Scale(&planet_vector[j].debris_vector[i].scale, planet_vector[j].debris_vector[i].scale_x, planet_vector[j].debris_vector[i].scale_y);

	//		AEMtx33Rot(&planet_vector[j].debris_vector[i].rotate, AEDegToRad(planet_vector[j].debris_vector[i].angle));

	//		AEMtx33Trans(&planet_vector[j].debris_vector[i].translate, planet_vector[j].debris_vector[i].position.x, planet_vector[j].debris_vector[i].position.y);

	//		AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].rotate, &planet_vector[j].debris_vector[i].scale);

	//		AEMtx33Concat(&planet_vector[j].debris_vector[i].transform, &planet_vector[j].debris_vector[i].translate, &planet_vector[j].debris_vector[i].transform);

	//		planet_vector[j].debris_draw_array[i] = planet_vector[j].debris_vector[i].transform;

	//		AEGfxSetTransform(planet_vector[j].debris_draw_array[i].m);
	//		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	//	}*/
	//}

	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			if (debris_vector_all[j][i].active)
			{
				AEMtx33Scale(&debris_vector_all[j][i].scale, debris_vector_all[j][i].scale_x, debris_vector_all[j][i].scale_y);

				AEMtx33Rot(&debris_vector_all[j][i].rotate, AEDegToRad(debris_vector_all[j][i].angle));

				AEMtx33Trans(&debris_vector_all[j][i].translate, debris_vector_all[j][i].position.x, debris_vector_all[j][i].position.y);

				AEMtx33Concat(&debris_vector_all[j][i].transform, &debris_vector_all[j][i].rotate, &debris_vector_all[j][i].scale);
				AEMtx33Concat(&debris_vector_all[j][i].transform, &debris_vector_all[j][i].translate, &debris_vector_all[j][i].transform);

				AEGfxSetTransform(debris_vector_all[j][i].transform.m);

				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
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
	debris_vector_all.push_back(debris_vector);

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