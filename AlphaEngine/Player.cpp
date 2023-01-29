#include "AEEngine.h"
#include "Player.h"
#include <iostream>

AEGfxTexture* player_tex;
Player player;

extern std::vector<Planets> planet_vector;

f32 player_speed_scale;

f32* cam_x;
f32* cam_y;

bool player_leave_orbit;

// Test for multiple planet check
f32 shortest_distance;

// Tractor beam test
AEGfxTexture* tractor_beam_tex;
f32 beam_x;
f32 beam_y;

// Test some variables
f32 collision_x;
f32 collision_y;
AEGfxTexture* Planet_Texture;


//AEGfxTexture* debris_tex;
//std::vector<Debris> debris_vectoray;

enum BUTTON_TYPE {
	SHOP_OPEN = 0,
	SHOP_OPTION
};

struct ShopOption
{
	AEVec2 position;
	f32 size;
	BUTTON_TYPE button_type;
};
AEGfxTexture* shop_icon_tex;
std::vector<ShopOption> button_list;

void Player::load()
{
	// load texture
	player_tex = AEGfxTextureLoad("Assets/test-player.png");
	tractor_beam_tex = AEGfxTextureLoad("Assets/TractorBeam.png");

	Planet_Texture = AEGfxTextureLoad("Assets/PlanetTexture.png");

	//debris_tex = AEGfxTextureLoad("Assets/Debris.png");
	shop_icon_tex = AEGfxTextureLoad("Assets/YellowTexture.png");
}

void Player::init()
{
	player.position.x = 100.f;
	player.position.y = 100.f;
	player.angle = 0.f;
	player.current_capacity = 0;

	// variables
	player.dist_from_planet = 90.f;
	player_speed_scale = 5.f;

	player.state = PLAYER_FLY;
	player_leave_orbit = true;

	shortest_distance = 0.f;

	// tractor beam
	beam_x = 0.f;
	beam_y = 0.f;

	collision_x = 0.f;
	collision_y = 0.f;


	// debris test
	/*for (int i = 0; i < 10; ++i) {
		Debris debris_test{};

		debris_test.angle = i * 30;

		debris_vectoray.push_back(debris_test);
	}*/

	// shop icon test
	ShopOption shop_open{};
	shop_open.size = 70.f;
	shop_open.position.x = AEGfxGetWinMaxX() - shop_open.size / 2;
	shop_open.position.y = AEGfxGetWinMaxY() * 2 / 3;
	button_list.push_back(shop_open);
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

			// debris test
			//debris_vectoray[0].position.x = planet_vector[0].position.x + 60;
			//debris_vectoray[0].position.y = planet_vector[0].position.y;
			//debris_exist = true;
		}

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
	if (AEInputCheckCurr(AEVK_F)) AEGfxSetCamPosition(player.current_planet.position.x, player.current_planet.position.y);
	else AEGfxSetCamPosition(0, 0);

	// debris test another one
	/*if (planet_vector.size() > 0) {
		for (int i = 0; i < debris_vectoray.size(); ++i) {
			debris_vectoray[i].position.x = planet_vector[0].position.x + 60 * AECos(AEDegToRad(debris_vectoray[i].angle));
			debris_vectoray[i].position.y = planet_vector[0].position.y + 60 * AESin(AEDegToRad(debris_vectoray[i].angle));
		}
	}*/

	// shop icon test
	s32 mouse_x, mouse_y;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);

	f32 mouse_x_actual = mouse_x - 800;
	f32	mouse_y_actual = 400 - mouse_y;

	if (button_list.size() == 1) {
		if ((button_list[0].position.x - button_list[0].size / 2 < mouse_x_actual) && (button_list[0].position.x + button_list[0].size / 2 > mouse_x_actual)) {
			if ((button_list[0].position.y - button_list[0].size / 2 < mouse_y_actual) && (button_list[0].position.y + button_list[0].size / 2 > mouse_y_actual)) {
				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					for (int i = 0; i < 3; ++i) {
						ShopOption player_upgrade;
						player_upgrade.size = 80.f;
						player_upgrade.position.x = button_list[0].position.x - 150;
						player_upgrade.position.y = (i + 1) * 90;
						player_upgrade.button_type = SHOP_OPTION;
						button_list.push_back(player_upgrade);
					}
				}
			}
		}
	}

	else if (button_list.size() == 4) {
		if ((button_list[0].position.x - button_list[0].size / 2 < mouse_x_actual) && (button_list[0].position.x + button_list[0].size / 2 > mouse_x_actual)) {
			if ((button_list[0].position.y - button_list[0].size / 2 < mouse_y_actual) && (button_list[0].position.y + button_list[0].size / 2 > mouse_y_actual)) {
				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					for (int i = 0; i < 3; ++i) {
						button_list.pop_back();
					}
				}
			}
		}
	}

	// if spacebar, draw box for test
	if (AEInputCheckCurr(AEVK_SPACE) && player.state == PLAYER_ORBIT) {
		beam_x = player.position.x - player.direction.x * 20;
		beam_y = player.position.y - player.direction.y * 20;

		/* = player.position.x - player.direction.x * 30;
		collision_y = player.position.y - player.direction.y * 30;*/

		// Space between planet and debris to change to proper variable
		collision_x = player.current_planet.position.x + 70 * AECos(AEDegToRad(player.angle));
		collision_y = player.current_planet.position.y + 70 * AESin(AEDegToRad(player.angle));

		AEVec2 coll_pos;
		coll_pos.x = collision_x;
		coll_pos.y = collision_y;

		if (player.current_planet.debris_vector.size() > 0)
		{
			for (int i = 0; i < player.current_planet.debris_vector.size(); ++i) {
				if (AEVec2Distance(&coll_pos, &player.current_planet.debris_vector[i].position) <= 10) {
					player.current_planet.debris_vector.erase(player.current_planet.debris_vector.begin() + i);
					//player.current_planet.debris_vector.pop_back();
					player.current_capacity++;
					break;
				}
			}
		}

		/*for (int i = 0; i < debris_vector.size(); ++i) {
			if (AEVec2Distance(&coll_pos, &debris_vector[i].position) <= 10) {
				debris_vector.erase(debris_vector.begin() + i);
				player.current_capacity++;
			}
		}*/
	}
}

void Player::draw(AEGfxVertexList* pMesh)
{
	AEGfxTextureSet(player_tex, 0, 0);

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

	/*if (debris_vectoray.size() > 0) {
		for (int i = 0; i < debris_vectoray.size(); ++i) {
			AEGfxTextureSet(debris_tex, 0, 0);
			AEMtx33Scale(&scale, 20.f, 20.f);
			AEMtx33Rot(&rotate, AEDegToRad(player.angle) + PI / 2);
			AEMtx33Trans(&translate, debris_vectoray[i].position.x, debris_vectoray[i].position.y);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}*/


	// tractor beam
	if (AEInputCheckCurr(AEVK_SPACE) && player.state == PLAYER_ORBIT) {
		// test collision
		AEGfxTextureSet(Planet_Texture, 0, 0);
		AEMtx33Scale(&scale, 20.f, 20.f);
		AEMtx33Rot(&rotate, AEDegToRad(player.angle) + PI / 2);
		AEMtx33Trans(&translate, collision_x, collision_y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(tractor_beam_tex, 0, 0);
		AEMtx33Scale(&scale, 20.f, 30.f);
		AEMtx33Rot(&rotate, AEDegToRad(player.angle) + PI / 2);
		AEMtx33Trans(&translate, beam_x, beam_y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Shop icon test
	AEGfxTextureSet(shop_icon_tex, 0, 0);
	for (int i = 0; i < button_list.size(); ++i) {
		if (button_list[i].button_type == SHOP_OPEN) {
			AEMtx33Scale(&scale, button_list[i].size, button_list[i].size);
		}
		else if (button_list[i].button_type == SHOP_OPTION) {
			AEMtx33Scale(&scale, button_list[i].size * 2, button_list[i].size);
		}

		AEMtx33Rot(&rotate, 0.f);
		AEMtx33Trans(&translate, button_list[i].position.x, button_list[i].position.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
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

	AEGfxTextureUnload(Planet_Texture);

	//AEGfxTextureUnload(debris_tex);
	AEGfxTextureUnload(shop_icon_tex);
}