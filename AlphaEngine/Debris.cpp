/******************************************************************************/
/*!
\file		Debris.cpp
\author 	Leong Wen Bin Aceson (80%)
\co-author	Hwang Jing Rui, Austin (20%)
\par    	email: leong.w\@digipen.edu
			jingruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the Debris.

			Austin - Added debris state and behaviour in each state

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "Debris.h"
#include "Planet.h"
#include "Drone.h"
#include "Shuttle.h"
#include "WaveManager.h"
#include <cmath>
#include "Data.h"
#include "GameStateList.h"

//VARIABLES
static float SPEED_DEBRIS;
static int	 MAX_DEBRIS_SIZE;
static int	 MIN_DEBRIS_SIZE;
static float SHUTTLE_HEIGHT;
static int	 MAX_BUFFER;
static int	 MIN_BUFFER;
static float EXPLOSION_WIDTH;
static float EXPLOSION_HEIGHT;
static int OUTERRIM_TO_DEBRIS;

static int DEBRIS_MAX;

extern WaveManager wave_manager;
extern Player player;

extern Planets MMplanet;

//PLANET DATA MAP
extern std::map<std::string, f32> 	PlanetDataMap;

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


// IMPORT DATA VECTOR
std::map<std::string, f32> 			DebrisDataMap;
std::vector<Data> 					DebrisData;
extern std::map<std::string, f32> 	ShuttleDataMap;

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void Debris::load()
{
	debrisTex = AEGfxTextureLoad("Assets/MainLevel/ml_Debris.png");
	explosionTex = AEGfxTextureLoad("Assets/MainLevel/ml_Explosion.png");
	ImportDataFromFile("Assets/GameObjectData/DebrisData.txt", DebrisData, DebrisDataMap);
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void Debris::init()
{
	DEBRIS_MAX			= static_cast<int>(PlanetDataMap["Maximum_Debris"]);
	SPEED_DEBRIS		= DebrisDataMap["Speed_Of_Debris"];
	MAX_DEBRIS_SIZE		= static_cast<int>(DebrisDataMap["Maximum_Debris_Size"]);
	MIN_DEBRIS_SIZE		= static_cast<int>(DebrisDataMap["Minimum_Debris_Size"]);
	SHUTTLE_HEIGHT		= ShuttleDataMap["Shuttle_Height"];
	MAX_BUFFER			= static_cast<int>(DebrisDataMap["Maximum_Buffer"]);
	MIN_BUFFER			= static_cast<int>(DebrisDataMap["Minimum_Buffer"]);
	EXPLOSION_WIDTH		= DebrisDataMap["Explosion_Width"];
	EXPLOSION_HEIGHT	= DebrisDataMap["Explosion_Height"];
	OUTERRIM_TO_DEBRIS	= static_cast<int>(DebrisDataMap["Distance_To_Planet"]);
}

/******************************************************************************/
/*!
	Update Debris
*/
/******************************************************************************/
void Debris::update()
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
				debris.angle -= AEDegToRad(0.125f) * debris.turning_speed * g_dt;
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
				if (AEVec2Distance(&shuttle_vector[i].position, &debris_vector_all[shuttle_vector[i].planet_id][k].position) <= ((SHUTTLE_HEIGHT -80) /2 + debris_vector_all[shuttle_vector[i].planet_id][k].size/2)) { // if collided
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
	AEMtx33 sc, rot, trans;
	for (int j = 0; j < debris_vector_all.size(); j++) {
		for (size_t i = 0; i < debris_vector_all[j].size(); i++) {

			Explosion& debris_explosion = debris_vector_all[j][i].explosion;
			if (debris_explosion.is_draw == 1) {

				if (debris_explosion.timer <= debris_explosion.total_time) {
					debris_explosion.timer += g_dt;
					AEMtx33Scale(&sc, debris_explosion.width, debris_explosion.height);
					AEMtx33Rot(&rot, 0);
					AEMtx33Trans(&trans, debris_explosion.position.x, debris_explosion.position.y);
					AEMtx33Concat(&debris_explosion.transform, &rot, &sc);
					AEMtx33Concat(&debris_explosion.transform, &trans, &debris_explosion.transform);
				}
				else {
					debris_explosion.timer = 0;
					debris_explosion.is_draw = 0;
				}
			}
		}
	}
}

/******************************************************************************/
/*!
	Draw Debris
*/
/******************************************************************************/
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

			Explosion& debris_explosion = debris_vector_all[j][i].explosion;
			if (debris_explosion.is_draw) {

				if (debris_explosion.timer <= debris_explosion.total_time) {
					AEGfxSetTransparency(debris_explosion.total_time - debris_explosion.timer);

					AEGfxTextureSet(explosionTex, 0, 0);
					AEGfxSetTransform(debris_explosion.transform.m);
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

/******************************************************************************/
/*!
	Clean Object Instances
*/
/******************************************************************************/
void Debris::free()
{
	for (int i = 0; i < debris_vector_all.size(); i++) {
		debris_vector_all[i].clear();
	}
	debris_vector_all.clear();

	if (next_state != GS_RESTART) {
		DebrisData.clear();
		DebrisDataMap.clear();
	}
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void Debris::unload()
{
	AEGfxTextureUnload(debrisTex);
	AEGfxTextureUnload(explosionTex);
}

// =======================================
// Create Debris vector for each planet
// =======================================
std::vector<Debris> Debris::create_debris(f32 planet_x, f32 planet_y, double new_size, int total_debris) {
	std::vector<Debris> new_debris_vector;
	for (int i = 0; i < total_debris; i++)
	{
		Debris new_debris;

		new_debris.id = i + 1;
		new_debris.angle = i * (2 * PI / static_cast<f32>(total_debris));
		new_debris.size = static_cast<f32>(rand() % (MAX_DEBRIS_SIZE - MIN_DEBRIS_SIZE) + MIN_DEBRIS_SIZE);
		new_debris.position.x = static_cast<f32>(planet_x + ((new_size / 2) + 20) * AECos(AEDegToRad(new_debris.angle)));
		new_debris.position.y = static_cast<f32>(planet_y + ((new_size / 2) + 20) * AESin(AEDegToRad(new_debris.angle)));
		new_debris.turning_speed = SPEED_DEBRIS;
		new_debris.active = true;
		new_debris.state = ORBIT_AROUND_PLANET;
		new_debris.distance = static_cast<f32>(OUTERRIM_TO_DEBRIS);

		new_debris.scale = { 0 };
		new_debris.rotate = { 0 };
		new_debris.translate = { 0 };
		new_debris.transform = { 0 };

		new_debris.explosion.height = EXPLOSION_HEIGHT;
		new_debris.explosion.width = EXPLOSION_WIDTH;
		new_debris.explosion.transform = { 0 };
		new_debris.explosion.total_time = 1;

		new_debris_vector.push_back(new_debris);
	}
	debris_vector_all.push_back(new_debris_vector);
	return new_debris_vector;
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
		new_debris.id = static_cast<int>(debris_vector_all[planet_id].size() + 1);
		new_debris.turning_speed = SPEED_DEBRIS;
		new_debris.active = true;
		new_debris.state = MOVE_TOWARDS_PLANET;

		new_debris.scale = { 0 };
		new_debris.rotate = { 0 };
		new_debris.translate = { 0 };
		new_debris.transform = { 0 };
		new_debris.distance = static_cast<f32>(OUTERRIM_TO_DEBRIS + (rand() % MIN_BUFFER));


		//EXPLOSION
		new_debris.explosion.height = EXPLOSION_HEIGHT;
		new_debris.explosion.width = EXPLOSION_WIDTH;
		new_debris.explosion.transform = { 0 };
		new_debris.explosion.total_time = 1;

		debris_vector_all[planet_id].push_back(new_debris);
	}
}
