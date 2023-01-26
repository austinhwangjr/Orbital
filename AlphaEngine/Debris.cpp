#include "AEEngine.h"
#include "Debris.h"
#include <cmath>
#include "Planet.h"

enum { num_stone=20 };
float planet_radius = 50.f;
float space = 20.f;
static f64 elapsed_time{};
struct Debris array[num_stone];
AEMtx33 debris_array[num_stone];
AEGfxTexture* debrisTex;
AEMtx33 debris_scale{};
AEMtx33 debris_rotate{};
AEMtx33 debris_translate{};
AEMtx33 debris_transform{};


void debris::load()
{
	debrisTex = AEGfxTextureLoad("Assets/Debris.png");
}

void debris::init(f32 planet_x_pos, f32 planet_y_pos, float scale_x, float scale_y, double max_debris )
{
	
	double speed = 0.125f;


	for (int i = 0; i < num_stone; i++) {
		array[i].id = i + 1;
		array[i].angle = 1;
		array[i].scale_x = 15.f;
		array[i].scale_y = 15.f;
		array[i].angle = 1;
		array[i].turning_angle = 0.f + (i * 50);
		array[i].position.x = planet_x_pos + planet_radius + space + (i * 5);
		array[i].position.y = planet_y_pos + planet_radius + space - (i * 5);
		array[i].turning_speed = speed;
		array[i].dist_from_planet = 70.f;
	}

	AEMtx33 debris_scale{};
	AEMtx33 debris_rotate{};
	AEMtx33 debris_translate{};
	AEMtx33 debris_transform{};
	for (int i = 0; i < num_stone; i++) {
		AEMtx33Scale(&debris_scale, array[i].scale_x, array[i].scale_x);
		AEMtx33Rot(&debris_rotate, array[i].angle);
		AEMtx33Trans(&debris_translate, array[i].position.x, array[i].position.y);
		AEMtx33Concat(&debris_transform, &debris_rotate, &debris_scale);
		AEMtx33Concat(&debris_transform, &debris_translate, &debris_transform);
		debris_array[i] = debris_transform;
	}
}

void debris::update(f64 frame_time)
{

	for (int i = 0; i < num_stone; i++) {

		array[i].turning_angle -= array[i].turning_speed;
		

		array[i].position.x = (array[i].dist_from_planet) * AECos(AEDegToRad(array[i].turning_angle));
		array[i].position.y = (array[i].dist_from_planet) * AESin(AEDegToRad(array[i].turning_angle));


		AEGfxTextureSet(debrisTex, 0, 0);
		AEMtx33Scale(&debris_scale, array[i].scale_x, array[i].scale_x);
		AEMtx33Rot(&debris_rotate, array[i].angle);
		AEMtx33Trans(&debris_translate, array[i].position.x, array[i].position.y);
		AEMtx33Concat(&debris_transform, &debris_rotate, &debris_scale);
		AEMtx33Concat(&debris_transform, &debris_translate, &debris_transform);
		debris_array[i] = debris_transform;

	}

}

void debris::draw(AEGfxVertexList* pMesh)
{
	for (int i = 0; i < num_stone; i++) {
		AEGfxSetTransform(debris_array[i].m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void debris::free()
{

}

void debris::unload()
{
	AEGfxTextureUnload(debrisTex);
}