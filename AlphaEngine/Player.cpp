#include "AEEngine.h"
#include "Player.h"
#include "Easing.h"

// Textures
AEGfxTexture* player_tex;
AEGfxTexture* tractor_beam_tex;
AEGfxTexture* orbit_halo_tex;

float animationDuration = 1.0f; // Duration of the animation in seconds
float elapsedTime = 0.0f; // Elapsed time for the animation
// Variables
bool beam_active = false;

// Vectors of planets and debris
extern std::vector<Planets> planet_vector;
extern std::vector<std::vector<Debris>> debris_vector_all;

void Player::load()
{
	// Load textures
	player_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_Spaceship2.png");
	tractor_beam_tex	= AEGfxTextureLoad("Assets/MainLevel/ml_TractorBeam.png");
	orbit_halo_tex		= AEGfxTextureLoad("Assets/MainLevel/neonCircle.png");
}

void Player::init()
{
	//--------------------Player--------------------
	state					= PLAYER_FLY;

	position.x				= 100.f;
	position.y				= 100.f;

	velocity.x				= 0.f;
	velocity.y				= 0.f;

	size					= 50.f;

	mov_speed				= 150.f;
	rot_speed				= 0.85f * PI;

	shortest_distance		= 0.f;

	direction				= 0.f;

	current_capacity		= 0;
	max_capacity			= 5;

	can_leave_orbit			= true;

	timer					= 0.f;

	//--------------------Score-keeping--------------------
	score					= 0;
	credits					= 200;

	//--------------------Upgrade Levels--------------------
	mov_speed_level			= 0;
	capacity_level			= 0;
	space_station_count		= 0;
	beam_level				= 0;

	//--------------------Tractor Beam--------------------
	beam_pos.x				= 0.f;
	beam_pos.y				= 0.f;

	beam_width				= size * 0.6f;
	beam_height				= beam_width * 2.f;

	//--------------------Planet Halo--------------------
	halo_scale_lerp			= 0.0f;

}

void Player::update(f64 frame_time)
{
	elapsedTime += static_cast<float>(AEFrameRateControllerGetFrameTime());

	// Player is in orbit state
	if (state == PLAYER_ORBIT)
		orbit_state(frame_time);

	// Player is in transit state
	if (state == PLAYER_TRANSIT)
		transit_state(frame_time);

	// Player is in free-flying state
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
	AEMtx33Scale(&scale, beam_width, beam_height);
	AEMtx33Rot(&rot, direction + PI / 2);
	AEMtx33Trans(&trans, beam_pos.x, beam_pos.y);
	AEMtx33Concat(&beam_transform, &rot, &scale);
	AEMtx33Concat(&beam_transform, &trans, &beam_transform);

static bool isOrbitingPlanet = false; // flag for is orbiting.. pretty sure suppose to be init, but placeholder! :(

	if (state == PLAYER_ORBIT)
	{
		// Check if the spaceship just started orbiting a planet
		if (!isOrbitingPlanet)
		{
		halo_scale_lerp = 0; // Reset the Lerp value for halo scale
		isOrbitingPlanet = true; 
		}

		// Calculate progress based on the elapsed time and animation duration
		float progress = elapsedTime / animationDuration;
		if (progress > 1.0f)
		{
			progress = 1.0f; // Clamp progress to 1.0f to prevent overshooting
		}

		// Use the EaseInOutBack easing function for smooth interpolation
		float easedProgress = EaseInOutBack(0, 1, progress);

		// Update the Lerp value for the halo scale
		halo_scale_lerp += (1.0f - halo_scale_lerp) * 0.1f;

		f32 val{ current_planet.size + 60.f };

		// Use the Lerp value to scale the halo
		AEMtx33Scale(&scale, val * halo_scale_lerp, val * halo_scale_lerp);
		AEMtx33Trans(&trans, current_planet.position.x, current_planet.position.y);
		AEMtx33Concat(&orbit_halo_transform, &rot, &scale);
		AEMtx33Concat(&orbit_halo_transform, &trans, &orbit_halo_transform);
	}
	else
	{
		// The spaceship is not orbiting a planet, so set the flag to false
		isOrbitingPlanet = false;
	}

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

	if (state == PLAYER_ORBIT) {
		AEGfxTextureSet(orbit_halo_tex, 0, 0);
		AEGfxSetTransform(orbit_halo_transform.m);
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
	AEGfxTextureUnload(orbit_halo_tex);
}

void Player::orbit_state(f64 frame_time)
{
	// ================
	// Check for input
	// ================

	if (AEInputCheckCurr(AEVK_A)) {
		direction += (rot_speed / 2) * static_cast<f32>(frame_time);

		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2 + current_planet.orbit_range) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2 + current_planet.orbit_range) * AESin(direction);
	}

	if (AEInputCheckCurr(AEVK_D)) {
		direction -= (rot_speed / 2) * static_cast<f32>(frame_time);

		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2 + current_planet.orbit_range) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2 + current_planet.orbit_range) * AESin(direction);
	}

	if (AEInputCheckPrev(AEVK_W))
		can_leave_orbit = false;
	else
		can_leave_orbit = true;

	if (AEInputCheckCurr(AEVK_W) && can_leave_orbit) {
		/*AEVec2Zero(&velocity);
		state = PLAYER_FLY;*/

		AEVec2Zero(&velocity);
		state = PLAYER_TRANSIT;
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
		position.x = current_planet.position.x + (static_cast<f32>(current_planet.size) / 2.f + current_planet.orbit_range) * AECos(direction);
		position.y = current_planet.position.y + (static_cast<f32>(current_planet.size) / 2.f + current_planet.orbit_range) * AESin(direction);
	}

	if (beam_active) {
		beam_pos.x = position.x - AECos(direction) * ((beam_height + size) / 2);
		beam_pos.y = position.y - AESin(direction) * ((beam_height + size) / 2);
	}

	// ================================
	// Check for beam-debris collision
	// ================================
	
	if (beam_active) {
		// Check for collision between tractor beam and debris
		for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
			Debris& debris = debris_vector_all[current_planet.id][i];
			
			if (debris.active) {
				// Debris to move towards player when in contact with beam	
				if ((current_capacity < max_capacity + capacity_level) && 
					AEVec2Distance(&beam_pos, &debris.position) <= beam_height / 2) {
					debris.state = MOVE_TOWARDS_PLAYER;
				}
				// Beam active, but not colliding with player beam or drone beam
				else if (debris.state == MOVE_TOWARDS_PLAYER && debris.state != MOVE_TOWARDS_DRONE)
					debris.state = MOVE_TOWARDS_PLANET;

				if ((current_capacity < max_capacity + capacity_level) && AEVec2Distance(&position, &debris.position) <= (size + debris.size) / 2) {
					// Debris to be destroyed when in contact with player
					current_capacity++;
					debris.active = false;
				}
			}
		}
	}
	else {
		for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
			Debris& debris = debris_vector_all[current_planet.id][i];
			// Beam no longer active, if debris was moving towards player, make it move towards planet
			if (debris.state == MOVE_TOWARDS_PLAYER)
				debris.state = MOVE_TOWARDS_PLANET;
		}
	}
}

void Player::transit_state(f64 frame_time)
{
	// ================
	// Check for input
	// ================

	if (AEInputCheckCurr(AEVK_W)) {
		AEVec2 added;
		AEVec2Set(&added, AECos(direction), AESin(direction));

		// Find the velocity according to the acceleration
		AEVec2Scale(&added, &added, mov_speed / 2.f);
		velocity.x = velocity.x + added.x * static_cast<f32>(frame_time);
		velocity.y = velocity.y + added.y * static_cast<f32>(frame_time);

		// Limit player's speed
		AEVec2Scale(&velocity, &velocity, 0.99f);

		// Add to timer. Change to flying state after 1s
		timer += static_cast<f32>(frame_time);
		if (timer >= 1.f) {
			// Change state and reset timer
			state = PLAYER_FLY;
			timer = 0.f;
		}
	}
	
	else {
		// Move player back to orbit
		AEVec2 diff;
		AEVec2Sub(&diff, &current_planet.position, &position);
		AEVec2Normalize(&diff, &diff);
		AEVec2Scale(&diff, &diff, mov_speed * static_cast<f32>(frame_time));
		AEVec2Add(&position, &position, &diff);

		timer -= static_cast<f32>(frame_time);

		// Debris to rotate around planet when in orbit range
		if (AEVec2Distance(&current_planet.position, &position) <= (current_planet.size / 2 + current_planet.orbit_range)) {
			// Change state and reset timer
			state = PLAYER_ORBIT;
			timer = 0.f;
		}
	}

	// =======================
	// Update player position
	// =======================

	position.x = position.x + velocity.x * static_cast<f32>(frame_time);
	position.y = position.y + velocity.y * static_cast<f32>(frame_time);
}

void Player::flying_state(f64 frame_time)
{
	// ================
	// Check for input
	// ================

	if (AEInputCheckCurr(AEVK_W))
	{
		AEVec2 added;
		AEVec2Set(&added, AECos(direction), AESin(direction));

		// Find the velocity according to the acceleration
		f32 easing = easeOutQuad(static_cast<f32>(frame_time));
		AEVec2Scale(&added, &added, mov_speed * static_cast<f32>(mov_speed_level + 1) / 2.f * easing);
		velocity.x = velocity.x + added.x;
		velocity.y = velocity.y + added.y;

		// Limit player's speed
		AEVec2Scale(&velocity, &velocity, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_S))
	{
		AEVec2 added;
		AEVec2Set(&added, -AECos(direction), -AESin(direction));

		// Find the velocity according to the decceleration
		f32 easing = easeOutQuad(static_cast<f32>(frame_time));
		AEVec2Scale(&added, &added, mov_speed * static_cast<f32>(mov_speed_level + 1) / 2.f * easing);
		velocity.x = velocity.x + added.x;
		velocity.y = velocity.y + added.y;

		// Limit player's speed
		AEVec2Scale(&velocity, &velocity, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_A)) {
		direction += rot_speed * static_cast<f32>(frame_time);
		direction = AEWrap(direction, -PI, PI);
	}

	if (AEInputCheckCurr(AEVK_D)) {
		direction -= rot_speed * static_cast<f32>(frame_time);
		direction = AEWrap(direction, -PI, PI);
	}

	if (AEVec2Distance(&current_planet.position, &position) <= (current_planet.size / 2 + current_planet.orbit_range)) {
		direction = static_cast<f32>(atan2(position.y - current_planet.position.y, position.x - current_planet.position.x));
		state = PLAYER_ORBIT;
	}

	// =======================
	// Update player position
	// =======================

	position.x = position.x + velocity.x * static_cast<f32>(frame_time);
	position.y = position.y + velocity.y * static_cast<f32>(frame_time);	

	// ===================================
	// Update active game object instances
	// ===================================

	/* if (0 <= Halo_Timer)
	{
		Halo_Timer -= frame_time;
	}*/

	// Determine planet closest to player
	current_planet = planet_vector[0];
	shortest_distance = AEVec2Distance(&position, &planet_vector[0].position);

	for (size_t i = 1; i < planet_vector.size(); ++i) {
		if (AEVec2Distance(&position, &planet_vector[i].position) < shortest_distance) {
			current_planet = planet_vector[i];
			shortest_distance = AEVec2Distance(&position, &planet_vector[i].position);
		}
	}

	// Player not in planet orbit, debris to return to planet
	for (int i = 0; i < debris_vector_all[current_planet.id].size(); ++i) {
		Debris& debris = debris_vector_all[current_planet.id][i];
		if (debris.state == MOVE_TOWARDS_PLAYER)
			debris.state = MOVE_TOWARDS_PLANET;
	}
}