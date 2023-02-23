#include "AEEngine.h"
#include "Player.h"

// Textures
AEGfxTexture* player_tex;
AEGfxTexture* tractor_beam_tex;

// Variables
bool beam_active = false;

// Vectors of planets and debris
extern std::vector<Planets> planet_vector;
extern std::vector<std::vector<Debris>> debris_vector_all;

void Player::load()
{
	// Load textures
	player_tex			= AEGfxTextureLoad("Assets/test-player.png");
	tractor_beam_tex	= AEGfxTextureLoad("Assets/TractorBeam.png");
}

void Player::init()
{
	// Player
	state					= PLAYER_FLY;

	position.x				= 100.f;
	position.y				= 100.f;

	velocity.x				= 0.f;
	velocity.y				= 0.f;

	size					= 20.f;

	mov_speed				= 150.f;
	rot_speed				= 1.5f * PI;
	speed_upgrade			= 1.0f;

	dist_from_planet		= 50.f;
	shortest_distance		= 0.f;

	direction				= 0.f;

	current_capacity		= 0;
	max_capacity			= 5;

	// Score-keeping
	score					= 0;
	credits					= 500;

	can_leave_orbit			= true;

	// Tractor beam
	beam_pos.x				= 0.f;
	beam_pos.y				= 0.f;

	beam_collision_pos.x	= 0.f;
	beam_collision_pos.y	= 0.f;

	beam_width				= size;
}

void Player::update(f64 frame_time)
{
	// Player is in orbit mode
	if (state == PLAYER_ORBIT)
		orbit_state(frame_time);

	// Player is in free-flying mode
	if (state == PLAYER_FLY)
		flying_state(frame_time);

	// =========================================
	// Calculate the matrix for player and beam
	// =========================================
	
	AEMtx33 scale, rot, trans;

	// Player
	AEMtx33Scale(&scale, size, size);
	AEMtx33Rot(&rot, direction + PI / 2);
	AEMtx33Trans(&trans, position.x, position.y);
	AEMtx33Concat(&player_transform, &rot, &scale);
	AEMtx33Concat(&player_transform, &trans, &player_transform);

	// Tractor beam
	AEMtx33Scale(&scale, size, size * 3 / 2);
	AEMtx33Rot(&rot, direction + PI / 2);
	AEMtx33Trans(&trans, beam_pos.x, beam_pos.y);
	AEMtx33Concat(&beam_transform, &rot, &scale);
	AEMtx33Concat(&beam_transform, &trans, &beam_transform);
}

/******************************************************************************/
/*!
	Draw player and tractor beam
*/
/******************************************************************************/
void Player::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(player_tex, 0, 0);
	AEGfxSetTransform(player_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// tractor beam
	if (AEInputCheckCurr(AEVK_SPACE) && state == PLAYER_ORBIT) {
		AEGfxTextureSet(tractor_beam_tex, 0, 0);
		AEGfxSetTransform(beam_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Player::free()
{

}

void Player::unload()
{
	AEGfxTextureUnload(player_tex);
	AEGfxTextureUnload(tractor_beam_tex);
}

void Player::orbit_state(f64 frame_time)
{
	// ================
	// Check for input
	// ================
	if (AEInputCheckCurr(AEVK_A)) {
		direction += (rot_speed / 2) * speed_upgrade * static_cast<f32>(frame_time);

		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2 + dist_from_planet) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2 + dist_from_planet) * AESin(direction);
	}

	if (AEInputCheckCurr(AEVK_D)) {
		direction -= (rot_speed / 2) * speed_upgrade * static_cast<f32>(frame_time);

		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2 + dist_from_planet) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2 + dist_from_planet) * AESin(direction);
	}

	if (AEInputCheckPrev(AEVK_W))
		can_leave_orbit = false;
	else
		can_leave_orbit = true;

	if (AEInputCheckCurr(AEVK_W) && can_leave_orbit) {
		AEVec2Zero(&velocity);
		state = PLAYER_FLY;
	}

	// Draw tractor beam
	if (AEInputCheckCurr(AEVK_SPACE))
		beam_active = true;
	else
		beam_active = false;

	// ================================
	// Update player and beam position
	// ================================

	if (AEInputCheckCurr(AEVK_W) && can_leave_orbit) {
		position.x += AECos(direction);
		position.y += AESin(direction);
	}
	else {
		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2.f + dist_from_planet) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2.f + dist_from_planet) * AESin(direction);
	}

	if (beam_active) {
		beam_pos.x = position.x - AECos(direction) * 20;
		beam_pos.y = position.y - AESin(direction) * 20;

		beam_collision_pos.x = beam_pos.x - AECos(direction) * 10;
		beam_collision_pos.y = beam_pos.y - AESin(direction) * 10;
	}

	// ================================
	// Check for beam-debris collision
	// ================================
	
	//if (beam_active) {
	//	// Check for collision between tractor beam and debris
	//	for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
	//		if (current_capacity < max_capacity && AEVec2Distance(&beam_collision_pos, &debris_vector_all[current_planet.id][i].position) <= beam_width / 2) {
	//			debris_vector_all[current_planet.id][i].to_erase = true;
	//			break;
	//		}
	//	}
	//}
	if (beam_active) {
		// Check for collision between tractor beam and debris
		for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
			Debris& debris = debris_vector_all[current_planet.id][i];
			
			// Debris to move towards player when in contact with beam
			if (current_capacity < max_capacity && AEVec2Distance(&beam_collision_pos, &debris.position) <= beam_width / 2) {
				debris.move_towards_player = true;
				break;
			}

			if (AEVec2Distance(&position, &debris.position) <= (size + debris.size) / 2) {
				// Debris to move towards player
				debris_vector_all[current_planet.id][i].to_erase = true;
				break;
			}
		}
	}

	// ===================================
	// Update player and beam instances
	// ===================================

	

	// ===========================
	// Remove debris to be erased
	// ===========================
	for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
		

		if (debris_vector_all[current_planet.id][i].to_erase) {
			debris_vector_all[current_planet.id].erase(debris_vector_all[current_planet.id].begin() + i);
			current_capacity++;
		}
	}
}

void Player::flying_state(f64 frame_time)
{
	// ================
	// Check for input
	// ================
	if (AEInputCheckCurr(AEVK_W)) {
		AEVec2 added;
		AEVec2Set(&added, AECos(direction), AESin(direction));

		// Find the velocity according to the acceleration
		// Limit your speed over here
		AEVec2Scale(&added, &added, mov_speed * speed_upgrade);
		velocity.x = velocity.x + added.x * static_cast<f32>(frame_time);
		velocity.y = velocity.y + added.y * static_cast<f32>(frame_time);
		AEVec2Scale(&velocity, &velocity, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_S)) {
		AEVec2 added;
		AEVec2Set(&added, -AECos(direction), -AESin(direction));

		// Find the velocity according to the decceleration
		// Limit your speed over here
		AEVec2Scale(&added, &added, mov_speed * speed_upgrade);
		velocity.x = velocity.x + added.x * static_cast<f32>(frame_time);
		velocity.y = velocity.y + added.y * static_cast<f32>(frame_time);
		AEVec2Scale(&velocity, &velocity, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_A)) {
		direction += rot_speed * speed_upgrade * static_cast<f32>(frame_time);
		direction = AEWrap(direction, -PI, PI);
	}

	if (AEInputCheckCurr(AEVK_D)) {
		direction -= rot_speed * speed_upgrade * static_cast<f32>(frame_time);
		direction = AEWrap(direction, -PI, PI);
	}

	if (AEVec2Distance(&current_planet.position, &position) <= (current_planet.size / 2 + dist_from_planet)) {
		//direction = atan2(position.y - current_planet.position.y, position.x - current_planet.position.x);
		direction = static_cast<f32>(atan2(position.y - current_planet.position.y, position.x - current_planet.position.x));
		//AEVec2Zero(&velocity);
		state = PLAYER_ORBIT;
	}

	// =======================
	// Update player position
	// =======================
	position.x = position.x + velocity.x * static_cast<f32>(frame_time);
	position.y = position.y + velocity.y * static_cast<f32>(frame_time);

	// ====================
	// Check for collision
	// ====================

	

	// ===================================
	// Update active game object instances
	// ===================================

	// Determine planet closest to player
	current_planet = planet_vector[0];
	shortest_distance = AEVec2Distance(&position, &planet_vector[0].position);

	for (size_t i = 1; i < planet_vector.size(); ++i) {
		if (AEVec2Distance(&position, &planet_vector[i].position) < shortest_distance) {
			current_planet = planet_vector[i];
			shortest_distance = AEVec2Distance(&position, &planet_vector[i].position);
		}
	}
}