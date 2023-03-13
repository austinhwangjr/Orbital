#include "AEEngine.h"
#include "Debris.h"
#include "Planet.h"
#include "Drone.h"
#include "Shuttle.h"
#include "WaveManager.h"
#include <cmath>

//VARIABLES
#define SPEED_DEBRIS 30
#define MAX_DEBRIS_SIZE 35
#define MIN_DEBRIS_SIZE 15
#define SHUTTLE_SIZE 20
#define MAX_BUFFER 80
#define MIN_BUFFER 30
#define EXPLOSION_WIDTH 40
#define EXPLOSION_HEIGHT 40

int OUTERRIM_TO_DEBRIS = 20;


extern WaveManager wave_manager;
extern Player player;


//PLANET VECTOR
extern std::vector<Planets> planet_vector;

//SHUTTLE VECTOR
extern std::vector<Shuttles> shuttle_vector;

//DRONE VECTOR
extern std::vector<std::vector<Drone>> drone_vector_all;

//VECTOR OF ALL PLANET'S DEBRIS
std::vector<std::vector<Debris>> debris_vector_all;

//TEXTURE OF DEBRIS
AEGfxTexture* debrisTex;

//TEXTURE OF EXPLOSION
AEGfxTexture* explosionTex;

void Debris::load()
{
	debrisTex = AEGfxTextureLoad("Assets/Debris.png");
	explosionTex = AEGfxTextureLoad("Assets/Explosion.png");
}

void Debris::init()
{
	
}

void Debris::update(f64 frame_time)
{
	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			if (debris_vector_all[j][i].active == false && debris_vector_all[j][i].explosion.is_draw == 0) {
				debris_vector_all[j].erase(debris_vector_all[j].begin() + i);
			}
		}
	}

	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			Debris& debris = debris_vector_all[j][i];

			if (debris.state == MOVE_TOWARDS_PLAYER && player.state == PLAYER_ORBIT) {
				// Move debris towards player
				AEVec2 diff;
				AEVec2Sub(&diff, &player.position, &debris.position);
				AEVec2Normalize(&diff, &diff);
				AEVec2Scale(&diff, &diff, (player.beam_level + 1) * 0.4f);
				AEVec2Add(&debris.position, &debris.position, &diff);
			}
			if (debris.state == MOVE_TOWARDS_DRONE) {
				// Move debris towards drone
				AEVec2 diff;
				AEVec2Sub(&diff, &drone_vector_all[j][debris.move_towards_drone_id - 1].position, &debris.position);
				AEVec2Normalize(&diff, &diff);
				AEVec2Scale(&diff, &diff, 1.5f);
				AEVec2Add(&debris.position, &debris.position, &diff);
			}
			if (debris.state == MOVE_TOWARDS_PLANET) {
				// Move debris back to orbit
				AEVec2 diff;
				AEVec2Sub(&diff, &planet_vector[j].position, &debris.position);
				AEVec2Normalize(&diff, &diff);
				AEVec2Add(&debris.position, &debris.position, &diff);

				// Debris to rotate around planet when in orbit range
				if (AEVec2Distance(&planet_vector[j].position, &debris.position) <= (planet_vector[j].size / 2.0 + debris.distance)) {
					debris.angle = static_cast<f32>(atan2(debris.position.y - planet_vector[j].position.y, debris.position.x - planet_vector[j].position.x));
					debris.state = ORBIT_AROUND_PLANET;
				}
			}

			if (debris.state == ORBIT_AROUND_PLANET) {
				// Orbit around planet
				debris.angle -= AEDegToRad(0.125f) * debris.turning_speed * static_cast<f32>(frame_time);
				debris.position.x = planet_vector[j].position.x + ((planet_vector[j].size / 2) + debris.distance) * AECos(debris.angle);
				debris.position.y = planet_vector[j].position.y + ((planet_vector[j].size / 2) + debris.distance) * AESin(debris.angle);
			}
		}
	}

	// ==========================================
	// Collision Check between DEBRIS AND SHUTTLE
	// ==========================================
	for (size_t i{}; i < shuttle_vector.size(); i++) {
		
		for (int k = 0; k < debris_vector_all[shuttle_vector[i].planet_id].size(); ++k) {
			if (shuttle_vector[i].active) {
				if (AEVec2Distance(&shuttle_vector[i].position, &debris_vector_all[shuttle_vector[i].planet_id][k].position) <= (SHUTTLE_SIZE/2 + debris_vector_all[shuttle_vector[i].planet_id][k].size/2)) { // if collided
					wave_manager.shuttle_has_collided = true;
					wave_manager.shuttle_left_planet++;
					debris_vector_all[shuttle_vector[i].planet_id][k].active = false;
					shuttle_vector[i].active = false;
					debris_vector_all[shuttle_vector[i].planet_id][k].explosion.is_draw = 1;
					debris_vector_all[shuttle_vector[i].planet_id][k].explosion.position = debris_vector_all[shuttle_vector[i].planet_id][k].position;
					//spawn_debris(3, shuttle_vector[i].planet_id);
					spawn_debris_shuttle(shuttle_vector[i].position, shuttle_vector[i].planet_id, 4);
					break;
				}
			}
			else {
				break;
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

	// ===================================
	// calculate the matrix for EXPLOSION
	// ===================================
	AEMtx33 scale, rot, trans;
	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {
			Explosion& explosion = debris_vector_all[j][i].explosion;

			if (explosion.is_draw == 1) {

				if (explosion.timer <= explosion.total_time) {
					explosion.timer += frame_time;
					AEMtx33Scale(&scale, explosion.width, explosion.height);
					AEMtx33Rot(&rot, 0);
					AEMtx33Trans(&trans, explosion.position.x, explosion.position.y);
					AEMtx33Concat(&explosion.transform, &rot, &scale);
					AEMtx33Concat(&explosion.transform, &trans, &explosion.transform);
				}
				else {
					explosion.timer = 0;
					explosion.is_draw = 0;
				}
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

	// ===============
	// DRAW EXPLOSION
	// ===============
	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {

			Explosion& explosion = debris_vector_all[j][i].explosion;
			if (explosion.is_draw) {

				if (explosion.timer <= explosion.total_time) {
					AEGfxSetTransparency(explosion.total_time - explosion.timer);

					AEGfxTextureSet(explosionTex, 0, 0);
					AEGfxSetTransform(explosion.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
				else {
					AEGfxSetTransparency(0.f);
				}
			}
		}
	}
	AEGfxSetTransparency(1.f);

}

void Debris::free()
{
	for (int i = 0; i < debris_vector_all.size(); i++) {
		debris_vector_all[i].clear();
	}
	debris_vector_all.clear();
}

void Debris::unload()
{
	AEGfxTextureUnload(debrisTex);
	AEGfxTextureUnload(explosionTex);
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
		new_debris.size = static_cast<f32>(rand() % (MAX_DEBRIS_SIZE - MIN_DEBRIS_SIZE) + MIN_DEBRIS_SIZE);
		new_debris.position.x = planet_x + ((size / 2) + 20) * AECos(AEDegToRad(new_debris.angle));
		new_debris.position.y = planet_y + ((size / 2) + 20) * AESin(AEDegToRad(new_debris.angle));
		new_debris.turning_speed = SPEED_DEBRIS;
		new_debris.active = true;
		new_debris.state = ORBIT_AROUND_PLANET;
		new_debris.distance = OUTERRIM_TO_DEBRIS;

		new_debris.scale = { 0 };
		new_debris.rotate = { 0 };
		new_debris.translate = { 0 };
		new_debris.transform = { 0 };

		new_debris.explosion.height = EXPLOSION_HEIGHT;
		new_debris.explosion.width = EXPLOSION_WIDTH;
		new_debris.explosion.transform = { 0 };
		new_debris.explosion.total_time = 1;

		debris_vector.push_back(new_debris);
	}
	debris_vector_all.push_back(debris_vector);
	return debris_vector;
}

// ==================================================
// FUNCTION TO SPAWN DEBRIS UPON SHUTTLE ESCAPE/DEATH
// ==================================================

void spawn_debris_shuttle(AEVec2 position, int planet_id, int num_of_debris) {
	for (int i = 0; i < num_of_debris; i++) {
		Debris new_debris;
		new_debris.size = static_cast<f32>(rand() % (MAX_DEBRIS_SIZE - MIN_DEBRIS_SIZE) + MIN_DEBRIS_SIZE);
		new_debris.angle = static_cast<f32>(rand() % static_cast<int>(new_debris.size));
		new_debris.position.x = position.x + (-(rand()  % (MAX_BUFFER - MIN_BUFFER)) + new_debris.size);
		new_debris.position.y = position.y + (-(rand() % (MAX_BUFFER - MIN_BUFFER)) + new_debris.size);
		new_debris.id = debris_vector_all[planet_id].size() + 1;
		new_debris.turning_speed = SPEED_DEBRIS;
		new_debris.active = true;
		new_debris.state = MOVE_TOWARDS_PLANET;

		new_debris.scale = { 0 };
		new_debris.rotate = { 0 };
		new_debris.translate = { 0 };
		new_debris.transform = { 0 };
		new_debris.distance = OUTERRIM_TO_DEBRIS + (rand() % MIN_BUFFER);


		//EXPLOSION
		new_debris.explosion.height = EXPLOSION_HEIGHT;
		new_debris.explosion.width = EXPLOSION_WIDTH;
		new_debris.explosion.transform = { 0 };
		new_debris.explosion.total_time = 1;

		debris_vector_all[planet_id].push_back(new_debris);
	}

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


// ==============================================
// FUNCTION TO SPAWN DEBRIS AND CHECK FOR OVERLAP
// ==============================================

void spawn_debris(int num_of_debris, int planet_id) {
	if (debris_vector_all[planet_id].size() + num_of_debris < DEBRIS_MAX) {
		int safe = 0;
		int not_collide = 0;
		int current_count = 0;
		while (current_count != num_of_debris) {

			Debris new_debris;
			new_debris.size = static_cast<f32>(rand() % (MAX_DEBRIS_SIZE - MIN_DEBRIS_SIZE) + MIN_DEBRIS_SIZE);
			new_debris.angle = static_cast<f32>(rand() % static_cast<int>(new_debris.size));
			new_debris.position.x = static_cast<f32>(planet_vector[planet_id].position.x + ((planet_vector[planet_id].size / 2) + OUTERRIM_TO_DEBRIS) * AECos(AEDegToRad(new_debris.angle)));
			new_debris.position.y = static_cast<f32>(planet_vector[planet_id].position.y + ((planet_vector[planet_id].size / 2) + OUTERRIM_TO_DEBRIS) * AESin(AEDegToRad(new_debris.angle)));

			for (int k = 0; k < debris_vector_all[planet_id].size(); ++k) {
				if (AEVec2Distance(&new_debris.position, &debris_vector_all[planet_id][k].position) >= (new_debris.size/2 + debris_vector_all[planet_id][k].size/2)) { //if its colliding when it spawn with nearest debris
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