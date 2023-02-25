#include "AEEngine.h"
#include "Debris.h"
#include "Planet.h"
#include "Shuttle.h"
#include "WaveManager.h"
#include <cmath>

//VARIABLES
#define SPEED_DEBRIS 10
#define DEBRIS_SIZE 15.f;

int OUTERRIM_TO_DEBRIS = 20;
int DISTANCE_TO_COLLISION_SHUTTLE = 12;
int DISTANCE_TO_COLLISION_DEBRIS = 8;

extern WaveManager wave_manager;
extern Player player;
Debris debris;

//PLANET VECTOR
extern std::vector<Planets> planet_vector;

//SHUTTLE VECTOR
extern std::vector<Shuttles> shuttle_vector;


//VECTOR OF ALL PLANET'S DEBRIS
std::vector<std::vector<Debris>> debris_vector_all;


//PLANET COUNT
extern int planet_count;

//ELAPSED TIME
static f64 elapsed_time{};

//TEXTURE OF DEBRIS
AEGfxTexture* debrisTex;

void Debris::load()
{
	debrisTex = AEGfxTextureLoad("Assets/Debris.png");
}

void Debris::init()
{
	
}

void Debris::update(f64 frame_time)
{

	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			Debris& debris = debris_vector_all[j][i];

			if (debris_vector_all[j][i].move_towards_player) {
				/*f32 t{};
				t += frame_time * (AEVec2Distance(&player.position, &debris.position) / 10.f);

				AEVec2Lerp(&debris.position, &debris.position, &player.position, t);*/

				AEVec2 diff;
				AEVec2Sub(&diff, &player.position, &debris.position);
				AEVec2Normalize(&diff, &diff);
				AEVec2Add(&debris.position, &debris.position, &diff);
			}
			else if (!debris_vector_all[j][i].orbit_around_planet) {
				// Move debris back to orbit
				AEVec2 diff;
				AEVec2Sub(&diff, &planet_vector[j].position, &debris.position);
				AEVec2Normalize(&diff, &diff);
				AEVec2Add(&debris.position, &debris.position, &diff);

				// Debris to rotate around planet when in orbit range
				if (AEVec2Distance(&planet_vector[j].position, &debris.position) <= (planet_vector[j].size / 2.0 + OUTERRIM_TO_DEBRIS)) {
					debris.angle = static_cast<f32>(atan2(debris.position.y - planet_vector[j].position.y, debris.position.x - planet_vector[j].position.x));
					//AEVec2Zero(&velocity);
					debris.move_towards_player = false;
					debris.orbit_around_planet = true;
				}
			}

			if (!debris_vector_all[j][i].move_towards_player && debris_vector_all[j][i].orbit_around_planet) {
				debris.angle -= AEDegToRad(0.125f);

				debris.position.x = planet_vector[j].position.x + ((planet_vector[j].size / 2) + OUTERRIM_TO_DEBRIS) * AECos(debris.angle);
				debris.position.y = planet_vector[j].position.y + ((planet_vector[j].size / 2) + OUTERRIM_TO_DEBRIS) * AESin(debris.angle);
			}
		}
	}

	// ==========================================
	// Collision Check between DEBRIS AND SHUTTLE
	// ==========================================
	for (size_t i{}; i < shuttle_vector.size(); i++) {
		
		for (int k = 0; k < debris_vector_all[shuttle_vector[i].planet_id].size(); ++k) {
			if (shuttle_vector[i].active) {
				if (AEVec2Distance(&shuttle_vector[i].position, &debris_vector_all[shuttle_vector[i].planet_id][k].position) <= DISTANCE_TO_COLLISION_SHUTTLE) { // if collided
					wave_manager.shuttle_has_collided = true;
					wave_manager.shuttle_left_planet++;
					debris_vector_all[shuttle_vector[i].planet_id][k].active = false;
					shuttle_vector[i].active = false;

					spawn_debris(3, shuttle_vector[i].planet_id);
					break;
				}
			}
		}
		
	}

	
	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			if (debris_vector_all[j][i].active==false) {
				debris_vector_all[j].erase(debris_vector_all[j].begin() + i);
			}
		}
	}

	// =======================================
	// calculate the matrix for DEBRIS
	// =======================================
	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			if (debris_vector_all[j][i].active)
			{
				AEMtx33Scale(&debris_vector_all[j][i].scale, debris_vector_all[j][i].size, debris_vector_all[j][i].size);

				AEMtx33Rot(&debris_vector_all[j][i].rotate, AEDegToRad(debris_vector_all[j][i].angle));

				AEMtx33Trans(&debris_vector_all[j][i].translate, debris_vector_all[j][i].position.x, debris_vector_all[j][i].position.y);

				AEMtx33Concat(&debris_vector_all[j][i].transform, &debris_vector_all[j][i].rotate, &debris_vector_all[j][i].scale);
				AEMtx33Concat(&debris_vector_all[j][i].transform, &debris_vector_all[j][i].translate, &debris_vector_all[j][i].transform);

			}
		}
	}



}


void Debris::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(debrisTex, 0, 0);
	
	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			if (debris_vector_all[j][i].active)
			{
				AEGfxSetTransform(debris_vector_all[j][i].transform.m);

				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
}

void Debris::free()
{
	debris_vector_all.clear();
}

void Debris::unload()
{
	AEGfxTextureUnload(debrisTex);
}



// =======================================
// Create Debris vector for each planet
// =======================================
std::vector<Debris> Debris::create_debris(f32 planet_x, f32 planet_y, double size, int total_debris) {
	std::vector<Debris> debris_vector;
	for (int i = 0; i < total_debris; i++)
	{
		Debris new_debris;

		new_debris.id = i + 1;
		new_debris.angle = i * (2 * PI / static_cast<f32>(total_debris));
		new_debris.size = DEBRIS_SIZE;
		new_debris.position.x = planet_x + ((size / 2) + 20) * AECos(AEDegToRad(new_debris.angle));
		new_debris.position.y = planet_y + ((size / 2) + 20) * AESin(AEDegToRad(new_debris.angle));
		new_debris.turning_speed = SPEED_DEBRIS;
		new_debris.active = true;

		new_debris.scale = { 0 };
		new_debris.rotate = { 0 };
		new_debris.translate = { 0 };
		new_debris.transform = { 0 };

		debris_vector.push_back(new_debris);
	}
	debris_vector_all.push_back(debris_vector);
	return debris_vector;
}


void spawn_debris_shuttle(AEVec2 position, int planet_id) {
	Debris new_debris;
	//new_debris.angle = rand();
	new_debris.position.x = position.x;
	new_debris.position.y = position.y;
	new_debris.id = debris_vector_all[planet_id].size() + 1;
	new_debris.size = DEBRIS_SIZE;
	new_debris.turning_speed = SPEED_DEBRIS;
	new_debris.active = true;

	new_debris.scale = { 0 };
	new_debris.rotate = { 0 };
	new_debris.translate = { 0 };
	new_debris.transform = { 0 };

	debris_vector_all[planet_id].push_back(new_debris);

}


bool distance_from_radius(AEVec2 planet_radius, AEVec2 position, int planet_id) { //position of shuttle
	double radius = 0;
	double radius_x;
	double radius_y;
	for (int i = 0; i < debris_vector_all[planet_id].size(); i++) {
		radius_x = pow((debris_vector_all[planet_id][i].position.x - planet_vector[planet_id].position.x), 2);
		radius_y = pow((debris_vector_all[planet_id][i].position.y - planet_vector[planet_id].position.y), 2);
		radius = sqrt((radius_x + radius_y));
	}

	if (AEVec2Distance(&planet_radius, &position) > radius + 15) { //when shuttle move from area to spawn debris
		return true;
	}

}


// =======================================
// FUNCTION TO SPAWN DEBRIS AND CHECK FOR OVERLAP
// =======================================

void spawn_debris(int num_of_debris, int planet_id) {
	if (debris_vector_all[planet_id].size() + num_of_debris < DEBRIS_MAX) {
		srand(3);

		int safe = 0;
		int not_collide = 0;
		int current_count = 0;
		while (current_count != num_of_debris) {

			Debris new_debris;
			new_debris.angle = static_cast<f32>(rand());
			new_debris.position.x = static_cast<f32>(planet_vector[planet_id].position.x + ((planet_vector[planet_id].size / 2) + OUTERRIM_TO_DEBRIS) * AECos(AEDegToRad(new_debris.angle)));
			new_debris.position.y = static_cast<f32>(planet_vector[planet_id].position.y + ((planet_vector[planet_id].size / 2) + OUTERRIM_TO_DEBRIS) * AESin(AEDegToRad(new_debris.angle)));

			for (int k = 0; k < debris_vector_all[planet_id].size(); ++k) {
				if (AEVec2Distance(&new_debris.position, &debris_vector_all[planet_id][k].position) >= DISTANCE_TO_COLLISION_DEBRIS) { //if its colliding when it spawn with nearest debris
					safe += 1;
				}

			}

			if (safe == debris_vector_all[planet_id].size()) {
				not_collide = 1;
			}
			else {
				safe = 0;

			}


			if (not_collide == 1) {
				current_count += 1;
				new_debris.id = debris_vector_all[planet_id].size() + 1;
				new_debris.size = DEBRIS_SIZE;
				new_debris.turning_speed = SPEED_DEBRIS;
				new_debris.active = true;

				new_debris.scale = { 0 };
				new_debris.rotate = { 0 };
				new_debris.translate = { 0 };
				new_debris.transform = { 0 };

				debris_vector_all[planet_id].push_back(new_debris);
				not_collide = 0;

			}

		}


	}
}