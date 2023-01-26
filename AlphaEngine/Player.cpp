#include "AEEngine.h"
#include "Player.h"


AEGfxTexture* playerTex;

Player player;

f32 dist_from_planet_centre;
f32 angle_around_planet;
f32 player_speed_scale;

enum PLAYER_STATES
{
	PLAYER_ORBIT = 0,
	PLAYER_FLY
};

bool player_leave_orbit;


void Player::load()
{
	// load texture
	playerTex = AEGfxTextureLoad("Assets/test-player.png");
}

void Player::init()
{
	player.position.x = 100.f;
	player.position.y = 100.f;

	// variables
	dist_from_planet_centre = 75.f;
	angle_around_planet = 0.f;
	player_speed_scale = 5.f;

	player.state = PLAYER_ORBIT;
	player_leave_orbit = true;
}

void Player::update(f64 frame_time)
{
	if (player.position.x <= AEGfxGetWinMinX() - 10) player.position.x = AEGfxGetWinMinX() - 10;
	else if (player.position.x >= AEGfxGetWinMaxX() + 10) player.position.x = AEGfxGetWinMaxX() + 10;
	if (player.position.y <= AEGfxGetWinMinY() - 10) player.position.y = AEGfxGetWinMinY() - 10;
	else if (player.position.y >= AEGfxGetWinMaxY() + 10) player.position.y = AEGfxGetWinMaxY() + 10;


	AEVec2 planet_pos;
	planet_pos.x = 1;
	planet_pos.y = 0;

	if (player.state == PLAYER_ORBIT) {
		player.position.x = dist_from_planet_centre * AECos(AEDegToRad(angle_around_planet));
		player.position.y = dist_from_planet_centre * AESin(AEDegToRad(angle_around_planet));

		if (AEInputCheckCurr(AEVK_A)) {
			angle_around_planet += 3.f;
		}

		if (AEInputCheckCurr(AEVK_D)) {
			angle_around_planet -= 3.f;
		}

		if (AEInputCheckPrev(AEVK_W)) {
			player_leave_orbit = false;
		}
		else player_leave_orbit = true;
		if (AEInputCheckCurr(AEVK_W) && player_leave_orbit) {

			player.position.x *= 1.6f;
			player.position.y *= 1.6f;

			player.state = PLAYER_FLY;
		}
	}

	else if (player.state == PLAYER_FLY) {

		AEVec2 player_dir_vector;
		player_dir_vector.x = AECos(AEDegToRad(angle_around_planet));
		player_dir_vector.y = AESin(AEDegToRad(angle_around_planet));
		AEVec2Normalize(&player_dir_vector, &player_dir_vector);

		if (AEInputCheckCurr(AEVK_W)) {
			player.position.x += player_dir_vector.x * player_speed_scale;
			player.position.y += player_dir_vector.y * player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_A)) {

			angle_around_planet += player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_S)) {
			player_dir_vector.x -= player_dir_vector.x * player_speed_scale;
			player_dir_vector.y -= player_dir_vector.y * player_speed_scale;
		}

		if (AEInputCheckCurr(AEVK_D)) {

			angle_around_planet -= player_speed_scale;
		}

		if (AEVec2Distance(&planet_pos, &player.position) <= dist_from_planet_centre) {
			angle_around_planet = AERadToDeg(atan2(player.position.y, player.position.x));
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
	AEMtx33Rot(&rotate, AEDegToRad(angle_around_planet) + PI / 2);


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