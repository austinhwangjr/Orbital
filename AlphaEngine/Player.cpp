#include "AEEngine.h"
#include "Player.h"
#include <iostream>


AEGfxTexture* playerTex;

extern Player player;

f32 player_speed_scale;

f32* cam_x;
f32* cam_y;

bool player_leave_orbit;

// Test for multiple planet check
f32 shortest_distance;

// Tractor beam test
AEGfxTexture* tractorBeamTex;
f32 beam_x;
f32 beam_y;

void Player::load()
{
	// load texture
	playerTex = AEGfxTextureLoad("Assets/test-player.png");
	tractorBeamTex = AEGfxTextureLoad("Assets/TractorBeam.png");
}

void Player::init()
{
	player.position.x = 100.f;
	player.position.y = 100.f;
	player.angle = 0.f;

	// variables
	player.dist_from_planet = 75.f;
	player_speed_scale = 5.f;

	player.state = PLAYER_FLY;
	player_leave_orbit = true;

	shortest_distance = 0.f;

	// tractor beam
	beam_x = 0.f;
	beam_y = 0.f;
}

void Player::update(f64 frame_time)
{
	if (player.position.x <= AEGfxGetWinMinX() - 10) player.position.x = AEGfxGetWinMinX() - 10;
	else if (player.position.x >= AEGfxGetWinMaxX() + 10) player.position.x = AEGfxGetWinMaxX() + 10;
	if (player.position.y <= AEGfxGetWinMinY() - 10) player.position.y = AEGfxGetWinMinY() - 10;
	else if (player.position.y >= AEGfxGetWinMaxY() + 10) player.position.y = AEGfxGetWinMaxY() + 10;


	// Change player state
	player.direction.x = AECos(AEDegToRad(player.angle));
	player.direction.y = AESin(AEDegToRad(player.angle));
	AEVec2Normalize(&player.direction, &player.direction);

	if (player.state == PLAYER_ORBIT) {
		player.position.x = player.current_planet.position.x + player.dist_from_planet * AECos(AEDegToRad(player.angle));
		player.position.y = player.current_planet.position.y + player.dist_from_planet * AESin(AEDegToRad(player.angle));

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

			//player.position.x = player.current_planet.position.x + ((player.position.x - player.current_planet.position.x) * 1.6f);
			//player.position.y = player.current_planet.position.y + ((player.position.y - player.current_planet.position.y) * 1.6f);

			player.position.x += player.direction.x * dist_from_planet;
			player.position.y += player.direction.y * dist_from_planet;

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

		/*AEVec2 player_dir_vector;
		player_dir_vector.x = AECos(AEDegToRad(player.angle));
		player_dir_vector.y = AESin(AEDegToRad(player.angle));
		AEVec2Normalize(&player_dir_vector, &player_dir_vector);*/

		if (AEInputCheckCurr(AEVK_W)) {
			player.position.x += player.direction.x * player_speed_scale;
			player.position.y += player.direction.y * player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_A)) {

			player.angle += player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_S)) {
			player.position.x -= player.direction.x * player_speed_scale;
			player.position.y -= player.direction.y * player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_D)) {

			player.angle -= player_speed_scale;
		}

		if (AEVec2Distance(&player.current_planet.position, &player.position) <= player.dist_from_planet) {
			player.angle = AERadToDeg(atan2(player.position.y - player.current_planet.position.y, player.position.x - player.current_planet.position.x));
			player.state = PLAYER_ORBIT;
		}
	}

	// camera stuff lol
	/*if (AEInputCheckCurr(AEVK_F)) AEGfxSetCamPosition(player.current_planet.position.x, player.current_planet.position.y);
	else AEGfxSetCamPosition(0, 0);*/

	// if spacebar, draw box for test
	if (AEInputCheckCurr(AEVK_SPACE) && player.state == PLAYER_ORBIT) {
		beam_x = player.position.x - player.direction.x * 20;
		beam_y = player.position.y - player.direction.y * 20;
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


	// tractor beam
	if (AEInputCheckCurr(AEVK_SPACE) && player.state == PLAYER_ORBIT) {
		AEGfxTextureSet(tractorBeamTex, 0, 0);
		AEMtx33Scale(&scale, 20.f, 30.f);
		AEMtx33Rot(&rotate, AEDegToRad(player.angle) + PI / 2);
		AEMtx33Trans(&translate, beam_x, beam_y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	std::cout << sizeof(long double) << '\n';
}

void Player::free()
{

}

void Player::unload()
{
	AEGfxTextureUnload(playerTex);
	AEGfxTextureUnload(tractorBeamTex);
}