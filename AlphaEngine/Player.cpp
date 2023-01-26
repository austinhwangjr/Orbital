#include "AEEngine.h"
#include "Player.h"
#include <iostream>


AEGfxTexture* playerTex;

extern Player player;

f32 dist_from_planet_centre;
f32 player_speed_scale;

bool player_leave_orbit;

// Test for multiple planet check
f32 shortest_distance;

void Player::load()
{
	// load texture
	playerTex = AEGfxTextureLoad("Assets/test-player.png");
}

void Player::init()
{
	player.position.x = 100.f;
	player.position.y = 100.f;
	player.angle = 0.f;

	// variables
	dist_from_planet_centre = 75.f;
	player_speed_scale = 5.f;

	player.state = PLAYER_ORBIT;
	player_leave_orbit = true;

	shortest_distance = 0.f;
}

void Player::update(f64 frame_time)
{
	if (player.position.x <= AEGfxGetWinMinX() - 10) player.position.x = AEGfxGetWinMinX() - 10;
	else if (player.position.x >= AEGfxGetWinMaxX() + 10) player.position.x = AEGfxGetWinMaxX() + 10;
	if (player.position.y <= AEGfxGetWinMinY() - 10) player.position.y = AEGfxGetWinMinY() - 10;
	else if (player.position.y >= AEGfxGetWinMaxY() + 10) player.position.y = AEGfxGetWinMaxY() + 10;


	// Change player state
	AEVec2 planet_pos;
	planet_pos.x = 1;
	planet_pos.y = 0;

	if (player.state == PLAYER_ORBIT) {
		player.position.x = player.current_planet.position.x + dist_from_planet_centre * AECos(AEDegToRad(player.angle));
		player.position.y = player.current_planet.position.y + dist_from_planet_centre * AESin(AEDegToRad(player.angle));

		if (AEInputCheckCurr(AEVK_A)) {
			player.angle += 3.f;
		}

		if (AEInputCheckCurr(AEVK_D)) {
			player.angle -= 3.f;
		}

		if (AEInputCheckPrev(AEVK_W)) {
			player_leave_orbit = false;
		}
		else player_leave_orbit = true;
		if (AEInputCheckCurr(AEVK_W) && player_leave_orbit) {

			player.position.x = player.current_planet.position.x + ((player.position.x - player.current_planet.position.x) * 1.6f);
			player.position.y = player.current_planet.position.y + ((player.position.y - player.current_planet.position.y) * 1.6f);

			player.state = PLAYER_FLY;
		}
	}

	else if (player.state == PLAYER_FLY) {

		if (planet_vector.size() > 0) {

			player.current_planet = planet_vector[0];
			shortest_distance = AEVec2Distance(&player.position, &planet_vector[0].position);

			for (size_t i = 1; i < planet_vector.size(); ++i) {
				if (AEVec2Distance(&player.position, &planet_vector[i].position) < shortest_distance) {
					player.current_planet = planet_vector[i];
					shortest_distance = AEVec2Distance(&player.position, &planet_vector[i].position);
				}
			}
		}

		AEVec2 player_dir_vector;
		player_dir_vector.x = AECos(AEDegToRad(player.angle));
		player_dir_vector.y = AESin(AEDegToRad(player.angle));
		AEVec2Normalize(&player_dir_vector, &player_dir_vector);

		if (AEInputCheckCurr(AEVK_W)) {
			player.position.x += player_dir_vector.x * player_speed_scale;
			player.position.y += player_dir_vector.y * player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_A)) {

			player.angle += player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_S)) {
			player_dir_vector.x -= player_dir_vector.x * player_speed_scale;
			player_dir_vector.y -= player_dir_vector.y * player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_D)) {

			player.angle -= player_speed_scale;
		}

		if (AEVec2Distance(&player.current_planet.position, &player.position) <= dist_from_planet_centre) {
			player.angle = AERadToDeg(atan2(player.position.y - player.current_planet.position.y, player.position.x - player.current_planet.position.x));
			player.state = PLAYER_ORBIT;
		}
	}
}

void Player::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(playerTex, 0, 0);

	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 20.f, 20.f);


	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, AEDegToRad(player.angle) + PI / 2);


	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, player.position.x, player.position.y);


	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void Player::free()
{

}

void Player::unload()
{
	AEGfxTextureUnload(playerTex);
}